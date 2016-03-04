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

//EDITED HERE

//void SB_registerStateTransitionCallback((void*), SB_State_Transition); // not sure if the (void *) is the correct way of prototyping for void(*function)(void)
void SB_addCallback(SB_CallbackFunc* );
transitionTable *callbackTable = NULL;
UInt taskKey;

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

/* ASSUMPTIONS:
 *  - the tasks will have been previously created, and in the function SB_switchState we are using the tasks, but not creating any new ones
 *
 *
 *
 */
SB_State SB_switchState(SB_State newState) {
	switch (newState) {
	case S_INIT:

	case S_SLEEP:

	case S_CHECK:

	case S_TRANSMIT:
		SB_setError(NoError);
		// No "break" on purpose

	case S_ERROR_TEMP:

		systemState.currentState = newState;
		break;

	default:

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


// Called from within periheral functions to register that the peripheral will need to be revisited when the state changes
void SB_registerStateTransitionCallback(void function(void), SB_State_Transition peripheral) {
	//create the callback
	SB_CallbackFunc *newCallback;
	newCallback = (SB_CallbackFunc*)malloc( sizeof(SB_CallbackFunc));
	newCallback->transition = peripheral;
	newCallback->function = function;
	newCallback->next = 0; //start with the new callback not pointing to anything
	//add the callback to the transition table
	SB_addCallback(newCallback);
}

/*
void SB_registerEvent(SB_EventHandler eventHandler, SB_State state) {
	//passes function pointer to be called in the future
	(*eventHandler)();
}
*/


// This will be put within the switchstate cases to make sure the peripherals are dealt with correctly
// This will be put within the switchstate cases to make sure the peripherals are dealt with correctly
void SB_callCallback( SB_State_Transition state) {
	//check if the linked list in empty, if it is then don't do anything, if it isn't, then iterate through list and call functions
    SB_CallbackFunc *current;
	if(callbackTable == NULL) {
        return;
	}
	current = callbackTable->callbacks ;
	// disable context switching for other tasks in this section
	taskKey = Task_disable();
	while(current->next != NULL) {
		if(state == current->transition){
			current->function();
			current = current->next;
		} else {
			current = current->next;
		}
	}
	//current = current->next;
	if(state == current->transition) {
		current->function();
	}
	// restore context switching for other tasks at this point
	Task_restore(taskKey);
}


// Function to add the callbacks to the list to be used later.
void SB_addCallback( SB_CallbackFunc *callback ){
	if(callbackTable == NULL) {
        callbackTable = malloc(sizeof(transitionTable));
        callbackTable->callbacks = callback;
		return;
	}

	SB_CallbackFunc *current = callbackTable->callbacks;

	if ( current != 0 ) {
		while ( current->next != 0)
		{
			current = current->next;
		}
	}
	current->next = callback;
}

