/*
 * headphone_controls.h
 *
 *
 *      Author: Jonathan
 */


#ifndef HEADPHONE_CONTROLS_H
#define HEADPHONE_CONTROLS_H

#include "main.h"
#include <stdbool.h>

/* Call once after GPIO and I2C initialization. */
HAL_StatusTypeDef HeadphoneControls_Init(I2C_HandleTypeDef *bus);

/* Call repeatedly from the main loop. */
HAL_StatusTypeDef HeadphoneControls_Process(void);

/* Volume codes range from 0 to 63. */
HAL_StatusTypeDef HeadphoneControls_SetVolume(uint8_t gain_code);
uint8_t HeadphoneControls_GetVolume(void);

HAL_StatusTypeDef HeadphoneControls_SetMute(bool muted);

/* Reports a button press; BM83 UART handling must act on it. */
bool HeadphoneControls_TakePlayPause(void);

#endif

 /* HEADPHONE_CONTROLS_H_ */
