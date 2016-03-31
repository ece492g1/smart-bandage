/*
 * readingsManager.c
 *
 *  Created on: Mar 12, 2016
 *      Author: michaelblouin
 */

#include <string.h>
#include <stddef.h>
#include "flash.h"
#include "readingsManager.h"
#include "../PROFILES/smartBandageProfile.h"
#include <xdc/runtime/System.h>

struct {
	uint8_t bleReadingsPopulated: 1;
	uint8_t clearReadingsMode:    1;
} RM;

/*********************************************************************
 * @fn      SB_readingsManagerInit
 *
 * @brief   Initializes the readings manager
 */
SB_Error SB_readingsManagerInit() {
	SB_PeripheralReadings* basePtr;
	RM.bleReadingsPopulated = false;
	RM.clearReadingsMode = false;

	if (SUCCESS != SB_Profile_Set16bParameter( SB_CHARACTERISTIC_READINGSIZE, sizeof(SB_PeripheralReadings), 0 )) {
		return BLECharacteristicWriteError;
	}

	// Update the reading count
	if (SUCCESS != SB_Profile_SetParameter( SB_CHARACTERISTIC_READINGCOUNT, sizeof(uint32_t), SB_flashReadingCountRef())) {
		return BLECharacteristicWriteError;
	}

	// Clear the readings buffer
	basePtr = (SB_PeripheralReadings*)
		SB_Profile_GetCharacteristicWritePTR(
			SB_CHARACTERISTIC_READINGS,
			SB_BLE_READINGS_LEN,
			0 );

	if (NULL == basePtr) {
		System_printf("PTR was zero.\n");
		return BLECharacteristicWriteError;
	}

	memset(basePtr, 0, SB_BLE_READINGS_LEN);

	// Set the offsets for the PTR struct
	uint8_t dataOffsets[SB_BLE_READINGDATAOFFSETS_LEN] = {
		offsetof(SB_PeripheralReadings, temperatures),
		offsetof(SB_PeripheralReadings, humidities),
		offsetof(SB_PeripheralReadings, moistures),
		offsetof(SB_PeripheralReadings, timeDiff),
	};

	if (SUCCESS != SB_Profile_SetParameter( SB_CHARACTERISTIC_READINGDATAOFFSETS, SB_BLE_READINGDATAOFFSETS_LEN, dataOffsets)) {
		System_printf("SB_CHARACTERISTIC_READINGDATAOFFSETS\n");
		return BLECharacteristicWriteError;
	}

	return NoError;
}

bool SB_sendNotificationIfSubscriptionChanged(bool forceTry) {
	if (forceTry || SB_Profile_NotificationStateChanged( SB_CHARACTERISTIC_READINGS )) {
		uint8_t status;
		if (0 != (status = SB_Profile_MarkParameterUpdated( SB_CHARACTERISTIC_READINGS ))) {
			System_printf("Failed to mark characteristic updated %d\n", status);

			return false;
		}

		return true;
	}

	return false;
}

/*********************************************************************
 * @fn      SB_newReadingsAvailable
 *
 * @brief   Called when new readings are available. May update bluetooth characteristics.
 */
SB_Error SB_newReadingsAvailable() {
	uint8_t i = 0, j = 0, status, numReadings;
	uint32_t *refTimestampPtr;
	SB_Error result;
	SB_PeripheralReadings* readingsPtr;

	// Update the reading count
	if (SUCCESS != SB_Profile_SetParameter( SB_CHARACTERISTIC_READINGCOUNT, sizeof(uint32_t), SB_flashReadingCountRef())) {
		return BLECharacteristicWriteError;
	}

	// If there are readings currently available don't do anything
	if (RM.bleReadingsPopulated) {
		SB_sendNotificationIfSubscriptionChanged(false);
		return NoError;
	}

	if (SB_flashReadingCount() < READINGS_MANAGER_THRESHOLD && !RM.clearReadingsMode || SB_flashReadingCount() == 0) {
		return NoError;
	}

	refTimestampPtr = (uint32_t*)
		SB_Profile_GetCharacteristicWritePTR(
			SB_CHARACTERISTIC_READINGS,
			SB_BLE_READINGS_LEN,
			0 );

	if (NULL == refTimestampPtr) {
		return BLECharacteristicWriteError;
	}

	*refTimestampPtr = SB_flashGetReferenceTime();
	readingsPtr = (SB_PeripheralReadings*)(&refTimestampPtr[1]);

	numReadings = READINGS_MANAGER_THRESHOLD;
	if (SB_flashReadingCount() < READINGS_MANAGER_THRESHOLD) {
		numReadings = SB_flashReadingCount();
		memset(readingsPtr, 0, SB_BLE_READINGS_LEN - SB_BLE_READINGREFTIMESTAMP_LEN);
	}

	for (i = 0; i < numReadings; ++i) {
		uint32_t thisRef;

		// Read from flash
		if (NoError != (result = SB_flashReadNext(&readingsPtr[i], &thisRef))) {
			return result;
		}

		if (thisRef == *refTimestampPtr || 0 == i) {
			// Leave them as is
		} else if (thisRef < *refTimestampPtr) {
			uint16_t diff = thisRef - *refTimestampPtr;
			// Set the refTimestamp to thisRef and adjust the other refs
			for (j = 0; j <= i; ++j) {
				if (readingsPtr[j].timeDiff <= diff) {
					readingsPtr[j].timeDiff = 1;
				} else {
					readingsPtr[j].timeDiff -= diff;
				}
			}
		} else {
			// thisRef > refTimestamp
			uint16_t diff = *refTimestampPtr - thisRef;
			for (j = i; j < numReadings; ++j) {
				readingsPtr[j].timeDiff += diff;
			}
		}

		// `0` is an invalid value for a timediff - an error of 1 second is fine.
		if (readingsPtr[i].timeDiff == 0) {
			readingsPtr[i].timeDiff = 1;
		}

		*refTimestampPtr = thisRef;
	}

	// Update the reference time
//	if (SUCCESS != SB_Profile_SetParameterPartial( SB_CHARACTERISTIC_READINGREFTIMESTAMP, sizeof(uint32_t), 0, &refTimestamp)) {
//		return BLECharacteristicWriteError;
//	}
	System_printf("Set ref timestamp: %u\n", *refTimestampPtr);

	// TODO: Send change notification
	RM.bleReadingsPopulated = true;
	if (0 != (status = SB_Profile_MarkParameterUpdated( SB_CHARACTERISTIC_READINGS ))) {
		System_printf("Failed to mark characteristic updated %d\n", status);
	}

	return NoError;
}

void SB_setClearReadingsMode(bool clearReadings) {
	RM.clearReadingsMode = clearReadings & 1;
}

/*********************************************************************
 * @fn      SB_currentReadingsRead
 *
 * @brief   Called when the currently available readings have been read
 */
SB_Error SB_currentReadingsRead() {
	SB_PeripheralReadings* basePtr;

	RM.bleReadingsPopulated = false;

	System_printf("BLE Readings Read. %d readings remaining.\n", SB_flashReadingCount());
	System_flush();

	if (SB_flashReadingCount() >= READINGS_MANAGER_THRESHOLD || RM.clearReadingsMode) {
		// If there are more readings available, place the next once in the characteristic buffer
		return SB_newReadingsAvailable();
	}

	// Update the reading count
	else if (SUCCESS != SB_Profile_SetParameter( SB_CHARACTERISTIC_READINGCOUNT, sizeof(uint32_t), SB_flashReadingCountRef())) {
		return BLECharacteristicWriteError;
	}

	// Clear the readings buffer
	basePtr = (SB_PeripheralReadings*)
		SB_Profile_GetCharacteristicWritePTR(
			SB_CHARACTERISTIC_READINGS,
			SB_BLE_READINGS_LEN,
			0 );

	if (NULL == basePtr) {
		return BLECharacteristicWriteError;
	}

	memset(basePtr, 0, SB_BLE_READINGS_LEN);

	return NoError;
}
