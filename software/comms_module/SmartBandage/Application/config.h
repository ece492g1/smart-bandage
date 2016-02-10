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
 * I2C Configuration
 ****************************************************************/
#define I2C_BITRATE    1 		// 0 = 100kHz, 1 = 400kHz
#define I2C_PERIPHERAL 1

#ifndef I2C_BITRATE
# error "I2C_BITRATE must be assigned a value."
#elif I2C_BITRATE < 0
# error "I2C_BITRATE has invalid value: Must be 0 or 1."
#elif I2C_BITRATE > 1
# error "I2C_BITRATE has invalid value: Must be 0 or 1."
#endif

/* I2C */
#define Board_I2C0_SDA0             IOID_5
#define Board_I2C0_SCL0             IOID_6
#define Board_I2C0_SDA1             IOID_8
#define Board_I2C0_SCL1             IOID_9

/* Generic I2C instance identifiers */
#define Board_I2C                   CC2650_I2C0

/*!
 *  @def    CC2650_I2CName
 *  @brief  Enum of I2C names on the CC2650 dev board
 */
typedef enum CC2650_I2CName {
    CC2650_I2C0 = 0,
    CC2650_I2CCOUNT
} CC2650_I2CName;


#endif /* APPLICATION_CONFIG_H_ */
