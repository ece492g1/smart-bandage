/*
 * @file mcp9808.h
 * @brief Contains definitions for the Microchip MCP9808 temperature sensor.
 *
 *  Created on: Feb 10, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_DEVICES_MCP9808_H_
#define APPLICATION_DEVICES_MCP9808_H_

#define MCP9808_REG_CONFIG          0x01
#define MCP9808_REG_TUPPER          0x02
#define MCP9808_REG_TLOWER          0x03
#define MCP9808_REG_TCRIT           0x04
#define MCP9808_REG_TA              0x05
#define MCP9808_REG_MANUFACTURER_ID 0x06
#define MCP9808_REG_DEVICE_ID       0x07
#define MCP9808_REG_RESOLUTION      0x07

#define MCP9808_TEMP_REG_MASK       0x1FFF
#define MCP9808_TEMP_REG_MASK_UPPER (MCP9808_TEMP_REG_MASK & 0xFF00)
#define MCP9808_TEMP_REG_MASK_LOWER (MCP9808_TEMP_REG_MASK & 0x00FF)

#define MCP9808_TEMP_LIM_REG_MASK       0x1FFC
#define MCP9808_TEMP_LIM_REG_MASK_UPPER (MCP9808_TEMP_LIM_REG_MASK & 0xFF00)
#define MCP9808_TEMP_LIM_REG_MASK_LOWER (MCP9808_TEMP_LIM_REG_MASK & 0x00FF)

#define MCP9808_CONF_REG_MASK       0x01FF
#define MCP9808_CONF_REG_MASK_UPPER (MCP9808_CONF_REG_MASK & 0xFF00)
#define MCP9808_CONF_REG_MASK_LOWER (MCP9808_CONF_REG_MASK & 0x00FF)

#define MCP9808_MANUFACTURER_ID     0x0054
#define MCP9808_MIN_DEVICE_ID       0x0400

#define MCP9808_RESOLUTION_0P5      0b00
#define MCP9808_RESOLUTION_0P25     0b01
#define MCP9808_RESOLUTION_0P125    0b10
#define MCP9808_RESOLUTION_0P0625   0b11

#define MCP9808_RES_TCONV_MS_0P5     30
#define MCP9808_RES_TCONV_MS_0P25    65
#define MCP9808_RES_TCONV_MS_0P125  130
#define MCP9808_RES_TCONV_MS_0P0625 250

#define MCP9808_CONFIG_ALERT_MODE      0
#define MCP9808_CONFIG_ALERT_POLARITY  1
#define MCP9808_CONFIG_ALERT_SELECT    2
#define MCP9808_CONFIG_ALERT_CONTROL   3
#define MCP9808_CONFIG_ALERT_STATUS    4
#define MCP9808_CONFIG_INT_CLR         5
#define MCP9808_CONFIG_WIN_LOCK        6
#define MCP9808_CONFIG_CRIT_LOCK       7
#define MCP9808_CONFIG_SHDN            8
#define MCP9808_CONFIG_THYST           9

#define MCP9808_ALERT_COMPARATOR 0
#define MCP9808_ALERT_INTERRUPT  1

#define MCP9808_ALERT_ALL_SOURCES 0
#define MCP9808_ALERT_TCRIT_ONLY  1

#define MCP9808_OUTPUT_ACTIVE_LOW  0
#define MCP9808_OUTPUT_ACTIVE_HIGH 1

#define MCP9808_ALERT_ENABLE  1
#define MCP9808_ALERT_DISABLE 0

#define MCP9808_ALERT_STATUS_NOT_ASSERTED 0
#define MCP9808_ALERT_STATUS_ASSERTED     1

#define MCP9808_OK 0

typedef enum {
	THYST_0C,
	THYST_1C5,
	THYST_3C,
	THYST_6C,
} MCP9808_THYST;

typedef enum {
	MCP9808_RUNNING = 0,
	MCP9808_SHUTDOWN = 1,
} MCP9808_SHUTDOWN_STATE;

typedef struct {
	int8_t   Address;
	int16_t  ManufacturerId;
	int16_t  DeviceId;
	uint16_t Configuration;
	uint16_t Resolution;
	int16_t  Temperature;
} MCP9808_DEVICE;

int16_t mcp9808_convert_raw_temp_data(uint8_t upperByte, uint8_t lowerByte) {
	if (upperByte & 0x10) {
		// Negative temperature
		return 256 - (upperByte & 0x0F)*16 + lowerByte/16;
	} else {
		return (upperByte & 0x0F) << 8 | lowerByte;
	}
}

#endif /* APPLICATION_DEVICES_MCP9808_H_ */
