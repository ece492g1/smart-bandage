/*
 * stc3115.c
 *
 *  Created on: Mar 4, 2016
 *      Author: michaelblouin
 */

#include "stc3115.h"
#include "../i2c.h"
#include <ti/sysbios/BIOS.h>

inline void swapEndianness(uint16_t *value) {
	uint8_t temp = ((uint8_t*)value)[0];
	((uint8_t*)value)[0] = ((uint8_t*)value)[1];
	((uint8_t*)value)[1] = temp;
}

void stc3115_preTransfer(STC3115_DEVICE_HANDLE device, STC3115_ENDIANNESS desiredEndianness) {
//	if (desiredEndianness != device->endianness) {
//		swapEndianness(&device->counter);
//		swapEndianness(&device->current);
//		swapEndianness(&device->voltage);
//		swapEndianness(&device->ocv);
//		swapEndianness(&device->cc_cnf);
//		swapEndianness(&device->vm_cnf);
//		swapEndianness(&device->acc_cc_adj);
//		swapEndianness(&device->acc_vm_adj);
//
//		device->endianness = desiredEndianness;
//	}
//
//	memcpy(((uint8_t*)&device->ocv) -1, &device->ocv, (uint8_t)(((uint8_t*)&device->address) - ((uint8_t*)&device->ocv)) - 1);
}

void stc3115_postTransfer(STC3115_DEVICE_HANDLE device, STC3115_ENDIANNESS desiredEndianness) {
//	// Need to do some alignment fixing
//	memcpy(&device->ocv, ((uint8_t*)&device->ocv) -1, (uint8_t)(((uint8_t*)&device->address) - ((uint8_t*)&device->ocv))
//				- 1);
//	stc3115_preTransfer(device, desiredEndianness);
}

SB_Error stc3115_init(STC3115_DEVICE_HANDLE device, Semaphore_Handle *semaphore) {
	stc3115_endianness(device) = LITTLE_ENDIAN;
	return stc3115_readInfo(device, semaphore);
}

/*********************************************************************
 * @fn      stc3115_configure
 *
 * @brief   Configure an STC3115 device after the _init() call
 *
 * @param   rsense        - The value of the sense resistor in milliOhms
 * @param	battImpedance - The datasheet value for battery internal impedance.
 * @param   battCapacity  - The nominal battery capacity in mAh
 *
 * @return  NoError if properly written, otherwise the error
 */
SB_Error stc3115_configure(STC3115_DEVICE_HANDLE device, Semaphore_Handle *semaphore, uint16_t rsense, uint16_t battImpedance, uint16_t battCapacity) {
	SB_i2cTransaction transaction;
	I2C_Transaction baseTransaction;
	SB_Error result;

	stc3115_devAddrPointer(device) = STC3115_REG_MODE;

	stc3115_mode(device) |=
			  (STC3115_REG_MODE_VMODE_PSAVE << STC3115_REG_MODE_VMODE);

	stc3115_cc_cnf(device) = ((uint32_t)rsense * battCapacity)*STC3115_REG_CC_CNF_CONV_FACTOR;
	stc3115_vm_cnf(device) = ((uint32_t)battImpedance * battCapacity)*STC3115_REG_VM_CNF_CONV_FACTOR;

	stc3115_preTransfer(device, BIG_ENDIAN);

	// Write the new config
	baseTransaction.writeCount   = STC3115_WRITE_REG_COUNT(device);
	baseTransaction.writeBuf     = STC3115_WRITE_PTR(device);
	baseTransaction.readCount    = 0;
	baseTransaction.readBuf      = NULL;
	baseTransaction.slaveAddress = stc3115_address(device);

	transaction.baseTransaction = &baseTransaction;
	transaction.completionSemaphore = semaphore; // Set to NULL to tell the I2C stack we aren't waiting on completion

	if (NoError != (result = SB_i2cQueueTransaction(&transaction, BIOS_WAIT_FOREVER))) {
		return result;
	}

	Semaphore_pend(*semaphore, BIOS_WAIT_FOREVER);

//	stc3115_postTransfer(device, LITTLE_ENDIAN);

	return NoError;
}

SB_Error stc3115_readInfo(STC3115_DEVICE_HANDLE device, Semaphore_Handle *semaphore) {
	SB_i2cTransaction transaction;
	I2C_Transaction baseTransaction;
	SB_Error result;

	stc3115_devAddrPointer(device) = STC3115_REG_MODE;

	stc3115_preTransfer(device, BIG_ENDIAN);

	// First read all device state
	baseTransaction.writeCount   = 1;
	baseTransaction.writeBuf     = &stc3115_devAddrPointer(device);
	baseTransaction.readCount    = STC3115_READ_REG_COUNT(device);
	baseTransaction.readBuf      = STC3115_READ_PTR(device);
	baseTransaction.slaveAddress = stc3115_address(device);

	transaction.baseTransaction = &baseTransaction;
	transaction.completionSemaphore = semaphore; // Set to NULL to tell the I2C stack we aren't waiting on completion

	if (NoError != (result = SB_i2cQueueTransaction(&transaction, BIOS_WAIT_FOREVER))) {
		return result;
	}

	Semaphore_pend(*semaphore, BIOS_WAIT_FOREVER);

	stc3115_postTransfer(device, LITTLE_ENDIAN);

	return NoError;
}

inline uint16_t stc3115_deviceVoltage(STC3115_DEVICE_HANDLE device) {
	return stc3115_voltage(device) * 2.2 * 16;
}


inline uint16_t stc3115_deviceTemp(STC3115_DEVICE_HANDLE device) {
	return stc3115_temperature(device);
}
