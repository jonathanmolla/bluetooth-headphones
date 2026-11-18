/*
 * Headphones_control.c
 *
 *  Created on: Nov 17, 2026
 *      Author: Jonathan
 */


#include "headphone_controls.h"
#include <stddef.h>

/* TPA6130A2 address, shifted for STM32 HAL. */
#define AMP_ADDRESS       ((uint16_t)(0x60U << 1))

#define AMP_CONTROL_REG   0x01U
#define AMP_VOLUME_REG    0x02U
#define AMP_IMPEDANCE_REG 0x03U

#define AMP_BOTH_CHANNELS 0xC0U
#define AMP_MUTE_BOTH     0xC0U

#define I2C_TIMEOUT_MS    20U
#define DEBOUNCE_MS       30U

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIO_PinState last_raw;
    GPIO_PinState stable;
    uint32_t changed_at;
} Button;

static Button buttons[] =
{
    { .port = BTN_VOL_UP_GPIO_Port,
      .pin = BTN_VOL_UP_Pin },

    { .port = BTN_VOL_DOWN_GPIO_Port,
      .pin = BTN_VOL_DOWN_Pin },

    { .port = BTN_PLAY_PAUSE_GPIO_Port,
      .pin = BTN_PLAY_PAUSE_Pin }
};

static I2C_HandleTypeDef *amp_bus;
static uint8_t volume;
static bool muted;
static bool ready;
static bool play_pause_pending;


/* Write one amplifier register. */
static HAL_StatusTypeDef amp_write(uint8_t reg, uint8_t value)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(
        amp_bus,
        AMP_ADDRESS,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        &value,
        1U,
        I2C_TIMEOUT_MS
    );

    if (status != HAL_OK)
    {
        HAL_GPIO_WritePin(
            AMP_ENABLE_GPIO_Port,
            AMP_ENABLE_Pin,
            GPIO_PIN_RESET
        );

        ready = false;
    }

    return status;
}


/* Detect one debounced press. Buttons are active low. */
static bool button_pressed(Button *button, uint32_t now)
{
    GPIO_PinState raw = HAL_GPIO_ReadPin(
        button->port,
        button->pin
    );

    if (raw != button->last_raw)
    {
        button->last_raw = raw;
        button->changed_at = now;
    }

    if ((raw != button->stable) &&
        ((uint32_t)(now - button->changed_at) >= DEBOUNCE_MS))
    {
        button->stable = raw;

        return raw == GPIO_PIN_RESET;
    }

    return false;
}


/* Initialize the amplifier and button state. */
HAL_StatusTypeDef HeadphoneControls_Init(I2C_HandleTypeDef *bus)
{
    ready = false;
    play_pause_pending = false;

    HAL_GPIO_WritePin(
        AMP_ENABLE_GPIO_Port,
        AMP_ENABLE_Pin,
        GPIO_PIN_RESET
    );

    if (bus == NULL)
    {
        return HAL_ERROR;
    }

    amp_bus = bus;
    volume = 0U;
    muted = true;

    HAL_Delay(1U);

    HAL_GPIO_WritePin(
        AMP_ENABLE_GPIO_Port,
        AMP_ENABLE_Pin,
        GPIO_PIN_SET
    );

    HAL_Delay(10U);

    /* Keep outputs muted during configuration. */
    HAL_StatusTypeDef status =
        amp_write(AMP_VOLUME_REG, AMP_MUTE_BOTH);

    if (status != HAL_OK)
    {
        return status;
    }

    /* Disable high-impedance output mode. */
    status = amp_write(AMP_IMPEDANCE_REG, 0x00U);

    if (status != HAL_OK)
    {
        return status;
    }

    /* Stereo mode, both channels enabled. */
    status = amp_write(AMP_CONTROL_REG, AMP_BOTH_CHANNELS);

    if (status != HAL_OK)
    {
        return status;
    }

    HAL_Delay(10U);

    /* Unmute at the lowest gain setting. */
    status = amp_write(AMP_VOLUME_REG, volume);

    if (status != HAL_OK)
    {
        return status;
    }

    muted = false;

    uint32_t now = HAL_GetTick();

    for (size_t i = 0U;
         i < sizeof(buttons) / sizeof(buttons[0]);
         ++i)
    {
        buttons[i].last_raw = HAL_GPIO_ReadPin(
            buttons[i].port,
            buttons[i].pin
        );

        buttons[i].stable = buttons[i].last_raw;
        buttons[i].changed_at = now;
    }

    ready = true;

    return HAL_OK;
}


/* Change volume while preserving the mute state. */
HAL_StatusTypeDef HeadphoneControls_SetVolume(uint8_t gain_code)
{
    if (!ready || gain_code > 63U)
    {
        return HAL_ERROR;
    }

    uint8_t value =
        gain_code | (muted ? AMP_MUTE_BOTH : 0U);

    HAL_StatusTypeDef status =
        amp_write(AMP_VOLUME_REG, value);

    if (status == HAL_OK)
    {
        volume = gain_code;
    }

    return status;
}


uint8_t HeadphoneControls_GetVolume(void)
{
    return volume;
}


/* Mute/unmute both amplifier channels. */
HAL_StatusTypeDef HeadphoneControls_SetMute(bool mute)
{
    if (!ready)
    {
        return HAL_ERROR;
    }

    uint8_t value =
        volume | (mute ? AMP_MUTE_BOTH : 0U);

    HAL_StatusTypeDef status =
        amp_write(AMP_VOLUME_REG, value);

    if (status == HAL_OK)
    {
        muted = mute;
    }

    return status;
}


/* Check buttons; make one volume change per press. */
HAL_StatusTypeDef HeadphoneControls_Process(void)
{
    if (!ready)
    {
        return HAL_ERROR;
    }

    uint32_t now = HAL_GetTick();

    bool up = button_pressed(&buttons[0], now);
    bool down = button_pressed(&buttons[1], now);

    if (button_pressed(&buttons[2], now))
    {
        play_pause_pending = true;
    }

    /* Ignore volume up if volume down is also held. */
    if (up &&
        buttons[1].last_raw == GPIO_PIN_SET &&
        volume < 63U)
    {
        return HeadphoneControls_SetVolume(
            (uint8_t)(volume + 1U)
        );
    }

    /* Ignore volume down if volume up is also held. */
    if (down &&
        buttons[0].last_raw == GPIO_PIN_SET &&
        volume > 0U)
    {
        return HeadphoneControls_SetVolume(
            (uint8_t)(volume - 1U)
        );
    }

    return HAL_OK;
}


/* Consume the pending play/pause event. */
bool HeadphoneControls_TakePlayPause(void)
{
    bool pending = play_pause_pending;

    play_pause_pending = false;

    return pending;
}
