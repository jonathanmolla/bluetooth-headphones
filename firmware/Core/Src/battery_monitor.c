#include "battery_monitor.h"
#include <stddef.h>

#define BATTERY_SAMPLE_COUNT 8U
#define BATTERY_ADC_MAX      4095U
#define BATTERY_DIVIDER_GAIN 2U
#define BATTERY_TIMEOUT_MS   20U

static HAL_StatusTypeDef SelectChannel(
    ADC_HandleTypeDef *adc,
    uint32_t channel)
{
    const ADC_ChannelConfTypeDef config =
    {
        .Channel = channel,
        .Rank = ADC_REGULAR_RANK_1,
        .SamplingTime = ADC_SAMPLINGTIME_COMMON_1
    };

    return HAL_ADC_ConfigChannel(adc, &config);
}

HAL_StatusTypeDef BatteryMonitor_Init(ADC_HandleTypeDef *adc)
{
    if (adc == NULL)
    {
        return HAL_ERROR;
    }

    HAL_StatusTypeDef status = HAL_ADCEx_Calibration_Start(adc);
    if (status != HAL_OK)
    {
        return status;
    }

    /* Enable the internal reference path and let it settle. */
    status = SelectChannel(adc, ADC_CHANNEL_VREFINT);
    if (status != HAL_OK)
    {
        return status;
    }

    HAL_Delay(1U);
    return SelectChannel(adc, ADC_CHANNEL_14);
}

static HAL_StatusTypeDef SampleSum(
    ADC_HandleTypeDef *adc,
    uint32_t *sum)
{
    *sum = 0U;

    for (uint32_t i = 0U; i < BATTERY_SAMPLE_COUNT; ++i)
    {
        HAL_StatusTypeDef status = HAL_ADC_Start(adc);
        if (status != HAL_OK)
        {
            return status;
        }

        status = HAL_ADC_PollForConversion(adc, BATTERY_TIMEOUT_MS);
        if (status != HAL_OK)
        {
            (void)HAL_ADC_Stop(adc);
            return status;
        }

        uint32_t raw = HAL_ADC_GetValue(adc);

        status = HAL_ADC_Stop(adc);
        if (status != HAL_OK)
        {
            return status;
        }

        if (raw > BATTERY_ADC_MAX)
        {
            return HAL_ERROR;
        }

        *sum += raw;
    }

    return HAL_OK;
}

HAL_StatusTypeDef BatteryMonitor_Read(
    ADC_HandleTypeDef *adc,
    uint32_t reference_mv,
    BatteryReading *output)
{
    if (adc == NULL || output == NULL ||
        reference_mv == 0U || reference_mv > 3600U)
    {
        return HAL_ERROR;
    }

    uint32_t sum;
    HAL_StatusTypeDef status = SampleSum(adc, &sum);
    if (status != HAL_OK)
    {
        return status;
    }

    const uint32_t denominator = BATTERY_SAMPLE_COUNT * BATTERY_ADC_MAX;

    BatteryReading result;
    result.raw = (uint16_t)((sum + BATTERY_SAMPLE_COUNT / 2U) /
                            BATTERY_SAMPLE_COUNT);
    result.pin_mv = (sum * reference_mv + denominator / 2U) / denominator;
    result.battery_mv = (sum * reference_mv * BATTERY_DIVIDER_GAIN +
                         denominator / 2U) / denominator;

    *output = result;
    return HAL_OK;
}

HAL_StatusTypeDef BatteryMonitor_ReadTracked(
    ADC_HandleTypeDef *adc,
    BatteryReading *output)
{
    if (adc == NULL || output == NULL)
    {
        return HAL_ERROR;
    }

    HAL_StatusTypeDef status = SelectChannel(adc, ADC_CHANNEL_VREFINT);
    if (status != HAL_OK)
    {
        return status;
    }

    uint32_t sum;
    status = SampleSum(adc, &sum);

    /* Return to PA7 even if reading the internal reference failed. */
    HAL_StatusTypeDef restore_status = SelectChannel(adc, ADC_CHANNEL_14);
    if (status != HAL_OK)
    {
        return status;
    }
    if (restore_status != HAL_OK)
    {
        return restore_status;
    }

    uint32_t reference_raw = (sum + BATTERY_SAMPLE_COUNT / 2U) /
                             BATTERY_SAMPLE_COUNT;
    if (reference_raw == 0U || reference_raw >= BATTERY_ADC_MAX)
    {
        return HAL_ERROR;
    }

    uint32_t reference_mv =
        __HAL_ADC_CALC_VREFANALOG_VOLTAGE(reference_raw, ADC_RESOLUTION_12B);
    if (reference_mv < 1710U || reference_mv > 3600U)
    {
        return HAL_ERROR;
    }

    return BatteryMonitor_Read(adc, reference_mv, output);
}
