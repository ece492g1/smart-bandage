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
#include <ti/sysbios/knl/Task.h>
//function prototypes
SB_State SB_checkTimerExpired(void);
SB_State SB_bleTimerExpired(void);
SB_State SB_noBandageDetected(void);
SB_State SB_errorEvent(); //a more permanent error caught
SB_State SB_dataChanged(void);
SB_State SB_returnToSleep(void);
void SB_setError(SB_Error);
void SB_callCallback(SB_State_Transition transition, SB_State state);

struct {
	SB_CallbackFunc *callbacks;
} callbackTable;

//local variables
SB_SystemState systemState = {
	.currentState = S_INIT,
	.lastState    = S_INIT,
	.currentError = NoError,
	.lastError    = NoError,
};

/* ASSUMPTIONS:
 *  - the tasks will have been previously created, and in the function SB_switchState we are using the tasks, but not creating any new ones
 */
SB_State SB_switchState(SB_State newState) {
	SB_State oldState = systemState.currentState;
	SB_callCallback(T_STATE_PRE_EXIT, systemState.currentState);

	switch (newState) {
	case S_INIT:

	case S_SLEEP:

	case S_CHECK:

	case S_TRANSMIT:
		SB_setError(NoError);
		// No "break" on purpose

	case S_ERROR_TEMP:
		SB_callCallback(T_STATE_PRE_ENTER, newState);
		systemState.currentState = newState;
		break;

	default:
		SB_callCallback(T_STATE_PRE_ENTER, S_ERROR_PERM);
		systemState.currentState = S_ERROR_PERM;
	}

	SB_callCallback(T_STATE_EXIT, oldState);
	SB_callCallback(T_STATE_ENTER, systemState.currentState);

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

inline void SB_setError(SB_Error error) {
	systemState.lastError = systemState.currentError;
	systemState.currentError = error;
}

inline SB_State SB_currentState() {
	return systemState.currentState;
}

// Called from within periheral functions to register that the peripheral will need to be revisited when the state changes
SB_Error SB_registerStateTransitionCallback(SB_StateTransitionCallbackFunc function, SB_State_Transition transition, SB_State state) {
	SB_CallbackFunc *newCallback, *current;

	if (NULL == function) {
		return InvalidParameter;
	}

	newCallback = (SB_CallbackFunc*)malloc( sizeof(SB_CallbackFunc));
	if (NULL == newCallback) {
		return OutOfMemory;
	}

	newCallback->state = state;
	newCallback->transition = transition;
	newCallback->function = function;
	newCallback->next = NULL;

	if (NULL == callbackTable.callbacks) {
		callbackTable.callbacks = newCallback;
		return NoError;
	}

	current = callbackTable.callbacks;

	while (NULL != current->next) {
		current = current->next;
	}

	current->next = newCallback;

	return NoError;
}

void SB_callCallback(SB_State_Transition transition, SB_State state) {
	UInt taskKey;
    SB_CallbackFunc *current = callbackTable.callbacks;
	if (current == NULL) {
		return;
	}

	// disable context switching for other tasks in this section
	taskKey = Task_disable();
	do {
		if (transition == current->transition && state == current->state && NULL != current->function) {
			current->function(transition, state);
		}

		current = current->next;
	} while (current != NULL);

	// restore context switching for other tasks at this point
	Task_restore(taskKey);
}
