#include "headphone_controls.h"
#include <assert.h>
#include <stdio.h>
#include <limits.h>

GPIO_TypeDef test_port;
static I2C_HandleTypeDef bus;
static uint32_t tick;
static GPIO_PinState pins[4];
static uint8_t registers[4];
static HAL_StatusTypeDef next_status = HAL_OK;
static unsigned writes;
static unsigned fail_on_write;

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *port, uint16_t pin)
{ (void)port; return pins[pin]; }
void HAL_GPIO_WritePin(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state)
{ (void)port; pins[pin] = state; }
uint32_t HAL_GetTick(void) { return tick; }
void HAL_Delay(uint32_t ms) { tick += ms; }
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *handle,
    uint16_t address, uint16_t reg, uint16_t reg_size,
    uint8_t *data, uint16_t size, uint32_t timeout)
{
    assert(handle == &bus && address == 0xC0U);
    assert(reg >= 1U && reg <= 3U && reg_size == I2C_MEMADD_SIZE_8BIT);
    assert(size == 1U && timeout == 20U);
    ++writes;
    if (writes == fail_on_write) return HAL_TIMEOUT;
    if (next_status != HAL_OK) return next_status;
    registers[reg] = *data;
    return HAL_OK;
}
static void poll_after(uint32_t ms)
{
    tick += ms;
    assert(HeadphoneControls_Process() == HAL_OK);
}
static void set_button(unsigned pin, GPIO_PinState state)
{
    pins[pin] = state;
    poll_after(0U);
    poll_after(30U);
}
static void click(unsigned pin)
{
    set_button(pin, GPIO_PIN_RESET);
    set_button(pin, GPIO_PIN_SET);
}
int main(void)
{
    for (unsigned i = 0; i < 4U; ++i) pins[i] = GPIO_PIN_SET;
    assert(HeadphoneControls_Init(NULL) == HAL_ERROR);
    assert(pins[3] == GPIO_PIN_RESET);
    assert(HeadphoneControls_Init(&bus) == HAL_OK);
    assert(writes == 4U && registers[1] == 0xC0U);
    assert(registers[2] == 0U && registers[3] == 0U);

    /* Bounce shorter than 30 ms must not register a press. */
    pins[0] = GPIO_PIN_RESET; poll_after(0U); poll_after(10U);
    pins[0] = GPIO_PIN_SET; poll_after(0U); poll_after(10U);
    assert(HeadphoneControls_GetVolume() == 0U);
    set_button(0U, GPIO_PIN_RESET);
    assert(HeadphoneControls_GetVolume() == 1U);
    poll_after(1000U);
    assert(HeadphoneControls_GetVolume() == 1U);
    set_button(0U, GPIO_PIN_SET);
    click(1U); click(1U);
    assert(HeadphoneControls_GetVolume() == 0U);
    assert(HeadphoneControls_SetVolume(63U) == HAL_OK);
    click(0U);
    assert(HeadphoneControls_GetVolume() == 63U);
    assert(HeadphoneControls_SetVolume(64U) == HAL_ERROR);

    assert(HeadphoneControls_SetMute(true) == HAL_OK);
    click(1U);
    assert(registers[2] == (0xC0U | 62U));
    assert(HeadphoneControls_SetMute(false) == HAL_OK);
    assert(registers[2] == 62U);
    click(2U);
    assert(HeadphoneControls_TakePlayPause());
    assert(!HeadphoneControls_TakePlayPause());

    pins[0] = pins[1] = GPIO_PIN_RESET;
    poll_after(0U); poll_after(30U);
    assert(HeadphoneControls_GetVolume() == 62U);
    pins[0] = pins[1] = GPIO_PIN_SET;
    poll_after(0U); poll_after(30U);

    /* Debounce across UINT32_MAX. */
    tick = UINT32_MAX - 10U;
    set_button(0U, GPIO_PIN_RESET);
    assert(HeadphoneControls_GetVolume() == 63U);
    set_button(0U, GPIO_PIN_SET);

    next_status = HAL_TIMEOUT;
    assert(HeadphoneControls_SetVolume(20U) == HAL_TIMEOUT);
    assert(HeadphoneControls_GetVolume() == 63U);
    assert(pins[3] == GPIO_PIN_RESET);
    assert(HeadphoneControls_Process() == HAL_ERROR);
    next_status = HAL_OK;

    /* A held startup button cannot fire until released and pressed again. */
    pins[0] = GPIO_PIN_RESET;
    assert(HeadphoneControls_Init(&bus) == HAL_OK);
    poll_after(100U);
    assert(HeadphoneControls_GetVolume() == 0U);
    set_button(0U, GPIO_PIN_SET);
    click(0U);
    assert(HeadphoneControls_GetVolume() == 1U);

    /* Every initialization write can fail without leaving amp enabled. */
    for (unsigned stage = 1U; stage <= 4U; ++stage) {
        fail_on_write = writes + stage;
        assert(HeadphoneControls_Init(&bus) == HAL_TIMEOUT);
        assert(pins[3] == GPIO_PIN_RESET);
        assert(HeadphoneControls_Process() == HAL_ERROR);
    }
    puts("PASS: debounce, held buttons, limits, mute, play/pause, tick wrap, I2C errors");
    return 0;
}
