/*
 * tca9554a.c
 *
 *  Created on: Feb 27, 2016
 *      Author: michaelblouin
 */

#include "tca9554a.h"
#include "../i2c.h"
#include <ti/sysbios/BIOS.h>

SB_Error tca9554a_writePinStatus(TCA9554A_DEVICE *device, Semaphore_Handle *semaphore) {
	SB_i2cTransaction transaction;
	I2C_Transaction baseTransaction;
	uint8_t txBuf[2];
	SB_Error result;

	txBuf[0] = TCA9554A_REG_OUTPUT;
	txBuf[1] = device->outputReg;

	// The configuration transaction
	baseTransaction.writeCount   = 2;
	baseTransaction.writeBuf     = txBuf;
	baseTransaction.readCount    = 0;
	baseTransaction.readBuf      = NULL;
	baseTransaction.slaveAddress = device->address;

	transaction.baseTransaction = &baseTransaction;
	transaction.completionSemaphore = semaphore; // Set to NULL to tell the I2C stack we aren't waiting on completion

	if (NoError != (result = SB_i2cQueueTransaction(&transaction, BIOS_WAIT_FOREVER))) {
		return result;
	}

	Semaphore_pend(*semaphore, BIOS_WAIT_FOREVER);

	return NoError;
}

SB_Error tca9554a_setPinStatus(TCA9554A_DEVICE *device, Semaphore_Handle *semaphore, TCA9554A_IO_PORT pin, bool status) {
	// Set the status of the pin in the output status register
	device->outputReg = (device->outputReg & ~(_BV(pin))) | ((1 & status) << pin);

	// Write the state to the device
	return tca9554a_writePinStatus(device, semaphore);
}
