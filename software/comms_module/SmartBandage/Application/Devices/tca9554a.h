/*
 * tca9554a.h IO Expander
 *
 *  Created on: Feb 21, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_DEVICES_TCA9554A_H_
#define APPLICATION_DEVICES_TCA9554A_H_

#include "hci_tl.h"
#include "../Board.h"
#include <ti/sysbios/knl/Semaphore.h>

#define TCA9554A_REG_INPUT    0
#define TCA9554A_REG_OUTPUT   1
#define TCA9554A_REG_POLARITY 2
#define TCA9554A_REG_CONFIG   3

#define TCA9554A_OUTPUT_HIGH  1
#define TCA9554A_OUTPUT_LOW   0

#define TCA9554A_POLARITY_NORMAL    0
#define TCA9554A_POLARITY_INVERTED  1

#define TCA9554A_CONFIG_INPUT  1
#define TCA9554A_CONFIG_OUTPUT 0

typedef enum {
	IOPORT0,
	IOPORT1,
	IOPORT2,
	IOPORT3,
	IOPORT4,
	IOPORT5,
	IOPORT6,
	IOPORT7,
} TCA9554A_IO_PORT;

typedef struct {
	uint8 inputReg;
	uint8 outputReg;
	uint8 polarityReg;
	uint8 configuration;
	uint8 address;
} TCA9554A_DEVICE;

SB_Error tca9554a_writePinStatus(TCA9554A_DEVICE *device, Semaphore_Handle *semaphore);
SB_Error tca9554a_setPinStatus(TCA9554A_DEVICE *device, Semaphore_Handle *semaphore, TCA9554A_IO_PORT pin, bool status);

#endif /* APPLICATION_DEVICES_TCA9554A_H_ */
