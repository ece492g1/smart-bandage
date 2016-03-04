/*
 * hdc1050.c
 *
 *  Created on: Feb 27, 2016
 *      Author: michaelblouin
 */

#include "hdc1050.h"
#include "../i2c.h"
#include <ti/sysbios/BIOS.h>

SB_Error hdc1050_startTempHumidityConversion(HDC1050_DEVICE *device, Semaphore_Handle *semaphore) {
	SB_i2cTransaction transaction;
	I2C_Transaction baseTransaction;
	uint8_t txBuf[1];
	SB_Error result;

	txBuf[0] = HDC1050_REG_TEMPERATURE;

	// The configuration transaction
	baseTransaction.writeCount   = 1;
	baseTransaction.writeBuf     = txBuf;
	baseTransaction.readCount    = 0;
	baseTransaction.readBuf      = NULL;
	baseTransaction.slaveAddress = device->address;

	transaction.baseTransaction = &baseTransaction;
	transaction.completionSemaphore = semaphore;

	result = SB_i2cQueueTransaction(&transaction, BIOS_WAIT_FOREVER);
	if (NoError != result) {
		return result;
	}

	Semaphore_pend(*semaphore, BIOS_WAIT_FOREVER);

	return transaction.completionResult;
}

SB_Error hdc1050_readTempHumidity(HDC1050_DEVICE *device, Semaphore_Handle *semaphore) {
	SB_i2cTransaction transaction;
	I2C_Transaction baseTransaction;
	uint8_t rxBuf[4];
	SB_Error result;

	// The configuration transaction
	baseTransaction.writeCount   = 0;
	baseTransaction.writeBuf     = NULL;
	baseTransaction.readCount    = 4;
	baseTransaction.readBuf      = rxBuf;
	baseTransaction.slaveAddress = device->address;

	transaction.baseTransaction = &baseTransaction;
	transaction.completionSemaphore = semaphore;

	result = SB_i2cQueueTransaction(&transaction, BIOS_WAIT_FOREVER);
	if (NoError != result) {
		return result;
	}

	Semaphore_pend(*semaphore, BIOS_WAIT_FOREVER);

	if (transaction.completionResult == NoError) {
		// Temperature = VALUE/2^16 * 165 - 40
		device->temperature = (((rxBuf[0] << 8) | (rxBuf[1])) * 165)*16/(1<<16) - (40 * 16);

		// Humidity (%RH) = VALUE/2^16 * 100
		device->humidity = ((rxBuf[2] << 8) | (rxBuf[3]))*100*16/(1<<16);
	}

	return transaction.completionResult;
}
