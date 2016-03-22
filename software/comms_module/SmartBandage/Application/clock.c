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

inline bool SB_clockIsSet() {
	return clock.isSet;
}

void SB_clockInit() {
	clock.isSet = false;
	clock.lastTimestamp = 0;
	clock.lastReferenceTicks = Clock_getTicks();
}

void SB_clockSetTime(uint32_t timestamp) {
	clock.lastTimestamp = timestamp;
	clock.lastReferenceTicks = Clock_getTicks();
	clock.isSet = true;
}

uint32_t SB_clockGetTime() {
	uint32_t referenceTicks = Clock_getTicks();
	clock.lastTimestamp += (referenceTicks - clock.lastReferenceTicks)/NTICKS_PER_SECOND;
	clock.lastReferenceTicks = referenceTicks;
	return clock.lastTimestamp;
}
