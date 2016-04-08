/*
 * clock.h
 *
 *  Created on: Mar 21, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_CLOCK_H_
#define APPLICATION_CLOCK_H_

#include "hci_tl.h"

/*********************************************************************
 * @fn      SB_clockInit
 *
 * @brief   Initializes the clock module
 *
 * @return  NoError if properly initialized, otherwise the error that occured
 */
void SB_clockInit();

/*********************************************************************
 * @fn      SB_clockIsSet
 *
 * @brief   Checks if the clock is set
 *
 * @return  True if the clock has been set
 */
bool SB_clockIsSet();

/*********************************************************************
 * @fn      SB_clockSetTime
 *
 * @brief   Sets the current time
 */
void SB_clockSetTime(uint32_t);

/*********************************************************************
 * @fn      SB_clockGetTime
 *
 * @brief   Gets the current time
 *
 * @return  The current time
 */
uint32_t SB_clockGetTime();

#endif /* APPLICATION_CLOCK_H_ */
