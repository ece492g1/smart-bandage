/*
 * readingsManager.h
 *
 *  Created on: Mar 12, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_READINGSMANAGER_H_
#define APPLICATION_READINGSMANAGER_H_

#include "Board.h"

typedef struct {
	SB_READING_T  temperatures[SB_NUM_TEMPERATURE];
	SB_READING_T  humidities[SB_NUM_HUMIDITY];
	SB_READING_T  moistures[SB_NUM_MOISTURE];
	SB_TIMEDIFF_T timeDiff;
} SB_PeripheralReadings;

/*********************************************************************
 * @fn      SB_readingsManagerInit
 *
 * @brief   Initializes the readings manager
 */
SB_Error SB_readingsManagerInit();

/*********************************************************************
 * @fn      SB_newReadingsAvailable
 *
 * @brief   Called when new readings are available. May update bluetooth characteristics.
 */
SB_Error SB_newReadingsAvailable();

/*********************************************************************
 * @fn      SB_currentReadingsRead
 *
 * @brief   Called when the currently available readings have been read
 */
SB_Error SB_currentReadingsRead();

#endif /* APPLICATION_READINGSMANAGER_H_ */
