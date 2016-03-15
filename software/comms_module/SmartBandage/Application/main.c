/**
  @file  main.c
  @brief Entry point of the application
*/
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Error.h>

#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/PIN.h>

#include "ICall.h"
#include "bcomdef.h"
#include "peripheral.h"
#include "i2c.h"
#include "peripheralManager.h"
#include "Board.h"
//#include "ble.h"
#include "flash.h"

/* Header files required to enable instruction fetch cache */
#include <inc/hw_memmap.h>
#include <driverlib/vims.h>

#ifndef USE_DEFAULT_USER_CFG

#include "../ICall/bleUserConfig.h"

// BLE user defined configuration
bleUserCfg_t user0Cfg = BLE_USER_CFG;

#endif // USE_DEFAULT_USER_CFG

/*
 *  ======== main ========
 */
int main()
{
	SB_Error error;

	PIN_init(BoardGpioInitTable);

#ifndef POWER_SAVING
    /* Set constraints for Standby, powerdown and idle mode */
    Power_setConstraint(Power_SB_DISALLOW);
    Power_setConstraint(Power_IDLE_PD_DISALLOW);
#endif // POWER_SAVING
    
    /* Initialize ICall module */
    ICall_init();
#ifdef SB_DEBUG
    System_printf("ICall Initialized.\n");
	System_flush();
#endif

    /* Start tasks of external images - Priority 5 */
    ICall_createRemoteTasks();
#ifdef SB_DEBUG
	System_printf("ICall Remote Tasks Initialized.\n");
	System_flush();

#endif

    /* Kick off profile - Priority 3 */
    GAPRole_createTask();
#ifdef SB_DEBUG
	System_printf("GAP Role Initialized.\n");
	System_flush();
#endif

/* enable I2C */
	if (NoError != (error = SB_i2cInit((I2C_BitRate) I2C_BITRATE))) {
#ifdef SB_DEBUG
		System_printf("Error No: %d\n", error);
		System_printf("SB application initialization failed while initializing I2C. This is a code error.\n");
		System_flush();
#endif

		while(1);
	}

#ifdef SB_DEBUG
	System_printf("I2C Initialized.\n");
	System_flush();
#endif

	/* enable peripheral manager after I2C */
	if (NoError != (error = SB_peripheralInit())) {
#ifdef SB_DEBUG
		System_printf("Error No: %d\n", error);
		System_printf("SB application initialization failed while initializing peripherals. This is a code error.\n");
		System_flush();
#endif

		while(1);
	}

#ifdef SB_DEBUG
	System_printf("Peripherals Initialized.\n");
	System_flush();
#endif

//	if (NoError != (error = SB_flashInit(sizeof(SB_PeripheralReadings), SB_REINIT_FLASH_ON_START))) {
//#ifdef SB_DEBUG
//		System_printf("Error No: %d\n", error);
//		System_printf("SB application initialization failed while initializing non-volatile flash storage. This is a code error.\n");
//		System_flush();
//#endif
//
//		while(1);
//	}
//
//#ifdef SB_DEBUG
//	System_printf("Non-Volatile Memory Initialized.\n");
//	System_flush();
//#endif

	/* Configure BLE */
//	SB_bleInit();
//#ifdef SB_DEBUG
//	System_printf("SB BLE Initialized.\n");
//	System_flush();
//#endif

	/* enable interrupts and start SYS/BIOS */
#ifdef SB_DEBUG
    System_printf("Smart Bandage Initialized.\n");
	System_flush();
#endif

    BIOS_start();

    return 0;
}
//
//#pragma optimize=none
//void exceptionHandler(struct exceptionFrame *e, unsigned int execLr) {
//	static unsigned int failPC = 0;
//	static unsigned int lr = 0;
//	failPC = e->_pc; // This is the Program Counter when the exception happened
//	lr = execLR; // The exception LR
//	while (1);
//}
