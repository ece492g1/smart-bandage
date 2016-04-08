/*
 * readingsManager.h
 *
 *  Created on: Mar 12, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_READINGSMANAGER_H_
#define APPLICATION_READINGSMANAGER_H_

#include "Board.h"

#define READINGS_MANAGER_THRESHOLD (SB_BLE_READINGS_LEN/sizeof(SB_PeripheralReadings))

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

/*********************************************************************
 * @fn      SB_sendNotificationIfSubscriptionChanged
 *
 * @brief   Send property changed notification if changed
 */
bool SB_sendNotificationIfSubscriptionChanged(bool forceTry);

/*********************************************************************
 * @fn      SB_setClearReadingsMode
 *
 * @brief   Allows all readings to be read, even if the last read does not have 3 readings.
 */
void SB_setClearReadingsMode(bool clearReadings);

/*********************************************************************
 * @fn      SB_setClearReadingsMode
 *
 * @brief   The reference time has been updated. Upudate the value in the characteristic.
 */
SB_Error SB_updateReadingsRefTimestamp();

#endif /* APPLICATION_READINGSMANAGER_H_ */
