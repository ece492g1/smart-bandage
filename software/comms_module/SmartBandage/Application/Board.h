/*
 * @file config.h
 * @brief Contains configuration variables for the application.
 *
 *  Created on: Feb 9, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_CONFIG_H_
#define APPLICATION_CONFIG_H_

#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

/*****************************************************************
 * Config Adjust Variables
 ****************************************************************/
#define LAUNCHPAD // Only define if using Launchpad for testing. Can be defined as compiler argument instead.
#define SB_DEBUG
#define SB_FLASH_SANITY_CHECKS
#define POWER_SAVING
#define IOEXPANDER_PRESENT
#define Board_BATT_110MAH
//#define PERIPHERAL_PWR_MGMT // Define to enable power management of external peripherals
#define BANDAGE_IMPEDANCE_READINGS

//#define SET_BDG_ID
#define BANDAGE14
//#define BANDAGE88

/*****************************************************************
 * General Configuration
 ****************************************************************/
// System clock ticks at 10us period
#define NTICKS_PER_SECOND 100000U
#define NTICKS_PER_MILLSECOND NTICKS_PER_SECOND/1000

#define DEFAULT_TASK_STACK_SIZE 512

#define I2C_TASK_STACK_SIZE   	512
#define GAPROLE_TASK_STACK_SIZE	600
#define PMGR_TASK_STACK_SIZE  	768
#define ICALL_TASK_STACK_SIZE 	800

typedef enum {
	IDLE_TASK_PRIORITY = 0,
	PMGR_TASK_PRIORITY,
	I2C_TASK_PRIORITY,
	GAPROLE_TASK_PRIORITY,
	ICALL_TASK_PRIORITY,
} TASK_PRIORITIES;

/*****************************************************************
 * IO Configuration
 ****************************************************************/
#define Board_MP_EN_SW   		IOID_0
#define Board_MSW_0		 		IOID_1
#define Board_MSW_1		 		IOID_2
#define Board_MSW_2		 		IOID_3
#define Board_MPSW		 		IOID_4
#define Board_SCL		 		IOID_7
#define Board_SDA		 		IOID_8
#define Board_BANDAGE_A_0  		IOID_9
#define Board_CONN_STATE_RD		IOID_10
#define Board_PERIPHERAL_PWR	IOID_11
#define Board_VSENSE_1			IOID_12
#define Board_1V3				IOID_13
#define Board_VSENSE_0			IOID_14

// TODO: Detect number of sensors. Should be 3 when bandage connected
#define SB_NUM_MCP9808_SENSORS 3
extern uint8_t Mcp9808Addresses[];

#define SB_NUM_TEMPERATURE  1+SB_NUM_MCP9808_SENSORS
#define SB_NUM_HUMIDITY		1
#define SB_NUM_MOISTURE		5
#define SB_READING_T		uint16

#ifdef LAUNCHPAD
#undef Board_SCL
#undef Board_SDA
#define Board_SDA		 		IOID_5
#define Board_T_LED_RED			IOID_6
#define Board_T_LED_GREEN		IOID_7
#define Board_SCL		 		IOID_8

#undef SB_NUM_MCP9808_SENSORS
#define SB_NUM_MCP9808_SENSORS 1
#endif

#define SYSDSBL_REFRESH_CLOCK_PERIOD 500

#define PIN_HIGH 1
#define PIN_LOW  0

/*****************************************************************
 * Time parameters
 ****************************************************************/
#define SB_TIMESTAMP_T uint32
#define SB_TIMEDIFF_T  uint16

/*****************************************************************
 * Flash parameters
 ****************************************************************/
#define SB_REINIT_FLASH_ON_START true

/*****************************************************************
 * External MUX configurations
 ****************************************************************/
#include "mux.h"

#define Board_IOMUX_S0 					Board_MSW_0
#define Board_IOMUX_S1 					Board_MSW_1
#define Board_IOMUX_S2 					Board_MSW_2
#define Board_IOMUX_BANDAGE_A_0			((MUX_OUTPUT)Y3)
#define Board_IOMUX_BANDAGE_A_1			((MUX_OUTPUT)Y2)
#define Board_IOMUX_BANDAGE_A_2			((MUX_OUTPUT)Y1)
#define Board_IOMUX_BANDAGE_A_3			((MUX_OUTPUT)Y0)
#define Board_IOMUX_BANDAGE_A_4			((MUX_OUTPUT)Y4)
#define Board_IOMUX_PERIPHERAL_DETECT	((MUX_OUTPUT)Y5)
#define Board_IOMUX_SYSDISBL_N			((MUX_OUTPUT)Y6)
#define Board_IOMUX_V_PREBUCK_DIV2		((MUX_OUTPUT)Y7)

#define Board_PWRMUX_S 							Board_MPSW
#define Board_PWRMUX_ENABLE_N 					Board_MP_EN_SW
#define Board_PWRMUX_PERIPHERAL_VCC				((MUX_OUTPUT)Y1)
#define Board_PWRMUX_1V3						((MUX_OUTPUT)Y0)

typedef enum {
	BANDAGE_A_0,
	BANDAGE_A_1,
	BANDAGE_A_2,
	BANDAGE_A_3,
	BANDAGE_A_4,

	SB_NumMoistureSensorLine
} SB_MoistureSensorLine;

typedef enum {
	MOISTURE_V_1V3,
	MOISTURE_V_PERIPHERAL_VCC,
} SB_MoistureSensorVoltage;

/*****************************************************************
 * Gas Gauge Configuration
 ****************************************************************/
#define GASGAUGE_SENSE_RESISTOR 		 10

#ifdef Board_BATT_110MAH
#define GASGAUGE_BATT_INTERNAL_IMPEDANCE 250
#define GASGAUGE_BATT_CAPACITY 			 110
#endif

/*****************************************************************
 * I2C Configuration
 ****************************************************************/
/* I2C Peripheral Addresses */
#define I2C_SENSOR_TEMP0_ADDR	 0b0011000
#define I2C_SENSOR_TEMP1_ADDR	 0b0011001
#define I2C_SENSOR_TEMP2_ADDR	 0b0011011
#define I2C_SENSOR_TEMPDBG_ADDR  0b0011000
#define I2C_SENSOR_HUMIDITY_ADDR 0b1000000
#define I2C_SENSOR_GASGAUGE_ADDR 0b1110000
#define I2C_DBGIOEXP_ADDR		 0b0111111

/* Custom I2C module config */
//#define I2C_ENABLE_TIMEOUT
#define I2C_TIMEOUT_PERIOD 10

/* Interface definitions */
#define I2C_BITRATE    				1 			// 0 = 100kHz, 1 = 400kHz
#define Board_I2C0_SDA0             Board_SDA
#define Board_I2C0_SCL0             Board_SCL

/* Generic I2C instance identifiers */
#define Board_I2C                   CC2650_I2C0
/* Generic Crypto instance identifiers */
#define Board_CRYPTO                CC2650_CRYPTO0
/* Generic Watchdog instance identifiers */
#define Board_WATCHDOG              CC2650_WATCHDOG0

#ifndef I2C_BITRATE
# error "I2C_BITRATE must be assigned a value."
#elif I2C_BITRATE < 0
# error "I2C_BITRATE has invalid value: Must be 0 or 1."
#elif I2C_BITRATE > 1
# error "I2C_BITRATE has invalid value: Must be 0 or 1."
#endif

/*!
 *  @def    CC2650_I2CName
 *  @brief  Enum of I2C names on the CC2650 dev board
 */
typedef enum CC2650_I2CName {
    CC2650_I2C0 = 0,
    CC2650_I2CCOUNT
} CC2650_I2CName;

/*****************************************************************
 * GPIO Configuration
 ****************************************************************/
extern PIN_Config BoardGpioInitTable[];

/*****************************************************************
 * Analog Configuration
 ****************************************************************/
#define SB_ADC_SAMPLE_TIME   AUXADC_SAMPLE_TIME_10P9_MS

#define SB_AN_BDG_A_0 		 ADC_COMPB_IN_AUXIO4
#define SB_AN_CONN_STATE_RD  ADC_COMPB_IN_AUXIO5
#define SB_AN_PERIPHERAL_PWR ADC_COMPB_IN_AUXIO3
#define SB_AN_VSENSE_1 		 ADC_COMPB_IN_AUXIO2
#define SB_AN_VSENSE_0 		 ADC_COMPB_IN_AUXIO0
#define SB_AN_VSENSE_1V3 	 ADC_COMPB_IN_AUXIO1

/*****************************************************************
 * Application Errors
 ****************************************************************/
typedef enum {
	UnknownError,
	NoError,
	I2CInitializationFailedError,
	OSResourceInitializationError,
	ResourceNotInitialized,
	InvalidParameter,
	OperationTimeout,
	OutOfMemory,
	SemaphorePendTimeout,
	SanityCheckFailed,
	WritePermissionDenied,
	BLECharacteristicWriteError,
	NoDataAvailable,
	ResourceBusy,
} SB_Error;

/*****************************************************************
 * State Timing
 ****************************************************************/
#define SB_TRANSMIT_MIN_CONN_PERIOD NTICKS_PER_SECOND    // One second
#define SB_TRANSMIT_MAX_STATE_TIME  NTICKS_PER_SECOND*10 // 10 seconds

/*****************************************************************
 * Helpers
 ****************************************************************/
#define _BV(bit_no) (1 << bit_no)
#define forever while (1)

/*****************************************************************
 * Settable Config Globals
 ****************************************************************/
struct GlobalDeviceConfigurationStruct {
	uint16_t CheckSleepIntervalMS;
	uint16_t BLECheckInterval;
	uint16_t CheckReadDelayMS;
	uint16_t MaxTransmitStateTimeS;
};

extern struct GlobalDeviceConfigurationStruct SB_GlobalDeviceConfiguration;

#endif /* APPLICATION_CONFIG_H_ */
