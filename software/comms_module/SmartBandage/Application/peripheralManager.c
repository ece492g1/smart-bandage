/*
 * peripheralManager.c
 *
 *  Created on: Feb 11, 2016
 *      Author: michaelblouin
 */

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/System.h>


#include "i2c.h"
#include "config.h"
#include "Devices/mcp9808.h"
#include "peripheralManager.h"

struct {
	MCP9808_DEVICE mcp9808Devices[SB_NUM_MCP9808_SENSORS];
	SB_PeripheralState mcp9808DeviceStates[SB_NUM_MCP9808_SENSORS];
	Semaphore_Handle mcp9808DeviceSemaphores[SB_NUM_MCP9808_SENSORS];

	Task_Handle taskHandle;
	Task_Struct task;
	Char taskStack[PMGR_TASK_STACK_SIZE];
} PMGR;

SB_Error applyTempSensorConfiguration(uint8_t deviceNo) {
	SB_i2cTransaction configTransaction, resolutionTransaction;
	I2C_Transaction configBaseTransaction, resolutionBaseTransaction;
	uint8_t txBuf[5];

	PMGR.mcp9808Devices[deviceNo].Configuration =
		  MCP9808_ALERT_COMPARATOR   << MCP9808_CONFIG_ALERT_MODE
		| MCP9808_OUTPUT_ACTIVE_HIGH << MCP9808_CONFIG_ALERT_POLARITY
		| MCP9808_ALERT_ALL_SOURCES  << MCP9808_CONFIG_ALERT_SELECT
	;

	PMGR.mcp9808Devices[deviceNo].Resolution = MCP9808_RESOLUTION_0P0625;

	txBuf[0] = MCP9808_REG_CONFIG;
	txBuf[1] = 0xFF & (PMGR.mcp9808Devices[deviceNo].Configuration >> 8);
	txBuf[2] = 0xFF & (PMGR.mcp9808Devices[deviceNo].Configuration >> 0);

	txBuf[3] = MCP9808_REG_RESOLUTION;
	txBuf[4] = PMGR.mcp9808Devices[deviceNo].Resolution;

	// The configuration transaction
	configBaseTransaction.writeCount   = 3;
	configBaseTransaction.writeBuf     = txBuf;
	configBaseTransaction.readCount    = 0;
	configBaseTransaction.readBuf      = NULL;
	configBaseTransaction.slaveAddress = PMGR.mcp9808Devices[deviceNo].Address;

	configTransaction.baseTransaction = &configBaseTransaction;
	configTransaction.completionSemaphore = &PMGR.mcp9808DeviceSemaphores[deviceNo];

	// The resolution transaction
	resolutionBaseTransaction.writeCount   = 2;
	resolutionBaseTransaction.writeBuf     = &txBuf[3];
	resolutionBaseTransaction.readCount    = 0;
	resolutionBaseTransaction.readBuf      = NULL;
	resolutionBaseTransaction.slaveAddress = PMGR.mcp9808Devices[deviceNo].Address;

	resolutionTransaction.baseTransaction = &resolutionBaseTransaction;
	resolutionTransaction.completionSemaphore = &PMGR.mcp9808DeviceSemaphores[deviceNo];

	// Queue the configuration and resolution transactions
	SB_i2cQueueTransaction(&configTransaction, BIOS_WAIT_FOREVER);
	SB_i2cQueueTransaction(&resolutionTransaction, BIOS_WAIT_FOREVER);

	// Wait for completion (twice)
	Semaphore_pend(PMGR.mcp9808DeviceSemaphores[deviceNo], BIOS_WAIT_FOREVER);
	Semaphore_pend(PMGR.mcp9808DeviceSemaphores[deviceNo], BIOS_WAIT_FOREVER);

	return configTransaction.completionResult;
}

SB_Error initPeripherals() {
	int i;

	// Initialize MCP9808 sensors
	for (i = 0; i < SB_NUM_MCP9808_SENSORS; ++i) {
		PMGR.mcp9808Devices[i].Address = Mcp9808Addresses[i];
		PMGR.mcp9808DeviceStates[i].lastError = applyTempSensorConfiguration(i);

		if (NoError == PMGR.mcp9808DeviceStates[i].lastError) {
			PMGR.mcp9808DeviceStates[i].currentState = PState_OK;
		} else {
			PMGR.mcp9808DeviceStates[i].currentState = PState_FailedConfig;
			return PMGR.mcp9808DeviceStates[i].lastError;
		}
	}

	return NoError;
}

static void SB_peripheralManagerTask(UArg a0, UArg a1) {
	SB_Error result;

#ifdef SB_DEBUG
		System_printf("Peripheral manager task started...\n");
		System_flush();
#endif

	if (NoError != (result = initPeripherals())) {
#ifdef SB_DEBUG
		System_printf("Peripheral initialization failure: %d. Peripheral Manager stalled.\n", result);
		System_flush();
#endif
		Task_exit();
	}

#ifdef SB_DEBUG
		System_printf("PMGR: Peripherals initialized.\n", result);
		System_flush();
#endif

	SB_i2cTransaction taTransaction;
	I2C_Transaction taBaseTransaction;
	uint8_t txBuf[1];
	uint8_t rxBuf[2];

	txBuf[0] = MCP9808_REG_TA;

	// The configuration transaction
	taBaseTransaction.writeCount   = 1;
	taBaseTransaction.writeBuf     = txBuf;
	taBaseTransaction.readCount    = 2;
	taBaseTransaction.readBuf      = rxBuf;
	taBaseTransaction.slaveAddress = PMGR.mcp9808Devices[0].Address;

	taTransaction.baseTransaction = &taBaseTransaction;
	taTransaction.completionSemaphore = &PMGR.mcp9808DeviceSemaphores[0];

	while (1) {
		// Queue the configuration and resolution transactions
		SB_i2cQueueTransaction(&taTransaction, BIOS_WAIT_FOREVER);

		// Wait for completion (twice)
		Semaphore_pend(PMGR.mcp9808DeviceSemaphores[0], BIOS_WAIT_FOREVER);

		if (taTransaction.completionResult == NoError) {
			System_printf("PMGR: Temperature read: %d\n", (0x0FFF & ((rxBuf[0] << 8) | (rxBuf[1] & 0x0FF)))>>4);
		} else {
			System_printf("PMGR: Temperature read failed.\n");
		}

		System_flush();
		Task_sleep(100000);
	}
}

SB_Error SB_peripheralInit() {
	int i;

	for (i = 0; i < SB_NUM_MCP9808_SENSORS; ++i) {
#ifdef SB_DEBUG
		System_printf("PMGR: Initializing data structures for MCP9808 Device %d\n", i);
		System_flush();
#endif
		PMGR.mcp9808DeviceSemaphores[i] = Semaphore_create(0, NULL, NULL);
		if (NULL == PMGR.mcp9808DeviceSemaphores[i]) {
			return OSResourceInitializationError;
		}
	}

	Task_Params taskParams;

	Task_Params_init(&taskParams);
	taskParams.stack = PMGR.taskStack;
	taskParams.stackSize = PMGR_TASK_STACK_SIZE;
	taskParams.priority = PMGR_TASK_PRIORITY;

	PMGR.taskHandle = Task_create(SB_peripheralManagerTask, &taskParams, NULL);

	if (PMGR.taskHandle == NULL) {
		System_printf("PMGR: Task creation failed\n");
		System_flush();

		return OSResourceInitializationError;
	}

	return NoError;
}
