#include "battery_monitor.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static ADC_HandleTypeDef handle;
static uint32_t sample;
static uint32_t reference_sample;
static uint32_t channel;
static unsigned starts, stops;
static HAL_StatusTypeDef start_status = HAL_OK;
static HAL_StatusTypeDef poll_status = HAL_OK;
static HAL_StatusTypeDef stop_status = HAL_OK;
static HAL_StatusTypeDef calibration_status = HAL_OK;
HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef *adc)
{ assert(adc == &handle); return calibration_status; }
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef *adc,
    const ADC_ChannelConfTypeDef *config)
{
    assert(adc == &handle && config->Rank == ADC_REGULAR_RANK_1);
    assert(config->SamplingTime == ADC_SAMPLINGTIME_COMMON_1);
    assert(config->Channel == ADC_CHANNEL_VREFINT || config->Channel == ADC_CHANNEL_14);
    channel = config->Channel;
    return HAL_OK;
}
uint32_t Mock_ADC_CalcVrefVoltage(uint32_t raw, uint32_t resolution)
{
    assert(raw == reference_sample && resolution == ADC_RESOLUTION_12B);
    return 3000U;
}
void HAL_Delay(uint32_t ms) { assert(ms == 1U); }
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef *adc)
{ assert(adc == &handle); ++starts; return start_status; }
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef *adc, uint32_t timeout)
{ assert(adc == &handle && timeout == 20U); return poll_status; }
uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef *adc)
{ assert(adc == &handle); return channel == ADC_CHANNEL_VREFINT ? reference_sample : sample; }
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef *adc)
{ assert(adc == &handle); ++stops; return stop_status; }
int main(void)
{
    BatteryReading result = {0};
    assert(BatteryMonitor_Init(NULL) == HAL_ERROR);
    assert(BatteryMonitor_Init(&handle) == HAL_OK);
    calibration_status = HAL_TIMEOUT;
    assert(BatteryMonitor_Init(&handle) == HAL_TIMEOUT);
    assert(BatteryMonitor_Read(NULL, 3000U, &result) == HAL_ERROR);
    assert(BatteryMonitor_Read(&handle, 0U, &result) == HAL_ERROR);
    assert(BatteryMonitor_Read(&handle, 3601U, &result) == HAL_ERROR);
    assert(BatteryMonitor_Read(&handle, 3000U, NULL) == HAL_ERROR);
    sample = 0U;
    assert(BatteryMonitor_Read(&handle, 3000U, &result) == HAL_OK);
    assert(result.raw == 0U && result.battery_mv == 0U);
    assert(starts == 8U && stops == 8U);
    sample = 2525U;
    assert(BatteryMonitor_Read(&handle, 3000U, &result) == HAL_OK);
    assert(result.raw == 2525U && result.pin_mv == 1850U);
    assert(result.battery_mv == 3700U);
    sample = 4095U;
    assert(BatteryMonitor_Read(&handle, 3600U, &result) == HAL_OK);
    assert(result.pin_mv == 3600U && result.battery_mv == 7200U);
    /* Rail-sized readings are exposed, not silently clamped to a cell voltage. */
    sample = 4096U;
    assert(BatteryMonitor_Read(&handle, 3000U, &result) == HAL_ERROR);
    assert(result.battery_mv == 7200U);
    poll_status = HAL_TIMEOUT;
    unsigned previous_stops = stops;
    assert(BatteryMonitor_Read(&handle, 3000U, &result) == HAL_TIMEOUT);
    assert(stops == previous_stops + 1U && result.battery_mv == 7200U);
    poll_status = HAL_OK;
    start_status = HAL_BUSY;
    assert(BatteryMonitor_Read(&handle, 3000U, &result) == HAL_BUSY);
    start_status = HAL_OK;
    stop_status = HAL_ERROR;
    assert(BatteryMonitor_Read(&handle, 3000U, &result) == HAL_ERROR);
    assert(result.battery_mv == 7200U);
    stop_status = HAL_OK;
    reference_sample = 1655U;
    sample = 2525U;
    assert(BatteryMonitor_ReadTracked(&handle, &result) == HAL_OK);
    assert(result.battery_mv == 3700U && channel == ADC_CHANNEL_14);
    reference_sample = 0U;
    assert(BatteryMonitor_ReadTracked(&handle, &result) == HAL_ERROR);
    assert(result.battery_mv == 3700U && channel == ADC_CHANNEL_14);
    poll_status = HAL_TIMEOUT;
    assert(BatteryMonitor_ReadTracked(&handle, &result) == HAL_TIMEOUT);
    assert(result.battery_mv == 3700U && channel == ADC_CHANNEL_14);
    puts("PASS: battery scaling, reference tracking, calibration and ADC failure cleanup");
    return 0;
}
