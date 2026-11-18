/*
 * battery_monitor.h
 *
 *
 *      Author: Jonathan
 */

#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include "main.h"

typedef struct
{
    uint16_t raw;        /* Average ADC reading: 0–4095 */
    uint32_t pin_mv;     /* PA7 voltage in millivolts */
    uint32_t battery_mv; /* Battery voltage in millivolts */
} BatteryReading;

/* Calibrate once, after MX_ADC1_Init(). */
HAL_StatusTypeDef BatteryMonitor_Init(ADC_HandleTypeDef *adc);

/* Take eight readings and calculate their average.
   reference_mv is the ADC reference voltage in millivolts.
   On failure, output retains its previous value. */
HAL_StatusTypeDef BatteryMonitor_Read(
    ADC_HandleTypeDef *adc,
    uint32_t reference_mv,
    BatteryReading *output
);
HAL_StatusTypeDef BatteryMonitor_ReadTracked(
    ADC_HandleTypeDef *adc,
    BatteryReading *output
);
#endif
