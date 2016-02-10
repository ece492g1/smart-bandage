/*
 * @file config.h
 * @brief Contains configuration variables for the application.
 *
 *  Created on: Feb 9, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_CONFIG_H_
#define APPLICATION_CONFIG_H_

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

/*****************************************************************
 * External MUX configurations
 ****************************************************************/
#include "mux.h"

#define Board_IOMUX_S0 					Board_MSW_0
#define Board_IOMUX_S1 					Board_MSW_1
#define Board_IOMUX_S2 					Board_MSW_2
#define Board_IOMUX_BANDAGE_A_0			((MUX_SELECT)Y3)
#define Board_IOMUX_BANDAGE_A_1			((MUX_SELECT)Y2)
#define Board_IOMUX_BANDAGE_A_2			((MUX_SELECT)Y1)
#define Board_IOMUX_BANDAGE_A_3			((MUX_SELECT)Y0)
#define Board_IOMUX_BANDAGE_A_4			((MUX_SELECT)Y4)
#define Board_IOMUX_PERIPHERAL_DETECT	((MUX_SELECT)Y5)
#define Board_IOMUX_SYSDISBL_N			((MUX_SELECT)Y6)
#define Board_IOMUX_V_PREBUCK_DIV2		((MUX_SELECT)Y7)

#define Board_PWRMUX_S 							Board_MPSW
#define Board_PWRMUX_ENABLE_N 					Board_MP_EN_SW
#define Board_PWRMUX_PERIPHERAL_VCC				((MUX_SELECT)Y1)
#define Board_PWRMUX_1V3						((MUX_SELECT)Y0)

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

#endif /* APPLICATION_CONFIG_H_ */
