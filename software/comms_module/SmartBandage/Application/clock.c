/*
 * clock.c
 *
 *  Created on: Mar 21, 2016
 *      Author: michaelblouin
 */

#include <ti/sysbios/knl/Clock.h>

#include "Board.h"

struct {
	uint32_t lastTimestamp;
	uint32_t lastReferenceTicks;
	bool isSet : 1;
} clock;

/*********************************************************************
 * @fn      SB_clockIsSet
 *
 * @brief   Checks if the clock is set
 *
 * @return  True if the clock has been set
 */
inline bool SB_clockIsSet() {
	return clock.isSet;
}

/*********************************************************************
 * @fn      SB_clockInit
 *
 * @brief   Initializes the clock module
 *
 * @return  NoError if properly initialized, otherwise the error that occured
 */
void SB_clockInit() {
	clock.isSet = false;
	clock.lastTimestamp = 0;
	clock.lastReferenceTicks = Clock_getTicks();
}

/*********************************************************************
 * @fn      SB_clockSetTime
 *
 * @brief   Sets the current time
 */
void SB_clockSetTime(uint32_t timestamp) {
	clock.lastTimestamp = timestamp;
	clock.lastReferenceTicks = Clock_getTicks();
	clock.isSet = true;
}

/*********************************************************************
 * @fn      SB_clockGetTime
 *
 * @brief   Gets the current time
 *
 * @return  The current time
 */
uint32_t SB_clockGetTime() {
	uint32_t referenceTicks = Clock_getTicks();
	clock.lastTimestamp += (referenceTicks - clock.lastReferenceTicks)/NTICKS_PER_SECOND;
	clock.lastReferenceTicks = referenceTicks;
	return clock.lastTimestamp;
}
