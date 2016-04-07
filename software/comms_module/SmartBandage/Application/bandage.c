/*
 * bandage.c

 *
 *  Created on: Mar 20, 2016
 *      Author: michaelblouin
 */

#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/PIN.h>
#include <driverlib/aux_adc.h>
#include <driverlib/aux_wuc.h>
#include <inc/hw_aux_evctl.h>

#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/BIOS.h>

#include <ti/drivers/PIN/PINCC26XX.h>
#include <ti/drivers/UART.h>

#include <driverlib/aux_adc.h>
#include <driverlib/aux_wuc.h>
#include <inc/hw_aux_evctl.h>


#include "bandage.h"
#include "peripheralManager.h"

#define READINGS_AVAIL_WAIT_TIME_MS 10

struct {
	Semaphore_Handle notifySem;
	Semaphore_Handle adcSem;
	Hwi_Struct hwi;
	SB_Error lastError : 4;
	bool readInProgress : 1;
	bool currentReadComplete : 1;
	volatile bool isrReadComplete : 1;
	SB_MoistureSensorLine currentReading : 3;
	uint16_t (*readings)[SB_NUM_MOISTURE];
} bandage;

SB_Error processNextReading(uint32_t timeout);
SB_Error _processNextReading(uint32_t timeout);
void adcIsr(UArg a0);

SB_Error SB_bandageInit(Semaphore_Handle readSem) {
	bandage.currentReading = BANDAGE_A_0;
	bandage.readInProgress = false;
	bandage.notifySem = readSem;
	bandage.lastError = NoError;

	if (NULL == readSem) {
		return InvalidParameter;
	}

	bandage.adcSem = Semaphore_create(1, NULL, NULL);
	if (NULL == bandage.adcSem) {
		return OSResourceInitializationError;
	}

	// Construct the hardware interrupt for the ADC
	Hwi_Params hwiParams;
	Hwi_Params_init(&hwiParams);
	hwiParams.enableInt = true;

	Error_Block eb;
	Error_init(&eb);

	Hwi_construct(&bandage.hwi, INT_AUX_ADC, adcIsr, &hwiParams, &eb);

	if (Error_check(&eb)) {
		return OSResourceInitializationError;
	}

	return NoError;
}

SB_Error SB_beginReadBandageImpedances(uint32_t timeout, uint16_t (*readingsBuf)[SB_NUM_MOISTURE]) {
	if (NULL == bandage.adcSem) {
		return OSResourceInitializationError;
	} else if (NULL == readingsBuf) {
		return InvalidParameter;
	}

	if (!Semaphore_pend(bandage.adcSem, timeout)) {
		return OperationTimeout;
	}

	if (bandage.readInProgress) {
		return ResourceBusy;
	} else if (NULL == bandage.notifySem) {
		return ResourceNotInitialized;
	}

	bandage.currentReading = BANDAGE_A_0;
	bandage.readInProgress = false;
	bandage.lastError = NoError;
	bandage.readings = readingsBuf;

	AUXWUCClockEnable(AUX_WUC_ADC_CLOCK | AUX_WUC_ADI_CLOCK | AUX_WUC_SOC_CLOCK |
			AUX_WUC_SMPH_CLOCK);//AUX_WUC_MODCLKEN0_SOC_M | AUX_WUC_MODCLKEN0_AUX_ADI4_M);
//	AUXWUCClockEnable(AUX_WUC_MODCLKEN0_AUX_DDI0_OSC_M|AUX_WUC_MODCLKEN0_AUX_ADI4_M);
//	AUXWUCClockFreqReq(AUX_WUC_CLOCK_HIFREQ);

	while (AUXWUCClockStatus(AUX_WUC_ADC_CLOCK) != AUX_WUC_CLOCK_READY);
	while (AUXWUCClockStatus(AUX_WUC_ADI_CLOCK) != AUX_WUC_CLOCK_READY);

	// Configure and enable the ADC using the fixed internal reference, manual triggers, and
	// the default sample time for the SmartBandage
	AUXADCEnableSync(AUXADC_REF_FIXED, SB_ADC_SAMPLE_TIME, AUXADC_TRIGGER_MANUAL);

	// Disallow STANDBY mode while using the ADC.
	Power_setConstraint(Power_SB_DISALLOW);
	Task_sleep(100 * 1000 / Clock_tickPeriod);
	// Start reading
	SB_Error result = _processNextReading(timeout);

	Semaphore_post(bandage.adcSem);

	return result;
}

SB_Error _readingComplete() {
	// Disable ADC
	AUXADCDisable();

	// Disable clocks
	AUXWUCClockDisable(AUX_WUC_ADC_CLOCK | AUX_WUC_ADI_CLOCK);

	// Allow STANDBY mode again
	Power_releaseConstraint(Power_SB_DISALLOW);

	// Restore pins to values in BoardGpioTable
//	PIN_close(pinHandle);

	return NoError;
}

SB_Error SB_processBandageReadings(uint32_t timeout) {
	if (SB_NumMoistureSensorLine == bandage.currentReading || !bandage.readInProgress || !bandage.currentReadComplete) {
		return NoError;
	}

	return processNextReading(timeout);
}

SB_Error processNextReading(uint32_t timeout) {
	SB_Error error;

	if (!Semaphore_pend(bandage.adcSem, timeout)) {
		return OperationTimeout;
	}

	error = _processNextReading(timeout);

	if (false == bandage.readInProgress) {
		_readingComplete();
	}

	Semaphore_post(bandage.adcSem);

	return error;
}

SB_Error _processNextReading(uint32_t timeout) {
	SB_Error error;

	if (bandage.readInProgress && !bandage.currentReadComplete && !bandage.isrReadComplete) {
		return NoError;
	}

	// Mark the current reading complete, and exit if all readings have been performed
	bandage.currentReadComplete = bandage.isrReadComplete;
	bandage.isrReadComplete = false;

	if (!bandage.readInProgress) {
		// This is the first read
		bandage.currentReading = BANDAGE_A_0;
		bandage.readInProgress = true;
	} else if (SB_NumMoistureSensorLine == ++bandage.currentReading) {
		bandage.readInProgress = false;

		if (NULL != bandage.notifySem) {
			Semaphore_post(bandage.notifySem);
		}

		return NoError;
	}

	// Try and configure the next reading
	AUXADCSelectInput(SB_AN_VSENSE_1);
	if (NoError == (error = SB_selectMoistureSensorInput(bandage.currentReading, MOISTURE_V_1V3, timeout))) {
		// Mux configured. Wait for input to stabilize and trigger a conversion
		Task_sleep(NTICKS_PER_MILLSECOND*10);
		bandage.currentReadComplete = false;
		AUXADCGenManualTrigger();
		return NoError;
	}

	return error;
}

SB_Error waitForReadingsAvailable() {
	SB_Error error;
	while (!Semaphore_pend(bandage.notifySem, READINGS_AVAIL_WAIT_TIME_MS)) {
		if (NoError != (error = processNextReading(READINGS_AVAIL_WAIT_TIME_MS))) {
			return error;
		}
	}

	return NoError;
}

void adcIsr(UArg a0) {
	// Pop sample from FIFO to allow clearing ADC_IRQ event
	(*bandage.readings)[bandage.currentReading] = (uint16_t)(AUXADCPopFifo()*43L*16L/(4096L));
//	(*bandage.readings)[bandage.currentReading] = (uint16_t)(AUXADCPopFifo());

	// Clear ADC_IRQ flag
	HWREGBITW(AUX_EVCTL_BASE + AUX_EVCTL_O_EVTOMCUFLAGSCLR, AUX_EVCTL_EVTOMCUFLAGSCLR_ADC_IRQ_BITN) = 1;

	// Process the next error if all resources available
	bandage.isrReadComplete = true;
//	bandage.lastError = processNextReading(BIOS_NO_WAIT);
}
