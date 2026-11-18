/*
 * BM83_UART.c
 *
 *  Created on: Nov 17, 2026
 *      Author: Jonathan
 */
#include "BM83_UART.h"

enum
{
    CMD_MMI = 0x02, CMD_MUSIC = 0x04, CMD_LINK = 0x0D, CMD_EVENT_ACK = 0x14,
    EVT_ACK = 0x00, EVT_STATUS = 0x01, EVT_LINK = 0x1E,
    MMI_ON_PRESS = 0x51, MMI_ON_RELEASE = 0x52,
    MMI_OFF_PRESS = 0x53, MMI_OFF_RELEASE = 0x54, MMI_PAIR = 0x5D,
    MEDIA_LINK_MASK = 0x05, MUSIC_TOGGLE = 0x07
};

typedef enum
{
    BT_IDLE, BT_RESET_HOLD, BT_RESET_WAKE, BT_BOOT,
    BT_ON_PRESS, BT_ON_RELEASE, BT_PAIR, BT_READY, BT_LINK, BT_TOGGLE,
    BT_START_FAILED, BT_OFF_PRESS, BT_OFF_RELEASE, BT_OFF_SETTLE, BT_OFF
} BtState;

static UART_HandleTypeDef *uart;
static BtState state;
static uint32_t state_since;
static bool pair_on_start;
static bool power_attempted;

/* The UART interrupt writes incoming bytes. The main loop reads them. */
static uint8_t rx_byte;
static volatile uint8_t rx_queue[256];
static volatile uint8_t rx_head, rx_tail;
static volatile uint32_t rx_errors;
static uint32_t seen_rx_errors;

/* Keep the first four bytes of each frame and checksum the whole frame. */
static uint8_t rx_step, rx_sum, rx_data[4];
static uint16_t rx_length, rx_count;
static uint32_t rx_last_ms;

static uint8_t pending_command;
static int16_t command_ack;
static uint32_t command_since;
static bool link_received, media_connected;
static bool power_off_reported;

/* Debugger: -1 waiting, -2 timeout, -3 TX failure, -4 no media link. */
static volatile int16_t bm83_last_result;

static void Wake(GPIO_PinState level)
{
    HAL_GPIO_WritePin(BT_WAKE_GPIO_Port, BT_WAKE_Pin, level);
}

static void Reset(GPIO_PinState level)
{
    HAL_GPIO_WritePin(BT_RESET_GPIO_Port, BT_RESET_Pin, level);
}

static void Enter(BtState next)
{
    state = next;
    state_since = HAL_GetTick();
}

static HAL_StatusTypeDef Transmit(uint8_t command, uint8_t first,
                                  uint8_t second, uint8_t count)
{
    uint8_t length = (uint8_t)(1U + count);
    uint8_t packet[7] = {0xAA, 0x00, length, command, first, second, 0};
    uint8_t sum = 0U;

    for (uint8_t i = 1U; i < length + 3U; ++i)
    {
        sum = (uint8_t)(sum + packet[i]);
    }
    packet[length + 3U] = (uint8_t)(0U - sum);

    bool hold_wake = (state == BT_BOOT);
    if (!hold_wake)
    {
        Wake(GPIO_PIN_SET);
        HAL_Delay(3U);
    }

    HAL_StatusTypeDef status = HAL_UART_Transmit(uart, packet, length + 4U, 10U);
    if (!hold_wake)
    {
        HAL_Delay(2U);
        Wake(GPIO_PIN_RESET);
    }
    return status;
}

static void StartCommand(BtState next, uint8_t command, uint8_t action)
{
    Enter(next);
    command_ack = -1;
    bm83_last_result = -1;
    link_received = false;

    if (Transmit(command, 0x00, action, command == CMD_LINK ? 1U : 2U) == HAL_OK)
    {
        pending_command = command;
        command_since = HAL_GetTick();
    }
    else
    {
        command_ack = -3;
    }
}

static void HandleFrame(void)
{
    if (rx_data[0] == EVT_ACK)
    {
        if (rx_length >= 3U && pending_command != 0U &&
            rx_data[1] == pending_command)
        {
            command_ack = rx_data[2];
        }
        return;
    }

    if (rx_data[0] == EVT_LINK && rx_length >= 8U && state == BT_LINK)
    {
        link_received = true;
        media_connected = ((rx_data[2] | rx_data[3]) & MEDIA_LINK_MASK) != 0U;
    }

    if (rx_data[0] == EVT_STATUS && rx_length >= 2U && rx_data[1] == 0x00U)
    {
        power_off_reported = true;
    }

    (void)Transmit(CMD_EVENT_ACK, rx_data[0], 0U, 1U);
}

static void ParseByte(uint8_t byte)
{
    uint32_t now = HAL_GetTick();
    if ((uint32_t)(now - rx_last_ms) >= 40U)
    {
        rx_step = 0U;
    }
    rx_last_ms = now;

    switch (rx_step)
    {
        case 0:
            if (byte == 0xAAU) rx_step = 1U;
            break;
        case 1:
            rx_sum = byte;
            rx_length = (uint16_t)byte << 8;
            rx_step = 2U;
            break;
        case 2:
            rx_sum = (uint8_t)(rx_sum + byte);
            rx_length |= byte;
            rx_count = 0U;
            rx_step = (rx_length == 0U) ? 0U : 3U;
            break;
        default:
            rx_sum = (uint8_t)(rx_sum + byte);
            if (rx_count < rx_length)
            {
                if (rx_count < sizeof(rx_data)) rx_data[rx_count] = byte;
                ++rx_count;
            }
            else
            {
                rx_step = 0U;
                if (rx_sum == 0U) HandleFrame();
            }
            break;
    }
}

HAL_StatusTypeDef BM83_Init(UART_HandleTypeDef *port)
{
    uart = port;
    pair_on_start = HAL_GPIO_ReadPin(BTN_PLAY_PAUSE_GPIO_Port,
                                     BTN_PLAY_PAUSE_Pin) == GPIO_PIN_RESET;
    Wake(GPIO_PIN_RESET);
    Reset(GPIO_PIN_RESET);
    Enter(BT_IDLE);
    return HAL_UART_Receive_IT(uart, &rx_byte, 1U);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *port)
{
    if (port == uart)
    {
        uint8_t next = (uint8_t)(rx_head + 1U);
        if (next != rx_tail)
        {
            rx_queue[rx_head] = rx_byte;
            rx_head = next;
        }
        else
        {
            ++rx_errors;
        }
        (void)HAL_UART_Receive_IT(port, &rx_byte, 1U);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *port)
{
    if (port == uart)
    {
        ++rx_errors;
        (void)HAL_UART_Receive_IT(port, &rx_byte, 1U);
    }
}

void BM83_Process(bool audio_allowed, bool play_pause_pressed)
{
    if (state == BT_OFF)
    {
        return;
    }

    if (seen_rx_errors != rx_errors)
    {
        seen_rx_errors = rx_errors;
        rx_tail = rx_head;
        rx_step = 0U;
    }
    for (uint8_t i = 0U; i < 64U && rx_tail != rx_head; ++i)
    {
        uint8_t byte = rx_queue[rx_tail];
        ++rx_tail;
        ParseByte(byte);
    }

    if (pending_command != 0U)
    {
        uint32_t elapsed = (uint32_t)(HAL_GetTick() - command_since);
        if ((command_ack == -1 && elapsed < 200U) ||
            (command_ack == 0 && state == BT_LINK && !link_received && elapsed < 600U))
        {
            return;
        }
        if (command_ack == -1 || (state == BT_LINK && command_ack == 0 && !link_received))
        {
            command_ack = -2;
        }
        pending_command = 0U;
    }

    if (!audio_allowed && state > BT_IDLE && state < BT_OFF_PRESS)
    {
        if (power_attempted)
        {
            power_off_reported = false;
            StartCommand(BT_OFF_PRESS, CMD_MMI, MMI_OFF_PRESS);
        }
        else
        {
            Wake(GPIO_PIN_RESET);
            Reset(GPIO_PIN_RESET);
            Enter(BT_OFF);
        }
        return;
    }

    uint32_t elapsed = (uint32_t)(HAL_GetTick() - state_since);
    switch (state)
    {
        case BT_IDLE:
            if (audio_allowed) Enter(BT_RESET_HOLD);
            break;

        case BT_RESET_HOLD:
            if (elapsed >= 200U)
            {
                Wake(GPIO_PIN_SET);
                Enter(BT_RESET_WAKE);
            }
            break;

        case BT_RESET_WAKE:
            if (elapsed >= 20U)
            {
                Reset(GPIO_PIN_SET);
                Enter(BT_BOOT);
            }
            break;

        case BT_BOOT:
            if (elapsed >= 500U)
            {
                Wake(GPIO_PIN_RESET);
                power_attempted = true;
                StartCommand(BT_ON_PRESS, CMD_MMI, MMI_ON_PRESS);
            }
            break;

        case BT_ON_PRESS:
            StartCommand(BT_ON_RELEASE, CMD_MMI, MMI_ON_RELEASE);
            break;

        case BT_ON_RELEASE:
            bm83_last_result = command_ack;
            Enter(command_ack == 0 ? BT_READY : BT_START_FAILED);
            break;

        case BT_READY:
            if (pair_on_start && elapsed >= 500U)
            {
                pair_on_start = false;
                StartCommand(BT_PAIR, CMD_MMI, MMI_PAIR);
            }
            else if (!pair_on_start && play_pause_pressed)
            {
                StartCommand(BT_LINK, CMD_LINK, 0U);
            }
            break;

        case BT_LINK:
            if (command_ack == 0 && media_connected)
            {
                StartCommand(BT_TOGGLE, CMD_MUSIC, MUSIC_TOGGLE);
                break;
            }
            bm83_last_result = command_ack == 0 ? -4 : command_ack;
            Enter(BT_READY);
            break;

        case BT_PAIR:
        case BT_TOGGLE:
            bm83_last_result = command_ack;
            Enter(BT_READY);
            break;

        case BT_OFF_PRESS:
            StartCommand(BT_OFF_RELEASE, CMD_MMI, MMI_OFF_RELEASE);
            break;

        case BT_OFF_RELEASE:
            bm83_last_result = command_ack;
            Enter(BT_OFF_SETTLE);
            break;

        case BT_OFF_SETTLE:
            if (elapsed >= 2000U)
            {
                if (power_off_reported)
                {
                    /* Drive the host TX line low after the module settles. */
                    GPIO_InitTypeDef tx = {0};
                    tx.Pin = GPIO_PIN_9;
                    tx.Mode = GPIO_MODE_OUTPUT_PP;
                    tx.Pull = GPIO_NOPULL;
                    tx.Speed = GPIO_SPEED_FREQ_LOW;
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
                    HAL_GPIO_Init(GPIOA, &tx);
                }
                Enter(BT_OFF);
            }
            break;

        default:
            break;
    }
}

