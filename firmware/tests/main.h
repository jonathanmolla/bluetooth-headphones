/* Host-only HAL substitute. Never copy this header into Core/Inc. */
#ifndef TEST_MAIN_H
#define TEST_MAIN_H
#include <stdint.h>
typedef enum { HAL_OK, HAL_ERROR, HAL_BUSY, HAL_TIMEOUT } HAL_StatusTypeDef;
typedef enum { GPIO_PIN_RESET, GPIO_PIN_SET } GPIO_PinState;
typedef struct { int unused; } GPIO_TypeDef;
typedef struct { int unused; } I2C_HandleTypeDef;
typedef struct { int unused; } ADC_HandleTypeDef;
typedef struct
{
    uint32_t Channel;
    uint32_t Rank;
    uint32_t SamplingTime;
} ADC_ChannelConfTypeDef;
#define ADC_CHANNEL_VREFINT 17U
#define ADC_CHANNEL_14 14U
#define ADC_REGULAR_RANK_1 1U
#define ADC_SAMPLINGTIME_COMMON_1 1U
#define ADC_RESOLUTION_12B 12U
#define __HAL_ADC_CALC_VREFANALOG_VOLTAGE(raw, resolution) \
    Mock_ADC_CalcVrefVoltage((raw), (resolution))
extern GPIO_TypeDef test_port;
#define BTN_VOL_UP_GPIO_Port (&test_port)
#define BTN_VOL_DOWN_GPIO_Port (&test_port)
#define BTN_PLAY_PAUSE_GPIO_Port (&test_port)
#define AMP_ENABLE_GPIO_Port (&test_port)
#define BTN_VOL_UP_Pin 0U
#define BTN_VOL_DOWN_Pin 1U
#define BTN_PLAY_PAUSE_Pin 2U
#define AMP_ENABLE_Pin 3U
#define I2C_MEMADD_SIZE_8BIT 1U
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *port, uint16_t pin);
void HAL_GPIO_WritePin(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state);
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t ms);
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *bus,
    uint16_t address, uint16_t reg, uint16_t reg_size,
    uint8_t *data, uint16_t size, uint32_t timeout);
HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef *adc);
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef *adc,
    const ADC_ChannelConfTypeDef *config);
uint32_t Mock_ADC_CalcVrefVoltage(uint32_t raw, uint32_t resolution);
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef *adc);
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef *adc, uint32_t timeout);
uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef *adc);
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef *adc);
#endif
