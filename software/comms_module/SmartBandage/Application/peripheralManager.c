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

#include "flash.h"
#include "i2c.h"
#include "util.h"
#include "ble.h"
#include "fsm.h"
#include "clock.h"
#include "bandage.h"
#include "Devices/mcp9808.h"
#include "Devices/hdc1050.h"
#include "Devices/tca9554a.h"
#include "Devices/stc3115.h"
#include "peripheralManager.h"
#include "readingsManager.h"
#include "../PROFILES/smartBandageProfile.h"

SB_Error applyTempSensorConfiguration(uint8_t deviceNo);
SB_Error applyIOMuxState(MUX_OUTPUT_ENABLE outputEnable, MUX_OUTPUT output);
SB_Error applyPWRMuxState(MUX_OUTPUT_ENABLE outputEnable, MUX_OUTPUT output);
SB_Error applyFullMuxState(SB_MUXState *muxState, uint32 timeout);
SB_Error _applyFullMuxState(SB_MUXState *muxState);
void     SB_sysdisblClockHandler(UArg arg);

void PreEnterSleepCallback(SB_State_Transition transition, SB_State state);
void ExitSleepCallback(SB_State_Transition transition, SB_State state);
void PreEnterTransmitCallback(SB_State_Transition transition, SB_State state);
void PreExitTransmitCallback(SB_State_Transition transition, SB_State state);
void PreEnterCheckCallback(SB_State_Transition transition, SB_State state);

SB_Error readSensorData();

struct {
	Semaphore_Handle i2cDeviceSem;
	MCP9808_DEVICE mcp9808Devices[SB_NUM_MCP9808_SENSORS];
	SB_PeripheralState mcp9808DeviceStates[SB_NUM_MCP9808_SENSORS];

	HDC1050_DEVICE hdc1050Device;
	SB_PeripheralState hdc1050DeviceState;

#ifdef IOEXPANDER_PRESENT
	TCA9554A_DEVICE ioexpanderDevice;
	SB_PeripheralState ioexpanderDeviceState;
#endif

	STC3115_DEVICE_HANDLE gasGaugeDevice;
	SB_PeripheralState gasGaugeDeviceState;

	Task_Handle taskHandle;
	Task_Struct task;
	Char taskStack[PMGR_TASK_STACK_SIZE];

	PIN_State PeripheralPower;
	PIN_State MUXPins;
	PIN_State AnalogPins;
	Semaphore_Handle muxSemaphore;
	Clock_Struct sysdisblClock;

	Semaphore_Handle stateSem;
	Semaphore_Handle adcSem;
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
	configTransaction.completionSemaphore = &PMGR.i2cDeviceSem;

	// The resolution transaction
	resolutionBaseTransaction.writeCount   = 2;
	resolutionBaseTransaction.writeBuf     = &txBuf[3];
	resolutionBaseTransaction.readCount    = 0;
	resolutionBaseTransaction.readBuf      = NULL;
	resolutionBaseTransaction.slaveAddress = PMGR.mcp9808Devices[deviceNo].Address;

	resolutionTransaction.baseTransaction = &resolutionBaseTransaction;
	resolutionTransaction.completionSemaphore = &PMGR.i2cDeviceSem;

	// Queue the configuration and resolution transactions
	SB_i2cQueueTransaction(&configTransaction, BIOS_WAIT_FOREVER);
	SB_i2cQueueTransaction(&resolutionTransaction, BIOS_WAIT_FOREVER);

	// Wait for completion (twice)
	Semaphore_pend(PMGR.i2cDeviceSem, BIOS_WAIT_FOREVER);
	Semaphore_pend(PMGR.i2cDeviceSem, BIOS_WAIT_FOREVER);

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
	configTransaction.completionSemaphore = &PMGR.i2cDeviceSem;

	// Queue the configuration and resolution transactions
	SB_i2cQueueTransaction(&configTransaction, BIOS_WAIT_FOREVER);

	// Wait for completion
	Semaphore_pend(PMGR.i2cDeviceSem, BIOS_WAIT_FOREVER);

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
	configTransaction.completionSemaphore = &PMGR.i2cDeviceSem;

	// Queue the configuration and resolution transactions
	SB_i2cQueueTransaction(&configTransaction, BIOS_WAIT_FOREVER);

	// Wait for completion
	Semaphore_pend(PMGR.i2cDeviceSem, BIOS_WAIT_FOREVER);

	return configTransaction.completionResult;
}
#endif

/*********************************************************************
 * @fn      initPeripherals
 *
 * @brief   Initializes external peripherals. Called after peripheral power enabled
 *
 * @return  NoError if properly initialized, otherwise the error that occured
 */
SB_Error initPeripherals() {
	int i;

#ifdef IOEXPANDER_PRESENT
	// Initialize IO Expander
	PMGR.ioexpanderDevice.address = I2C_DBGIOEXP_ADDR;
	PMGR.ioexpanderDeviceState.lastError = applyIOExpanderConfiguration();

	if (NoError == PMGR.ioexpanderDeviceState.lastError) {
		PMGR.ioexpanderDeviceState.currentState = PState_OK;
	} else {
# ifdef SB_DEBUG
		System_printf("IO Expander config failed: %d...\n", PMGR.ioexpanderDeviceState.lastError);
# endif
		PMGR.ioexpanderDeviceState.currentState = PState_FailedConfig;
	}

#endif

	// Initialize MCP9808 sensors
	for (i = 0; i < SB_NUM_MCP9808_SENSORS; ++i) {
		if (PMGR.mcp9808DeviceStates[i].currentState != PState_Failed) {
			PMGR.mcp9808Devices[i].Address = Mcp9808Addresses[i];
			PMGR.mcp9808DeviceStates[i].lastError = applyTempSensorConfiguration(i);

			if (NoError == PMGR.mcp9808DeviceStates[i].lastError) {
				PMGR.mcp9808DeviceStates[i].currentState = PState_OK;
			} else {
				PMGR.mcp9808DeviceStates[i].currentState = PState_Intermittent;
				if (++PMGR.mcp9808DeviceStates[i].numReadAttempts > PERIPHERAL_MAX_READ_ATTEMPTS) {
					PMGR.mcp9808DeviceStates[i].currentState = PState_Failed;
#ifdef SB_DEBUG
					System_printf("PMGR: Temperature sensor failed permanently: %d\n", i);
#endif
				} else {
#ifdef SB_DEBUG
					System_printf("MCP9808 device %d init failed...\n", i);
#endif
				}
			}


		}
	}



	// Initialize Humidity Sensor
	PMGR.hdc1050Device.address = HDC1050_I2C_ADDRESS;
	PMGR.hdc1050DeviceState.lastError = applyHumiditySensorConfiguration();
	if (PMGR.hdc1050DeviceState.lastError == NoError) {
		PMGR.hdc1050DeviceState.currentState = PState_OK;
	} else {
# ifdef SB_DEBUG
		System_printf("Humidity sensor config failed: %d...\n", PMGR.hdc1050DeviceState.lastError);
		System_flush();
# endif
		PMGR.hdc1050DeviceState.currentState = PState_FailedConfig;
	}


	// Start the conversion for the humidity sensor
	PMGR.hdc1050DeviceState.lastError = hdc1050_startTempHumidityConversion(&PMGR.hdc1050Device, &PMGR.i2cDeviceSem);
	if (PMGR.hdc1050DeviceState.lastError == NoError) {
		PMGR.hdc1050Device.readReadyTime = HDC1050_READ_WAIT_TICKS + Clock_getTicks();
	} else {
		if (++PMGR.hdc1050DeviceState.numReadAttempts > PERIPHERAL_MAX_READ_ATTEMPTS) {
			PMGR.hdc1050DeviceState.currentState = PState_Failed;
#ifdef SB_DEBUG
		System_printf("PMGR: HDC1050 sensor failed permanently: %d\n", i);
#endif
		} else {
#ifdef SB_DEBUG
		System_printf("PMGR: HDC1050 read failed.\n");
#endif
		}
	}


	return NoError;
}

/*********************************************************************
 * @fn      initAlwaysOnPeripherals
 *
 * @brief   Initializes external peripherals that are always on
 */
SB_Error initAlwaysOnPeripherals() {
	// Initialize Gas Gauge
//	PMGR.gasGaugeDeviceState.currentState = PState_Failed;
	if (PMGR.gasGaugeDeviceState.currentState != PState_Failed) {
		stc3115_address(PMGR.gasGaugeDevice) = I2C_SENSOR_GASGAUGE_ADDR;
		PMGR.gasGaugeDeviceState.currentState = PState_Intermittent;

		PMGR.gasGaugeDeviceState.lastError = stc3115_init(PMGR.gasGaugeDevice, &PMGR.i2cDeviceSem);
		if (PMGR.gasGaugeDeviceState.lastError == NoError) {
			PMGR.gasGaugeDeviceState.lastError = stc3115_configure(
					PMGR.gasGaugeDevice,

					&PMGR.i2cDeviceSem,
					GASGAUGE_SENSE_RESISTOR,
					GASGAUGE_BATT_INTERNAL_IMPEDANCE,
					GASGAUGE_BATT_CAPACITY);

			if (PMGR.gasGaugeDeviceState.lastError == NoError) {
				PMGR.gasGaugeDeviceState.currentState = PState_OK;
			} else {
				if (++PMGR.gasGaugeDeviceState.numReadAttempts > PERIPHERAL_MAX_READ_ATTEMPTS) {
					PMGR.gasGaugeDeviceState.currentState = PState_Failed;
#ifdef SB_DEBUG
				System_printf("PMGR: Gas Gauge failed permanently: %d\n", PMGR.gasGaugeDeviceState.lastError);
#endif
				} else {
#ifdef SB_DEBUG
				System_printf("PMGR: Gas Gauge read failed.\n");
#endif
				}
			}
		} else {
			if (++PMGR.gasGaugeDeviceState.numReadAttempts > PERIPHERAL_MAX_READ_ATTEMPTS) {
				PMGR.gasGaugeDeviceState.currentState = PState_Failed;
#ifdef SB_DEBUG
			System_printf("PMGR: Gas Gauge failed permanently: %d\n", PMGR.gasGaugeDeviceState.lastError);
#endif
			} else {
#ifdef SB_DEBUG
			System_printf("PMGR: Gas Gauge read failed.\n");
#endif
			}
		}
	}

	if (stc3115_id(PMGR.gasGaugeDevice) != STC3115_DEVICE_ID) {
		return ResourceNotInitialized;
	}

	return NoError;
}

/*********************************************************************
 * @fn      readSensorData
 *
 * @brief   Retrieves readings from all external peripherals and moisture lines,
 * 			stores them in a reading, saves to flash, and updates BLE characteristics
 */
SB_Error readSensorData() {
	SB_PeripheralReadings readings;
	SB_i2cTransaction taTransaction;
	I2C_Transaction taBaseTransaction;
	uint8_t txBuf[1];
	uint8_t rxBuf[2];
	uint8_t i;
	SB_Error result;

#ifdef BANDAGE_IMPEDANCE_READINGS
	// Trigger the start of bandage readings
	if (NoError != (result = SB_beginReadBandageImpedances(BIOS_NO_WAIT, &readings.moistures))) {
# ifdef SB_DEBUG
		System_printf("Could not start bandage impedance reading: %d", result);
		System_flush();
# endif
	}
#endif

	// The configuration transaction for the temperature sensors
	taBaseTransaction.writeCount   = 1;
	taBaseTransaction.writeBuf     = txBuf;
	taBaseTransaction.readCount    = 2;
	taBaseTransaction.readBuf      = rxBuf;

	taTransaction.baseTransaction = &taBaseTransaction;
	taTransaction.completionSemaphore = &PMGR.i2cDeviceSem;

	// Read temperature sensors
	{
		for (i = 0; i < SB_NUM_MCP9808_SENSORS; ++i) {
			// Only talk to good or intermittent sensors
			if (PMGR.mcp9808DeviceStates[i].currentState == PState_OK || PMGR.mcp9808DeviceStates[i].currentState == PState_Intermittent) {
#ifndef LAUNCHPAD
				if (NoError != tca9554a_setPinStatus(&PMGR.ioexpanderDevice, &PMGR.i2cDeviceSem, IOEXP_I2CSTATUS_PIN_TEMP(i), true)) {
					System_printf("IOEXP Error");
					System_flush();
				}
#endif

				txBuf[0] = MCP9808_REG_TA;
				taBaseTransaction.slaveAddress = PMGR.mcp9808Devices[i].Address;

				// Queue the configuration and resolution transactions
				SB_i2cQueueTransaction(&taTransaction, BIOS_WAIT_FOREVER);

				// Wait for completion (twice)
				Semaphore_pend(PMGR.i2cDeviceSem, BIOS_WAIT_FOREVER);

#ifndef LAUNCHPAD
				if (NoError != tca9554a_setPinStatus(&PMGR.ioexpanderDevice, &PMGR.i2cDeviceSem, IOEXP_I2CSTATUS_PIN_TEMP(i), false)) {
					System_printf("IOEXP Error");
					System_flush();
				}
#endif

				// Handle success or failure of the I2C operation
				if (taTransaction.completionResult == NoError) {
					// The temperature sensor is big endian and this device is little endian
					// Also need to apply the mask for the data from the sensor: 0x0FFF
					PMGR.mcp9808Devices[i].Temperature = 0x0FFF & ((rxBuf[0] << 8) | (rxBuf[1]));
#ifdef SB_DEBUG
					System_printf("PMGR: Temperature read: %d\n", PMGR.mcp9808Devices[i].Temperature>>4);
#endif
					readings.temperatures[i] = PMGR.mcp9808Devices[i].Temperature;

					SB_Profile_Set16bParameter( SB_CHARACTERISTIC_TEMPERATURE, PMGR.mcp9808Devices[i].Temperature, i );
				} else {
					// The I2C transaction failed. Manage sensor state
					PMGR.mcp9808DeviceStates[i].currentState = PState_Intermittent;
					if (++PMGR.mcp9808DeviceStates[i].numReadAttempts > PERIPHERAL_MAX_READ_ATTEMPTS) {
						PMGR.mcp9808DeviceStates[i].currentState = PState_Failed;
#ifdef SB_DEBUG
					System_printf("PMGR: Temperature sensor failed permanently: %d\n", i);
#endif
					} else {
#ifdef SB_DEBUG
					System_printf("PMGR: Temperature read failed.\n");
#endif
					}
				}


			}
		}
	}

	// Humidity Sensor has longest init time
	{
		if (PMGR.hdc1050DeviceState.currentState == PState_OK || PMGR.hdc1050DeviceState.currentState == PState_Intermittent) {
			// Sleep if result not yet ready
			int currentTime = Clock_getTicks();
			if (PMGR.hdc1050Device.readReadyTime > currentTime) {
				Task_sleep(PMGR.hdc1050Device.readReadyTime - currentTime);
			}

#ifndef LAUNCHPAD
			if (NoError != tca9554a_setPinStatus(&PMGR.ioexpanderDevice, &PMGR.i2cDeviceSem, IOEXP_I2CSTATUS_PIN_HUMIDITY, true)) {
				System_printf("IOEXP Error\n");
				System_flush();
			}
#endif

			// Start the conversion for the humidity sensor
			PMGR.hdc1050DeviceState.lastError = hdc1050_readTempHumidity(&PMGR.hdc1050Device, &PMGR.i2cDeviceSem);

			if (PMGR.hdc1050DeviceState.lastError == NoError) {
#ifdef SB_DEBUG
				System_printf("PMGR: Humidity read:  %d\n", PMGR.hdc1050Device.humidity/16);
				System_printf("PMGR: HTemp read:  %d\n", PMGR.hdc1050Device.temperature/16);
#endif

				SB_Profile_Set16bParameter( SB_CHARACTERISTIC_HUMIDITY, PMGR.hdc1050Device.humidity, 0 );
				SB_Profile_Set16bParameter( SB_CHARACTERISTIC_TEMPERATURE, PMGR.hdc1050Device.temperature, SB_NUM_MCP9808_SENSORS );

				readings.humidities[0] = PMGR.hdc1050Device.humidity;
				readings.temperatures[SB_NUM_MCP9808_SENSORS] = PMGR.hdc1050Device.temperature;
			} else {
				if (++PMGR.hdc1050DeviceState.numReadAttempts > PERIPHERAL_MAX_READ_ATTEMPTS) {
					PMGR.hdc1050DeviceState.currentState = PState_Failed;
#ifdef SB_DEBUG
				System_printf("PMGR: HDC1050 sensor failed permanently: %d\n", i);
#endif
				} else {
#ifdef SB_DEBUG
				System_printf("PMGR: HDC1050 read failed.\n");
#endif
				}
			}


#ifndef LAUNCHPAD
			if (NoError != tca9554a_setPinStatus(&PMGR.ioexpanderDevice, &PMGR.i2cDeviceSem, IOEXP_I2CSTATUS_PIN_HUMIDITY, false)) {
# ifdef SB_DEBUG
				System_printf("IOEXP Error\n");
				System_flush();
# endif
			}

#endif
		}
	}

	// Read gas gauge
	stc3115_readInfo(PMGR.gasGaugeDevice, &PMGR.i2cDeviceSem);
#ifdef SB_DEBUG
	System_printf("PMGR: Battery voltage: %fmV\n", stc3115_convertedVoltage(PMGR.gasGaugeDevice)/16.);
#endif
	SB_Profile_Set16bParameter( SB_CHARACTERISTIC_BATTCHARGE, stc3115_convertedVoltage(PMGR.gasGaugeDevice), 0 );

#ifdef BANDAGE_IMPEDANCE_READINGS
	// Wait for ADC readings to be available
	if (NoError != (result = SB_waitForReadingsAvailable())) {
# ifdef SB_DEBUG
		System_printf("PMGR: Error waiting for ADC readings to be available: %d\n", result);
# endif
	}

	// Convert moisture readings to percentages
	for (i = 0; i < SB_NUM_MOISTURE; ++i) {
		readings.moistures[i] = (100L*16L*(208 - readings.moistures[i]))/208;
	}
#endif

	// Write the temporary moisture parameter
	SB_Profile_SetParameter( SB_CHARACTERISTIC_MOISTUREMAP, sizeof(SB_READING_T) * SB_NUM_MOISTURE, readings.moistures );

	// Write the data to flash storage
	readings.timeDiff = SB_clockGetTime() - SB_flashGetReferenceTime();
	result = SB_flashWriteReadings(&readings);
	if (NoError != result) {
		return result;
	}

	// Tell the readings manager new readings are available
	SB_newReadingsAvailable();

	return NoError;
}

/*********************************************************************
 * @fn      SB_peripheralManagerTask
 *
 * @brief   The main task function for the peripheral manager.
 */
static void SB_peripheralManagerTask(UArg a0, UArg a1) {
	SB_Error result;

#ifdef SB_DEBUG
	System_printf("Peripheral manager task started...\n");
	System_flush();
#endif

	SB_clockInit();

	SimpleBLEPeripheral_init();

#ifdef SB_DEBUG
	System_printf("BLE Initialized.\n");
	System_flush();
#endif

	// Initialize readings size parameter
	SB_Profile_Set16bParameter( SB_CHARACTERISTIC_READINGSIZE, sizeof(SB_PeripheralReadings), 0 );

	SB_setPeripheralsEnable(true);
	result = SB_sysDisableRefresh(BIOS_WAIT_FOREVER);

	if (NoError != result) {
#ifdef SB_DEBUG
		System_printf("Peripheral initialization failure: %d. Could not set MUX output.\n", result);
		System_flush();
#endif
	}

	if (NoError != (result = initAlwaysOnPeripherals())) {
#ifdef SB_DEBUG
		System_printf("Always on peripheral initialization failure: %d.\n", result);
		System_flush();
#endif
	}

	if (NoError != (result = initPeripherals())) {
#ifdef SB_DEBUG
		System_printf("Peripheral initialization failure: %d. Peripheral Manager stalled.\n", result);
		System_flush();
#endif
		Task_exit();
	}

	if (NoError != (result = SB_bandageInit(PMGR.adcSem))) {
#ifdef SB_DEBUG
		System_printf("Error No: %d\n", result);
		System_printf("SB application initialization failed while initializing adc. This is a code error.\n");
		System_flush();
#endif

		forever;
	}

	if (NoError != (result = SB_flashInit(sizeof(SB_PeripheralReadings), SB_REINIT_FLASH_ON_START))) {
#ifdef SB_DEBUG
		System_printf("Error No: %d\n", result);
		System_printf("SB application initialization failed while initializing non-volatile flash storage. This is a code error.\n");
		System_flush();
#endif

		forever;
	}

#ifdef SB_DEBUG
	System_printf("Non-Volatile Memory Initialized.\n");
	System_flush();
#endif

	if (NoError != (result = SB_readingsManagerInit())) {
#ifdef SB_DEBUG
		System_printf("Error No: %d\n", result);
		System_printf("SB application initialization failed while initializing readings manager. This is a code error.\n");
		System_flush();
#endif

		forever;
	}

#ifdef SB_DEBUG
	System_printf("PMGR: Peripherals initialized.\n", result);
	System_flush();
#endif
#ifdef LAUNCHPAD
	PIN_State sbpPins;
//	PIN_Handle statusPin = PIN_open(&sbpPins, pinConfigTable);
#endif

	SB_switchState(S_CHECK);

	bool bleLedStatus = false;
	uint8_t nChecks = 0;
	uint8_t timeouts = 0;
	uint32_t startTime;
	forever {
		// Wait for a state change to occur
		Semaphore_pend(PMGR.stateSem, BIOS_WAIT_FOREVER);
		System_printf("Loop started %d\n", SB_currentState());
		System_flush();
		Task_sleep(NTICKS_PER_MILLSECOND * SB_GlobalDeviceConfiguration.CheckSleepIntervalMS);

		switch (SB_currentState()) {
		case S_CHECK:
#ifdef PERIPHERAL_PWR_MGMT
			// Enable peripherals
			SB_setPeripheralsEnable(true);
#endif

			// Initialize them
			initPeripherals();

			if (SB_GlobalDeviceConfiguration.CheckReadDelayMS > 0) {
				Task_sleep(NTICKS_PER_MILLSECOND * SB_GlobalDeviceConfiguration.CheckReadDelayMS);
			}

			// Read sensor data
			result = readSensorData();
			if (NoError != result) {
#ifdef SB_DEBUG
				System_printf("PMGR: Saving readings failed: %d.\n", result);
#endif
			}

#ifdef SB_DEBUG
			System_flush();
#endif

#ifdef PERIPHERAL_PWR_MGMT
			// Disable peripherals
			SB_setPeripheralsEnable(false);
#endif

			// Transition out of the check state
			SB_switchState(S_SLEEP);
			break; // S_CHECK

		case S_TRANSMIT:
			result = SB_enableBLE();
			if (NoError != result) {
				System_printf("Error enabling BLE for transmission: %d", result);
			}

			// Turn on the BLE LED
#ifndef LAUNCHPAD
			bleLedStatus = true;
			if (NoError != tca9554a_setPinStatus(&PMGR.ioexpanderDevice, &PMGR.i2cDeviceSem, IOEXP_I2CSTATUS_PIN_BLE, bleLedStatus)) {
				System_printf("IOEXP Error");
				System_flush();
			}
#endif

			// Do a single quick reading now
			result = readSensorData();
			if (NoError != result) {
			#ifdef SB_DEBUG
				System_printf("PMGR: Saving readings failed: %d.\n", result);
			#endif
			}

			// TODO: SB_TRANSMIT_MAX_STATE_TIME should be the min amount of time we spend waiting for a connection.
			// Once connected, provided that the other device consumes a block of readings at least every SB_TRANSMIT_MIN_CONN_PERIOD
			// we should stay connected until complete. We can be assured of a return to sleep as memory is finite and we
			// aren't adding new readings in the transmit state.

			// Spend the next SB_TRANSMIT_MAX_STATE_TIME talking over bluetooth
			SB_setClearReadingsMode(true);

			startTime = Clock_getTicks();
			while ((Clock_getTicks() - startTime) < (NTICKS_PER_SECOND * SB_GlobalDeviceConfiguration.MaxTransmitStateTimeS)) {
#ifndef LAUNCHPAD
				bleLedStatus = !bleLedStatus;
				tca9554a_setPinStatus(&PMGR.ioexpanderDevice, &PMGR.i2cDeviceSem, IOEXP_I2CSTATUS_PIN_BLE, bleLedStatus);
#endif

				// This could end up taking up to SB_TRANSMIT_MIN_CONN_PERIOD more than SB_TRANSMIT_MAX_STATE_TIME. Do we care?
				ICall_Errno errno = ICall_wait(500);

				if (errno == ICALL_ERRNO_SUCCESS)
				{
					SB_processBLEMessages();
				} else {
					// Handle the error that a request wasn't received in a SB_TRANSMIT_MIN_CONN_PERIOD
				}
			}

			SB_setClearReadingsMode(false);

			result = SB_disableBLE();
			if (NoError != result) {
#ifdef SB_DEBUG
				System_printf("Error disabling BLE for transmission: %d\n", result);
#endif
			}

			while (SB_bleConnected()) {
#ifndef LAUNCHPAD
				bleLedStatus = !bleLedStatus;
				tca9554a_setPinStatus(&PMGR.ioexpanderDevice, &PMGR.i2cDeviceSem, IOEXP_I2CSTATUS_PIN_BLE, bleLedStatus);
#endif
				// This could end up taking up to SB_TRANSMIT_MIN_CONN_PERIOD more than SB_TRANSMIT_MAX_STATE_TIME. Do we care?
				ICall_Errno errno = ICall_wait(500);

				if (errno == ICALL_ERRNO_SUCCESS)
				{
					SB_processBLEMessages();
				} else if (++timeouts > 10) {
					// Handle the error that a request wasn't received in a SB_TRANSMIT_MIN_CONN_PERIOD
					break;
				}
			}

			timeouts = 0;

#ifdef SB_DEBUG
			System_flush();
#endif
#ifndef LAUNCHPAD
			bleLedStatus = false;
			tca9554a_setPinStatus(&PMGR.ioexpanderDevice, &PMGR.i2cDeviceSem, IOEXP_I2CSTATUS_PIN_BLE, bleLedStatus);
#endif

			// Transition out of the transmit state
			SB_switchState(S_SLEEP);
			break; // S_TRANSMIT

		case S_SLEEP:
			// Todo this may be best handled in the state manager?
			if (++nChecks < SB_GlobalDeviceConfiguration.BLECheckInterval) {
				SB_switchState(S_CHECK);
			} else {
				SB_switchState(S_TRANSMIT);
				nChecks = 0;
			}
			break; // S_SLEEP

		default:
			break;
		}
	}
}

/*********************************************************************
 * @fn      SB_peripheralInit
 *
 * @brief   Initializes the peripheral manager
 *
 * @return  NoError if properly initialized, otherwise the error that occured
 */
SB_Error SB_peripheralInit() {
	SB_Error result;
	// Initialize MUX semaphore with 1 free resource (use as mutex)
	PMGR.muxSemaphore = Semaphore_create(1, NULL, NULL);
	PMGR.stateSem = Semaphore_create(0, NULL, NULL);
	PMGR.i2cDeviceSem = Semaphore_create(0, NULL, NULL);
	PMGR.adcSem = Semaphore_create(0, NULL, NULL);

	if (NULL == PMGR.muxSemaphore || NULL == PMGR.stateSem || NULL == PMGR.i2cDeviceSem || NULL == PMGR.adcSem) {
		return OSResourceInitializationError;
	}

	// Initialize power pin
	PIN_Config peripheralPowerConfigTable[] =
	{
		Board_PERIPHERAL_PWR | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
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
		Board_MP_EN_SW		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		Board_MSW_0			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		Board_MSW_1			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		Board_MSW_2			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
		Board_MPSW			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
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
		Board_BANDAGE_A_0		| PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS ,
		Board_CONN_STATE_RD		| PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS ,
		Board_VSENSE_0			| PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS ,
		Board_VSENSE_1			| PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS ,
		Board_1V3				| PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS ,
		PIN_TERMINATE
	};

	if (!PIN_open(&PMGR.AnalogPins, analogPinsConfigTable)) {
#ifdef SB_DEBUG
	System_printf("Failed to initialize analog pins...\n");
	System_flush();
#endif
		return OSResourceInitializationError;
	}

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

	// Initialize state callback functions
	if (
		NoError != (result = SB_registerStateTransitionCallback(PreEnterSleepCallback, 	 T_STATE_PRE_ENTER, S_SLEEP))
		|| NoError != (result = SB_registerStateTransitionCallback(PreEnterTransmitCallback, T_STATE_PRE_ENTER, S_TRANSMIT))
		|| NoError != (result = SB_registerStateTransitionCallback(PreEnterCheckCallback, 	 T_STATE_PRE_ENTER, S_CHECK))
		|| NoError != (result = SB_registerStateTransitionCallback(ExitSleepCallback, 		 T_STATE_EXIT, 		S_SLEEP))
		|| NoError != (result = SB_registerStateTransitionCallback(PreExitTransmitCallback,  T_STATE_PRE_EXIT,  S_TRANSMIT))) {
		System_printf("Error initializing peripheral manager callbacks: %d\n", result);
		return result;
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

/*********************************************************************
 * @fn      SB_setPeripheralsEnable
 *
 * @brief   Enables or disables peripherals
 *
 * @return  NoError if properly enabled/disabled, otherwise the error that occured
 */
SB_Error SB_setPeripheralsEnable(bool enable) {
	PIN_Status result = PIN_setOutputValue(&PMGR.PeripheralPower, Board_PERIPHERAL_PWR, enable == false);
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
SB_Error applyFullMuxState(SB_MUXState *muxState, uint32 timeout) {
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
SB_Error _applyFullMuxState(SB_MUXState *muxState) {
	PIN_Status result =
			PIN_setPortOutputValue(&PMGR.MUXPins,
				// Set the MUX Select S* outputs
				  (MUX_SELECT_VALUE(S0, muxState->iomuxOutput) << Board_IOMUX_S0)
				| (MUX_SELECT_VALUE(S1, muxState->iomuxOutput) << Board_IOMUX_S1)
				| (MUX_SELECT_VALUE(S2, muxState->iomuxOutput) << Board_IOMUX_S2)

				| (MUX_SELECT_VALUE(S0, muxState->pwrmuxOutput) << Board_PWRMUX_S)

				// Set the MUX enable output
				| (muxState->pwrmuxOutputEnable << Board_PWRMUX_ENABLE_N));

	if (result == PIN_SUCCESS) {
		return NoError;
	}

#ifdef SB_DEBUG
	System_printf("Received error setting IO MUX state: %d\n", result);
	System_flush();
#endif

	return UnknownError;
}

/*********************************************************************
 * @fn      SB_selectMoistureSensorInput
 *
 * @brief   Selects the current moisture sensor input
 *
 * @return  NoError if properly selected, otherwise the error that occured
 */
SB_Error SB_selectMoistureSensorInput(SB_MoistureSensorLine line, SB_MoistureSensorVoltage voltage, uint32_t timeout) {
	SB_MUXState selectState  = {
		.pwrmuxOutputEnable = MUX_ENABLE,
	};

	// Get the output for the desired line
	switch (line) {
	case BANDAGE_A_0:
		selectState.iomuxOutput = Board_IOMUX_BANDAGE_A_0;
		break;
	case BANDAGE_A_1:
		selectState.iomuxOutput = Board_IOMUX_BANDAGE_A_1;
		break;
	case BANDAGE_A_2:
		selectState.iomuxOutput = Board_IOMUX_BANDAGE_A_2;
		break;
	case BANDAGE_A_3:
		selectState.iomuxOutput = Board_IOMUX_BANDAGE_A_3;
		break;
	case BANDAGE_A_4:
		selectState.iomuxOutput = Board_IOMUX_BANDAGE_A_4;
		break;
	default:
		return InvalidParameter;
	}

	// Configure the correct voltage
	switch(voltage) {
	case MOISTURE_V_1V3:
		selectState.pwrmuxOutput = Board_PWRMUX_1V3;
		break;

	case MOISTURE_V_PERIPHERAL_VCC:
		selectState.pwrmuxOutput = Board_PWRMUX_PERIPHERAL_VCC;
		break;

	default:
		return InvalidParameter;
	}

	return applyFullMuxState(&selectState, timeout);
}

/*********************************************************************
 * @fn      SB_sysDisableRefresh
 *
 * @brief   Refreshes the sys disable output waits at most semaphoreTimeout to access resources
 *
 * @return  NoError if properly refreshed, otherwise the error that occured
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

	SB_Error result = _applyFullMuxState(&refreshState);

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

/*********************************************************************
 * @fn      SB_sysDisableShutdown
 *
 * @brief   Shuts down the MCU through the sys disable output. This function doesn't return.
 *
 * @return  Doesn't return if success, otherwise the error that occured
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

	SB_Error result = _applyFullMuxState(&shutdownState);

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

void PreEnterSleepCallback(SB_State_Transition transition, SB_State state) {
	System_printf("PreEnterSleepCallback\n");
	Semaphore_post(PMGR.stateSem);
}

void ExitSleepCallback(SB_State_Transition transition, SB_State state) {
	System_printf("ExitSleepCallback\n");
}

void PreEnterTransmitCallback(SB_State_Transition transition, SB_State state) {
	System_printf("PreEnterTransmitCallback\n");
	Semaphore_post(PMGR.stateSem);
}

void PreExitTransmitCallback(SB_State_Transition transition, SB_State state) {
	System_printf("PreExitTransmitCallback\n");
}

void PreEnterCheckCallback(SB_State_Transition transition, SB_State state) {
	System_printf("PreEnterCheckCallback\n");
	Semaphore_post(PMGR.stateSem);
}
