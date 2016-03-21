/*
 * bandage.h
 *
 *  Created on: Mar 20, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_BANDAGE_H_
#define APPLICATION_BANDAGE_H_

#include "Board.h"

SB_Error SB_processBandageReadings(uint32_t timeout);
SB_Error SB_bandageInit(Semaphore_Handle readSem);
SB_Error SB_beginReadBandageImpedances(uint32_t timeout, uint16_t (*readingsBuf)[SB_NUM_MOISTURE]);
SB_Error waitForReadingsAvailable();

#endif /* APPLICATION_BANDAGE_H_ */
