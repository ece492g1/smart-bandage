/*
 * config.c
 *
 *  Created on: Feb 9, 2016
 *      Author: michaelblouin
 */

#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>

#include <driverlib/ioc.h>
#include "Board.h"

struct GlobalDeviceConfigurationStruct SB_GlobalDeviceConfiguration = {
	.CheckSleepIntervalMS = 10000,
	.BLECheckInterval = 10,
};

/*
 *  ============================= PIN configs===================================
*/
PIN_Config BoardGpioInitTable[] = {
	// General IO
	Board_PERIPHERAL_PWR | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_MP_EN_SW		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_MSW_0			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_MSW_1			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_MSW_2			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_MPSW			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,

	// Analog Pins
	Board_BANDAGE_A_0    | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_CONN_STATE_RD  | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_VSENSE_0       | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_VSENSE_1		 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_1V3			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,

	// I2C
	Board_SDA 			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	Board_SCL 			 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
	PIN_TERMINATE
};

/*
 *  ============================= MCP9808 configs===============================
*/
uint8_t Mcp9808Addresses[] = {
	I2C_SENSOR_TEMP0_ADDR,
	I2C_SENSOR_TEMP1_ADDR,
	I2C_SENSOR_TEMP2_ADDR
};

/*
 *  ============================= I2C Begin=====================================
*/

/* This configuration is used to compile the bluetooth drivers for the RTOS  */
/* Location is in the .c file so that these variables are useable elsewhere. */

/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(I2C_config, ".const:I2C_config")
#pragma DATA_SECTION(i2cCC26xxHWAttrs, ".const:i2cCC26xxHWAttrs")
#endif

/* Include drivers */
#include <ti/drivers/i2c/I2CCC26XX.h>

/* I2C objects */
I2CCC26XX_Object i2cCC26xxObjects[CC2650_I2CCOUNT];

/* I2C configuration structure, describing which pins are to be used */
const I2CCC26XX_HWAttrs i2cCC26xxHWAttrs[CC2650_I2CCOUNT] = {
    {
        .baseAddr = I2C0_BASE,
        .intNum = INT_I2C,
        .powerMngrId = PERIPH_I2C0,
        .sdaPin = Board_I2C0_SDA0,
        .sclPin = Board_I2C0_SCL0,
    }
};

const I2C_Config I2C_config[] = {
    {&I2CCC26XX_fxnTable, &i2cCC26xxObjects[0], &i2cCC26xxHWAttrs[0]},
    {NULL, NULL, NULL}
};
/*
 *  ========================== I2C end =========================================
*/
