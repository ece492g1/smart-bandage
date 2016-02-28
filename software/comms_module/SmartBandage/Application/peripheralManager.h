/*
 * peripheralManager.h
 *
 *  Created on: Feb 11, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_PERIPHERALMANAGER_H_
#define APPLICATION_PERIPHERALMANAGER_H_

#include "Board.h"
#include "Devices/tca9554a.h"

#define IOEXP_I2CSTATUS_PIN_TEMP0 IOPORT2
#define IOEXP_I2CSTATUS_PIN_HUMIDITY (TCA9554A_IO_PORT)(IOEXP_I2CSTATUS_PIN_TEMP0 + (TCA9554A_IO_PORT)SB_NUM_MCP9808_SENSORS)

#if IOEXP_I2CSTATIS_PIN_HUMIDITY > 7
#error "Too many MCP9808 sensor for debug LEDs"
#endif

typedef enum {
	PState_Unknown,
	PState_OK,
	PState_Intermittent,
	PState_FailedConfig,
	PState_Failed,
} SB_PeripheralFunctionalState;

typedef struct {
	SB_Error lastError;
	SB_PeripheralFunctionalState currentState;
} SB_PeripheralState;

typedef struct {
	MUX_OUTPUT pwrmuxOutput;
	MUX_OUTPUT_ENABLE pwrmuxOutputEnable;
	MUX_OUTPUT iomuxOutput;
} SB_MUXState;

SB_Error SB_peripheralInit();
SB_Error SB_setPeripheralsEnable(bool enable);
SB_Error SB_sysDisableRefresh(uint32 semaphoreTimeout);
SB_Error SB_sysDisableShutdown();

#endif /* APPLICATION_PERIPHERALMANAGER_H_ */
