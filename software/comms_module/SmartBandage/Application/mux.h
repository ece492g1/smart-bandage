/*
 * @file mux.h
 * @brief Contains external MUX interface definitions.
 *
 *  Created on: Feb 9, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_MUX_H_
#define APPLICATION_MUX_H_

typedef enum {
	S0 = 0b001,
	S1 = 0b010,
	S2 = 0b100,
} MUX_SELECT;

typedef enum {
	Y0 = 0,
	Y1 = 1,
	Y2 = 2,
	Y3 = 3,
	Y4 = 4,
	Y5 = 5,
	Y6 = 6,
	Y7 = 7,
} MUX_OUTPUT;

typedef enum {
	MUX_ENABLE,
	MUX_DISABLE
} MUX_OUTPUT_ENABLE;

// Gets the value for MUX_SELECT input `muxSelect` to enable output pin `output`
#define MUX_SELECT_VALUE(muxSelect, output) ((muxSelect & output) >> (muxSelect - S0))

#endif /* APPLICATION_MUX_H_ */
