/*
 * bandage.h
 *
 *  Created on: Mar 20, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_BANDAGE_H_
#define APPLICATION_BANDAGE_H_

#include "Board.h"

/*********************************************************************
 * @fn      SB_processBandageReadings
 *
 * @brief   Processes any completed bandage readings
 */
SB_Error SB_processBandageReadings(uint32_t timeout);

/*********************************************************************
 * @fn      SB_bandageInit
 *
 * @brief   Initializes the bandage module with the given semaphore
 */
SB_Error SB_bandageInit(Semaphore_Handle readSem);

/*********************************************************************
 * @fn      SB_beginReadBandageImpedances
 *
 * @brief   Begins reading all bandage readings by starting the async read process
 */
SB_Error SB_beginReadBandageImpedances(uint32_t timeout, uint16_t (*readingsBuf)[SB_NUM_MOISTURE]);

/*********************************************************************
 * @fn      waitForReadingsAvailable
 *
 * @brief   Waits for final readings to be available
 */
SB_Error SB_waitForReadingsAvailable();

#endif /* APPLICATION_BANDAGE_H_ */
