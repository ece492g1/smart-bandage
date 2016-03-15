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

#define READINGS_MANAGER_THRESHOLD (SB_BLE_READINGS_LEN/sizeof(SB_PeripheralReadings))

struct {
	uint8_t bleReadingsPopulated: 1;
} RM;

/*********************************************************************
 * @fn      SB_readingsManagerInit
 *
 * @brief   Initializes the readings manager
 */
SB_Error SB_readingsManagerInit() {
	SB_PeripheralReadings* basePtr;
	RM.bleReadingsPopulated = false;

	if (SUCCESS != SB_Profile_Set16bParameter( SB_CHARACTERISTIC_READINGSIZE, sizeof(SB_PeripheralReadings), 0 )) {
		System_printf("SB_CHARACTERISTIC_READINGSIZE\n");
		return BLECharacteristicWriteError;
	}

	// Update the reading count
	if (SUCCESS != SB_Profile_SetParameter( SB_CHARACTERISTIC_READINGCOUNT, sizeof(uint32_t), SB_flashReadingCountRef())) {
		System_printf("SB_CHARACTERISTIC_READINGCOUNT\n");
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

/*********************************************************************
 * @fn      SB_newReadingsAvailable
 *
 * @brief   Called when new readings are available. May update bluetooth characteristics.
 */
SB_Error SB_newReadingsAvailable() {
	uint8_t i = 0, j = 0, status;
	uint32_t refTimestamp = 0;
	SB_Error result;
	SB_PeripheralReadings* basePtr;

	// Update the reading count
	if (SUCCESS != SB_Profile_SetParameter( SB_CHARACTERISTIC_READINGCOUNT, sizeof(uint32_t), SB_flashReadingCountRef())) {
		return BLECharacteristicWriteError;
	}

	// If there are readings currently available don't do anything
	if (RM.bleReadingsPopulated) {

		if (SB_Profile_NotificationStateChanged( SB_CHARACTERISTIC_READINGS )) {
			System_printf("Sending readings update...\n");
			if (0 != (status = SB_Profile_MarkParameterUpdated( SB_CHARACTERISTIC_READINGS ))) {
				System_printf("Failed to mark characteristic updated %d\n", status);
			}
		}

		return NoError;
	}

	if (SB_flashReadingCount() < READINGS_MANAGER_THRESHOLD) {
		return NoError;
	}

	basePtr = (SB_PeripheralReadings*)
		SB_Profile_GetCharacteristicWritePTR(
			SB_CHARACTERISTIC_READINGS,
			SB_BLE_READINGS_LEN,
			0 );

	if (NULL == basePtr) {
		return BLECharacteristicWriteError;
	}

	for (i = 0; i < READINGS_MANAGER_THRESHOLD; ++i) {
		uint32_t thisRef;

		// Read from flash
		if (NoError != (result = SB_flashReadNext((uint8_t*) &basePtr[i], &thisRef))) {
			return result;
		}

		if (thisRef == refTimestamp || 0 == i) {
			// Leave them as is
		} else if (thisRef < refTimestamp) {
			uint16_t diff = thisRef - refTimestamp;
			// Set the refTimestamp to thisRef and adjust the other refs
			for (j = 0; j <= i; ++j) {
				if (basePtr[j].timeDiff <= diff) {
					basePtr[j].timeDiff = 1;
				} else {
					basePtr[j].timeDiff -= diff;
				}
			}
		} else {
			// thisRef > refTimestamp
			uint16_t diff = refTimestamp - thisRef;
			for (j = i; j < READINGS_MANAGER_THRESHOLD; ++j) {
				basePtr[j].timeDiff += diff;
			}
		}

		// `0` is an invalid value for a timediff - an error of 1 second is fine.
		if (basePtr[i].timeDiff == 0) {
			basePtr[i].timeDiff = 1;
		}

		refTimestamp = thisRef;
	}

	// Update the reference time
	if (SUCCESS != SB_Profile_SetParameterPartial( SB_CHARACTERISTIC_READINGREFTIMESTAMP, sizeof(uint32_t), 0, &refTimestamp)) {
		return BLECharacteristicWriteError;
	}

	// TODO: Send change notification
	RM.bleReadingsPopulated = true;
	System_printf("Sending readings update...\n");
	if (0 != (status = SB_Profile_MarkParameterUpdated( SB_CHARACTERISTIC_READINGS ))) {
		System_printf("Failed to mark characteristic updated %d\n", status);
	}

	return NoError;
}

/*********************************************************************
 * @fn      SB_currentReadingsRead
 *
 * @brief   Called when the currently available readings have been read
 */
SB_Error SB_currentReadingsRead() {
	SB_PeripheralReadings* basePtr;

	RM.bleReadingsPopulated = false;

	if (SB_flashReadingCount() >= READINGS_MANAGER_THRESHOLD) {
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
