// Outline for finite state machine for MCU

/* 
states:
    sleep mode
    transmit mode
    check mode
    init mode
    temp error
    perm error
    
events:
    check timer expires
    ble timer expires
    data change (alert)
    no bandage detected
    error occurs
 */

 #include "fsm.h"

//function prototypes
SB_State SB_checkTimerExpired(void);
SB_State SB_bleTimerExpired(void);
SB_State SB_noBandageDetected(void);
SB_State SB_errorEvent(); //a more permanent error caught
SB_State SB_dataChanged(void);
SB_State SB_returnToSleep(void);
void SB_setError(SB_Error);

//EDITED HERE
//callback function prototypes -- should these be pointers themselves?
void SB_enterSleepCallback(void);
void SB_transmitCallback(void);
void SB_checkCallback(void);
void SB_initSystemCallback(void);
void SB_tempErrorCallback(void);
void SB_permErrorCallback(void);
void SB_registerStateTransitionCallback(SB_Callback, SB_State);


//LUT TABLE
//TODO: If we continue to run out of heap, implement this in a switch statement as with 5 events and 6 states this is 120bytes of memory
//implemented a "do nothing" function for the events that are not relevent to a particular
SB_State (*SB_stateTable [SB_NUM_STATES][SB_NUM_EVENTS]) (void) = {
    {SB_checkTimerExpired, SB_bleTimerExpired, SB_returnToSleep, SB_noBandageDetected, SB_errorEvent }, //procedures for state sleep
    {SB_checkTimerExpired, SB_bleTimerExpired, SB_dataChanged, SB_noBandageDetected, SB_errorEvent },//procedures for state check
    {SB_returnToSleep, SB_bleTimerExpired, SB_returnToSleep, SB_noBandageDetected, SB_errorEvent}, //procedure for transmit
    {SB_checkTimerExpired, SB_bleTimerExpired, SB_dataChanged, SB_noBandageDetected, SB_errorEvent}, //procedures for state error temp
    {SB_returnToSleep, SB_bleTimerExpired, SB_returnToSleep, SB_returnToSleep, SB_errorEvent}, //procedures for state error perm
    {SB_returnToSleep, SB_bleTimerExpired, SB_dataChanged, SB_noBandageDetected, SB_errorEvent} //procedures for initialize state
};

//local variables
SB_SystemState systemState = {
	.currentState = S_INIT,
	.lastState    = S_INIT,
	.currentError = NoError,
	.lastError    = NoError,
};

//EDITED HERE
//switch the states
SB_State SB_switchState(SB_State newState) {
	switch (newState) {
	case S_INIT:
		SB_registerStateTransitionCallback(SB_initSystemCallback, newState);
	case S_SLEEP:
		SB_registerStateTransitionCallback(SB_enterSleepCallback, newState);
	case S_CHECK:
		SB_registerStateTransitionCallback(SB_checkCallback, newState);
	case S_TRANSMIT:
		SB_setError(NoError);
		// No "break" on purpose
		SB_registerStateTransitionCallback(SB_transmitCallback, newState);
	case S_ERROR_TEMP:
		SB_registerStateTransitionCallback(SB_tempErrorCallback, newState);
		systemState.currentState = newState;
		break;

	default:
		SB_registerStateTransitionCallback(SB_permErrorCallback, newState);
		systemState.currentState = S_ERROR_PERM;
	}

	return systemState.currentState;
}

//action procedures here for now, also might want to put into another file later
SB_State SB_checkTimerExpired(void){
    //the timer expired, 
	return SB_switchState(S_CHECK);
    //check data from periperal sensors and store it into a register
    
    //reset the timer
    
    //check new data with previous data (if it exists), if it is outside expected range, send an alert, if not, return to sleep
}
SB_State SB_bleTimerExpired(void){
    //the transmit data timer expired
	return SB_switchState(S_TRANSMIT);
    //send the data via the bluetooth to the smartphone android application
    //check if successful - remove previous data(?) and return to sleep
    //if not, reset timer, and save previous data
}
SB_State SB_noBandageDetected(void){
    //no connection between the communications module and the bandage module detected
    //set the current state to temp error and wait for check timer to expire to see if initialized (?)
	return SB_switchState(S_ERROR_TEMP);
}
SB_State SB_errorEvent(){
    //a more permanent error has occurred, do nothing or else relay problem via bluetooth

	//permanent errors
	switch (systemState.currentError) {
	case OSResourceInitializationError:
	case OutOfMemory:
		return SB_switchState(S_ERROR_PERM);

	// All other errors implied temporary
	default:
		return SB_switchState(S_ERROR_TEMP);
	}
}
SB_State SB_dataChanged(void){
    //data changed prior to the ble_timer expiring
	return SB_switchState(S_TRANSMIT);
}

//not actually an event, just something that happens when there isn't anything else to do
SB_State SB_returnToSleep(void){
	return SB_switchState(S_SLEEP);
}

SB_State SB_handleEvent(SB_Event event) {
	// State or event invalid
	if (systemState.currentState >= SB_NUM_STATES || event >= SB_NUM_EVENTS) {
		SB_setError(InvalidParameter);
		return SB_errorEvent();
	}

	// No action if the pointer is null
	if (SB_stateTable[systemState.currentState][event] == NULL) {
		return systemState.currentState;
	}

	return SB_stateTable[systemState.currentState][event]();
}

inline void SB_setError(SB_Error error) {
	systemState.lastError = systemState.currentError;
	systemState.currentError = error;
}

inline SB_State SB_currentState() {
	return systemState.currentState;
}

//EDITED HERE
void SB_enterSleepCallback() {
	//Handle entering the sleep state. Turn off peripherals to reduce power consumption
}

void SB_transmitCallback() {
	//Handle entering the transmit state. Turn on the bluetooth peripheral

}

void SB_checkCallback() {
	//Handle entering the check state. Turn on all peripherals except for the bluetooth

}

void SB_initSystemCallback() {
	//Handle system initialization

}

void SB_tempErrorCallback(){
	//Handle temporary errors, possibly reset peripherals, depends on error

}

void SB_permErrorCallback() {
	//Handle permanent error. Possibly turn on bluetooth peripheral if transmitting error

}

void SB_registerStateTransitionCallback(SB_Callback callback, SB_State state) {
	//passes function pointer to be called in the future
	(*callback)();
}
