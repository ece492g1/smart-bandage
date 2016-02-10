/*
 * i2c.c
 *
 *  Created on: Feb 9, 2016
 *      Author: michaelblouin
 */

#include "i2c.h"
#include "config.h"

I2C_Handle handle;

void SB_i2cTransferCompleteHandler(I2C_Handle handle, I2C_Transaction *transac, bool result);

void SB_i2cInit(I2C_BitRate bitRate) {
	I2C_Params params;
	params.bitRate = bitRate;
	params.transferMode = I2C_MODE_CALLBACK;
	params.transferCallbackFxn = SB_i2cTransferCompleteHandler;

	// Configure I2C parameters.
	I2C_Params_init(&params);

	// Open I2C
	handle = I2C_open(Board_I2C, &params);
}

void SB_i2cSleep() {

}

void SB_i2cTransferCompleteHandler(I2C_Handle handle, I2C_Transaction *transac, bool result) {
	// Handle the transfer completion event.
	// Post to the semaphore that the requesting task is waiting on.
	// Queue the next transfer
}
