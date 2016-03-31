/*
 * clock.h
 *
 *  Created on: Mar 21, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_CLOCK_H_
#define APPLICATION_CLOCK_H_

#include "hci_tl.h"

void SB_clockInit();
bool SB_clockIsSet();
void SB_clockSetTime(uint32_t);
uint32_t SB_clockGetTime();

#endif /* APPLICATION_CLOCK_H_ */
