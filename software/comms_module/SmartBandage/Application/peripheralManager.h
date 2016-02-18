/*
 * peripheralManager.h
 *
 *  Created on: Feb 11, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_PERIPHERALMANAGER_H_
#define APPLICATION_PERIPHERALMANAGER_H_

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

SB_Error SB_peripheralInit();

#endif /* APPLICATION_PERIPHERALMANAGER_H_ */
