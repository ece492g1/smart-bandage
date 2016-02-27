/*
 * peripheralManager.c
 *
 *  Created on: Feb 11, 2016
 *      Author: michaelblouin
 */

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/System.h>
#include <ti/drivers/PIN.h>

#include "i2c.h"
#include "util.h"
#include "Devices/mcp9808.h"
#include "Devices/hdc1050.h"
#include "Devices/tca9554a.h"
#include "peripheralManager.h"
#include "../PROFILES/smartBandageProfile.h"

SB_Error applyTempSensorConfiguration(uint8_t deviceNo);
SB_Error applyIOMuxState(MUX_OUTPUT_ENABLE outputEnable, MUX_OUTPUT output);
SB_Error applyPWRMuxState(MUX_OUTPUT_ENABLE outputEnable, MUX_OUTPUT output);
SB_Error applyFullMuxState(SB_MUXState muxState, uint32 timeout);
SB_Error _applyFullMuxState(SB_MUXState muxState);
void     SB_sysdisblClockHandler(UArg arg);

struct {
	MCP9808_DEVICE mcp9808Devices[SB_NUM_MCP9808_SENSORS];
	SB_PeripheralState mcp9808DeviceStates[SB_NUM_MCP9808_SENSORS];
	Semaphore_Handle mcp9808DeviceSemaphores[SB_NUM_MCP9808_SENSORS];

	HDC1050_DEVICE hdc1050Device;
	SB_PeripheralState hdc1050DeviceState;
	Semaphore_Handle hdc1050DeviceSemaphore;

#ifdef IOEXPANDER_PRESENT
	TCA9554A_DEVICE ioexpanderDevice;
	SB_PeripheralState ioexpanderDeviceState;
	Semaphore_Handle ioexpanderDeviceSemaphore;
#endif

	Task_Handle taskHandle;
	Task_Struct task;
	Char taskStack[PMGR_TASK_STACK_SIZE];

	PIN_State PeripheralPower;
	PIN_State MUXPins;
	PIN_State AnalogPins;
	Semaphore_Handle muxSemaphore;
	Clock_Struct sysdisblClock;
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

SB_Error applyHumiditySensorConfiguration() {
	SB_i2cTransaction configTransaction;
	I2C_Transaction configBaseTransaction;
	uint8_t txBuf[3];

	PMGR.hdc1050Device.configuration =
		  HDC1050_REG_CONFIGURATION_HEAT_DSBL       << HDC1050_REG_CONFIGURATION_HEAT
		| HDC1050_REG_CONFIGURATION_MODE_SEQUENTIAL << HDC1050_REG_CONFIGURATION_MODE
		| HDC1050_REG_CONFIGURATION_TRES_14BIT 		<< HDC1050_REG_CONFIGURATION_TRES
		| HDC1050_REG_CONFIGURATION_HRES_14BIT 		<< HDC1050_REG_CONFIGURATION_HRES
	;

	txBuf[0] = HDC1050_REG_CONFIGURATION;
	txBuf[1] = 0xFF & (PMGR.hdc1050Device.configuration >> 8);
	txBuf[2] = 0xFF & (PMGR.hdc1050Device.configuration >> 0);

	// The configuration transaction
	configBaseTransaction.writeCount   = 3;
	configBaseTransaction.writeBuf     = txBuf;
	configBaseTransaction.readCount    = 0;
	configBaseTransaction.readBuf      = NULL;
	configBaseTransaction.slaveAddress = PMGR.hdc1050Device.address;

	configTransaction.baseTransaction = &configBaseTransaction;
	configTransaction.completionSemaphore = &PMGR.hdc1050DeviceSemaphore;

	// Queue the configuration and resolution transactions
	SB_i2cQueueTransaction(&configTransaction, BIOS_WAIT_FOREVER);

	// Wait for completion
	Semaphore_pend(PMGR.hdc1050DeviceSemaphore, BIOS_WAIT_FOREVER);

	return configTransaction.completionResult;
}

#ifdef IOEXPANDER_PRESENT
SB_Error applyIOExpanderConfiguration() {
	SB_i2cTransaction configTransaction;
	I2C_Transaction configBaseTransaction;
	uint8_t txBuf[2];

	PMGR.ioexpanderDevice.configuration =
			  TCA9554A_CONFIG_INPUT  << IOPORT0
			| TCA9554A_CONFIG_INPUT  << IOPORT1
			| TCA9554A_CONFIG_OUTPUT << IOPORT2
			| TCA9554A_CONFIG_OUTPUT << IOPORT3
			| TCA9554A_CONFIG_OUTPUT << IOPORT4
			| TCA9554A_CONFIG_OUTPUT << IOPORT5
			| TCA9554A_CONFIG_OUTPUT << IOPORT6
			| TCA9554A_CONFIG_OUTPUT << IOPORT7
	;

	txBuf[0] = TCA9554A_REG_CONFIG;
	txBuf[1] = PMGR.ioexpanderDevice.configuration;

	// The configuration transaction
	configBaseTransaction.writeCount   = 2;
	configBaseTransaction.writeBuf     = txBuf;
	configBaseTransaction.readCount    = 0;
	configBaseTransaction.readBuf      = NULL;
	configBaseTransaction.slaveAddress = PMGR.ioexpanderDevice.address;

	configTransaction.baseTransaction = &configBaseTransaction;
	configTransaction.completionSemaphore = &PMGR.ioexpanderDeviceSemaphore;

	// Queue the configuration and resolution transactions
	SB_i2cQueueTransaction(&configTransaction, BIOS_WAIT_FOREVER);

	// Wait for completion
	Semaphore_pend(PMGR.ioexpanderDeviceSemaphore, BIOS_WAIT_FOREVER);

	return configTransaction.completionResult;
}
#endif

SB_Error initPeripherals() {
	int i;

	// Initialize MCP9808 sensors
	for (i = 0; i < SB_NUM_MCP9808_SENSORS; ++i) {
		PMGR.mcp9808Devices[i].Address = Mcp9808Addresses[i];
		PMGR.mcp9808DeviceStates[i].lastError = applyTempSensorConfiguration(i);

		if (NoError == PMGR.mcp9808DeviceStates[i].lastError) {
			PMGR.mcp9808DeviceStates[i].currentState = PState_OK;
		} else {
#ifdef SB_DEBUG
		System_printf("MCP9808 device %d init failed...\n", i);
		System_flush();
#endif
			PMGR.mcp9808DeviceStates[i].currentState = PState_FailedConfig;
			return PMGR.mcp9808DeviceStates[i].lastError;
		}
	}

#ifdef IOEXPANDER_PRESENT
	// Initialize IO Expander
	PMGR.ioexpanderDevice.address = I2C_DBGIOEXP_ADDR;
	PMGR.ioexpanderDeviceState.lastError = applyIOExpanderConfiguration();

	if (NoError == PMGR.ioexpanderDeviceState.lastError) {
		PMGR.ioexpanderDeviceState.currentState = PState_OK;
	} else {
#ifdef SB_DEBUG
		System_printf("IO Expander init failed...\n");
		System_flush();
#endif
		PMGR.ioexpanderDeviceState.currentState = PState_FailedConfig;
		return PMGR.ioexpanderDeviceState.lastError;
	}
#endif

	return NoError;
}

static void SB_peripheralManagerTask(UArg a0, UArg a1) {
	SB_Error result;

#ifdef LAUNCHPAD
	PIN_Config pinConfigTable[] =
	{
		Board_T_LED_GREEN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		PIN_TERMINATE,
	};
#endif LAUNCHPAD

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
#ifdef LAUNCHPAD
	PIN_State sbpPins;
	PIN_Handle statusPin = PIN_open(&sbpPins, pinConfigTable);
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

	uint16_t temperature;
	while (1) {

		// Enable peripherals
		SB_setPeripheralsEnable(true);

#ifdef LAUNCHPAD
		PIN_setOutputValue(statusPin, Board_T_LED_GREEN, 1);
#endif
		// Queue the configuration and resolution transactions
		SB_i2cQueueTransaction(&taTransaction, BIOS_WAIT_FOREVER);

		// Wait for completion (twice)
		Semaphore_pend(PMGR.mcp9808DeviceSemaphores[0], BIOS_WAIT_FOREVER);
#ifdef LAUNCHPAD
		PIN_setOutputValue(statusPin, Board_T_LED_GREEN, 0);
#endif

		if (taTransaction.completionResult == NoError) {
			// The temperature sensor is big endian and this device is little endian
			// Also need to apply the mask for the data from the sensor: 0x0FFF
			temperature = 0x0FFF & ((rxBuf[0] << 8) | (rxBuf[1]));
#ifdef SB_DEBUG
			System_printf("PMGR: Temperature read: %d\n", temperature>>4);
#endif

			// TODO: Calls like this should likely be protected with a semaphore
			SB_Profile_Set16bParameter( SB_CHARACTERISTIC_TEMPERATURE, temperature, 1 );
		} else {
#ifdef SB_DEBUG
			System_printf("PMGR: Temperature read failed.\n");
#endif
		}

		// Disable peripherals
		SB_setPeripheralsEnable(false);

#ifdef SB_DEBUG
		System_flush();
#endif

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
			System_printf("PMGR: Initializing data structures for MCP9808 Device %d failed!\n", i);
			System_flush();
			return OSResourceInitializationError;
		}
	}

#ifdef IOEXPANDER_PRESENT
	PMGR.ioexpanderDeviceSemaphore = Semaphore_create(0, NULL, NULL);
	if (NULL == PMGR.ioexpanderDeviceSemaphore) {
		System_printf("PMGR: Initializing data structures for io expander failed.\n");
		System_flush();
		return OSResourceInitializationError;
	}
#endif

	// Initialize power pin
	PIN_Config peripheralPowerConfigTable[] =
	{
		Board_PERIPHERAL_PWR,
		PIN_TERMINATE,
	};

	if (!PIN_open(&PMGR.PeripheralPower, peripheralPowerConfigTable)) {
#ifdef SB_DEBUG
	System_printf("Failed to initialize power pin...\n");
	System_flush();
#endif
		return OSResourceInitializationError;
	}

	// Initialize MUX pins
	PIN_Config muxPinsConfigTable[] =
	{
		Board_MP_EN_SW,
		Board_MSW_0,
		Board_MSW_1,
		Board_MSW_2,
		Board_MPSW,
		PIN_TERMINATE,
	};

	if (!PIN_open(&PMGR.MUXPins, muxPinsConfigTable)) {
#ifdef SB_DEBUG
	System_printf("Failed to initialize MUX pins...\n");
	System_flush();
#endif
		return OSResourceInitializationError;
	}

	// Initialize analog input pins
	PIN_Config analogPinsConfigTable[] =
	{
		Board_BANDAGE_A_0,
		Board_CONN_STATE_RD,
		Board_VSENSE_0,
		Board_VSENSE_1,
		Board_1V3,
		PIN_TERMINATE,
	};

	if (!PIN_open(&PMGR.AnalogPins, analogPinsConfigTable)) {
#ifdef SB_DEBUG
	System_printf("Failed to initialize analog pins...\n");
	System_flush();
#endif
		return OSResourceInitializationError;
	}

	// Initialize MUX semaphore with 1 free resource (use as mutex)
	PMGR.muxSemaphore = Semaphore_create(1, NULL, NULL);

	// Initialize sysdisbl clock
	if (NULL == Util_constructClock(
			&PMGR.sysdisblClock,
			SB_sysdisblClockHandler,
			SYSDSBL_REFRESH_CLOCK_PERIOD,
			CLOCK_ONESHOT,
			false,
			NULL)) {

#ifdef SB_DEBUG
		System_printf("Failed to initialize sysdisbl clock...\n");
		System_flush();
#endif
		return OSResourceInitializationError;
	}

	// Initialize peripheral manager task
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

/**
 * \brief Enables or disables power to external PCB peripherals
 */
SB_Error SB_setPeripheralsEnable(bool enable) {
	PIN_Status result = PIN_setOutputValue(&PMGR.PeripheralPower, Board_PERIPHERAL_PWR, enable != false);
	if (result == PIN_SUCCESS) {
		return NoError;
	}

#ifdef SB_DEBUG
	System_printf("Received error setting peripheral power: %d\n", result);
	System_flush();
#endif

	return UnknownError;
}

/**
 * \brief Applies the mux states to the PWR and IO muxes after pending on the MUX semaphore.
 */
SB_Error applyFullMuxState(SB_MUXState muxState, uint32 timeout) {
	if (!Semaphore_pend(PMGR.muxSemaphore, timeout)) {
		return SemaphorePendTimeout;
	}

	SB_Error result = _applyFullMuxState(muxState);

	Semaphore_post(PMGR.muxSemaphore);

	return result;
}

/**
 * \brief Applies the mux states to the PWR and IO muxes without pending on the MUX semaphore.
 * \remark You must posess a lock on the PMGR.muxSemaphore
 */
SB_Error _applyFullMuxState(SB_MUXState muxState) {
	PIN_Status result =
			PIN_setPortOutputValue(&PMGR.MUXPins,
				// Set the MUX Select S* outputs
				  (MUX_SELECT_VALUE(S0, muxState.iomuxOutput) << Board_IOMUX_S0)
				| (MUX_SELECT_VALUE(S1, muxState.iomuxOutput) << Board_IOMUX_S1)
				| (MUX_SELECT_VALUE(S2, muxState.iomuxOutput) << Board_IOMUX_S2)

				| (MUX_SELECT_VALUE(S0, muxState.pwrmuxOutput) << Board_PWRMUX_S)

				// Set the MUX enable output
				| (muxState.pwrmuxOutputEnable << Board_PWRMUX_ENABLE_N));

	if (result == PIN_SUCCESS) {
		return NoError;
	}

#ifdef SB_DEBUG
	System_printf("Received error setting IO MUX state: %d\n", result);
	System_flush();
#endif

	return UnknownError;
}

/**
 * \brief Refreshes the SYSDISBL hardware
 * \remark Returns as soon as the output is assigned, but keeps the MUX semaphore.
 * 			No MUX operations can complete until after SYSDSBL_REFRESH_CLOCK_PERIOD has elapsed.
 */
SB_Error SB_sysDisableRefresh(uint32 semaphoreTimeout) {
	SB_MUXState refreshState  = {
		.iomuxOutput = Board_IOMUX_SYSDISBL_N,
		.pwrmuxOutput = Board_PWRMUX_PERIPHERAL_VCC,
		.pwrmuxOutputEnable = MUX_ENABLE,
	};

	if (!Semaphore_pend(PMGR.muxSemaphore, semaphoreTimeout)) {
		// This is unknown error as that pend should never return (BIOS_WAIT_FOREVER)
		return UnknownError;
	}

	SB_Error result = _applyFullMuxState(refreshState);

	if (result != NoError) {
		Semaphore_post(PMGR.muxSemaphore);
		return result;
	}

	Util_startClock(&PMGR.sysdisblClock);

	return NoError;
}

void SB_sysdisblClockHandler(UArg arg) {
	Semaphore_post(PMGR.muxSemaphore);
}

/**
 * \brief Triggers the SYSDISBL shutdown. If shutdown is triggered this function does not return before the system loses power.
 */
SB_Error SB_sysDisableShutdown() {
	// TODO: Generate an error if jack power is present

	// IO MUX should connect the SYSDISBL output.
	// PWRMUX doesn't matter which output is select as it is disabled.
	SB_MUXState shutdownState  = {
		.iomuxOutput = Board_IOMUX_SYSDISBL_N,
		.pwrmuxOutput = Board_PWRMUX_PERIPHERAL_VCC,
		.pwrmuxOutputEnable = MUX_DISABLE,
	};

	if (!Semaphore_pend(PMGR.muxSemaphore, BIOS_WAIT_FOREVER)) {
		// This is unknown error as that pend should never return (BIOS_WAIT_FOREVER)
		return UnknownError;
	}

	SB_Error result = _applyFullMuxState(shutdownState);

	if (result != NoError) {
		Semaphore_post(PMGR.muxSemaphore);
		return result;
	}

	// Reconfigure the CONN_STATE_RD pin as a sink to speed shutdown
	PIN_setConfig(&PMGR.AnalogPins,
		PIN_BM_INPUT_EN | PIN_BM_PULLING | PIN_BM_GPIO_OUTPUT_EN | PIN_BM_GPIO_OUTPUT_VAL | PIN_BM_OUTPUT_BUF,
		PIN_INPUT_DIS   | PIN_NOPULL     | PIN_GPIO_OUTPUT_EN    | PIN_GPIO_LOW           | PIN_OPENDRAIN      | Board_CONN_STATE_RD);

	// Enable the current sink output
	PIN_setOutputValue(&PMGR.AnalogPins, Board_CONN_STATE_RD, PIN_LOW);

	// TODO: This loop should actually check on occasion if jack power has become available by switching MUX to V_PREBUCK_DIV2 for a moment
	// This function doesn't return - the system is about to die.
	while (1);
}
