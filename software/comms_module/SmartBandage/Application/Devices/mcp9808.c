/*
 * mcp9808.c
 *
 *  Created on: Feb 17, 2016
 *      Author: michaelblouin
 */

#include "mcp9808.h"

int16_t mcp9808_convert_raw_temp_data(uint8_t upperByte, uint8_t lowerByte) {
	if (upperByte & 0x10) {
		// Negative temperature
		return 256 - (upperByte & 0x0F)*16 + lowerByte/16;
	} else {
		return (upperByte & 0x0F) << 8 | lowerByte;
	}
}
