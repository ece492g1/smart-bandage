/*
 * @file i2c.h
 * @brief Contains application convenience functions for working with I2C
 *
 *  Created on: Feb 9, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_I2C_H_
#define APPLICATION_I2C_H_

#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC26XX.h>
#include <ti/sysbios/knl/Semaphore.h>
#include "Board.h"

typedef struct {
	I2C_Transaction* baseTransaction;
	Semaphore_Handle* completionSemaphore;
	SB_Error completionResult;
} SB_i2cTransaction;

SB_Error SB_i2cQueueTransaction(SB_i2cTransaction* transaction, uint32_t timeout);
SB_Error SB_i2cInit(I2C_BitRate bitRate);
void SB_i2cSleep();

#endif /* APPLICATION_I2C_H_ */
