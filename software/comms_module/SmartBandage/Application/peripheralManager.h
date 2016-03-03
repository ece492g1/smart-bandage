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

#define PERIPHERAL_MAX_READ_ATTEMPTS 3

#define HDC1050_READ_WAIT_TICKS ((uint16)(HDC1050_CONV_TIME_HRES_14BIT + HDC1050_CONV_TIME_TRES_14BIT)) * NTICKS_PER_MILLSECOND + (1 * NTICKS_PER_MILLSECOND)

#define IOEXP_I2CSTATUS_PIN_TEMP0 IOPORT2
#define IOEXP_I2CSTATUS_PIN_HUMIDITY (TCA9554A_IO_PORT)(IOEXP_I2CSTATUS_PIN_TEMP0 + (TCA9554A_IO_PORT)SB_NUM_MCP9808_SENSORS)
#define IOEXP_I2CSTATUS_PIN_TEMP(index) (TCA9554A_IO_PORT)(IOEXP_I2CSTATUS_PIN_TEMP0 + (TCA9554A_IO_PORT)(index % SB_NUM_MCP9808_SENSORS))

#ifdef SB_DEBUG
# define PMANAGER_TASK_YIELD_HIGHERPRI() Task_sleep(100)
#else
# define PMANAGER_TASK_YIELD_HIGHERPRI() Task_sleep(1)
#endif

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
	uint8_t numReadAttempts;
} SB_PeripheralState;

typedef struct {
	MUX_OUTPUT pwrmuxOutput;
	MUX_OUTPUT_ENABLE pwrmuxOutputEnable;
	MUX_OUTPUT iomuxOutput;
} SB_MUXState;

typedef struct {
	SB_READING_T  temperatures[SB_NUM_TEMPERATURE];
	SB_READING_T  humidities[SB_NUM_HUMIDITY];
	SB_READING_T  moistures[SB_NUM_MOISTURE];
	SB_TIMEDIFF_T timeDiff;
} SB_PeripheralReadings;

SB_Error SB_peripheralInit();
SB_Error SB_setPeripheralsEnable(bool enable);
SB_Error SB_sysDisableRefresh(uint32 semaphoreTimeout);
SB_Error SB_sysDisableShutdown();

#endif /* APPLICATION_PERIPHERALMANAGER_H_ */
