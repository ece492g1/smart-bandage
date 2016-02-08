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
 
 
 #include "stdio.h"
 #include "stdlib.h"
 #include "fsm.h"


/*
COMMENTED OUT FOR TESTING PURPOSES

//where things will actually be happening
void main(void) {
//start out in the initialize state
    currentState = S_INIT;
    //check if a new event has occurred
    newEvent = getNewEvent(); 
    if (((newEvent >= 0) && (newEvent < EVENTS)) && ((currentState >= 0) && (currentState < STATES))) {
        state_table[currentState][newEvent] ();
    }
    else {
    // value outside range of LUT. call error - not necessarily same as error from bandage
        errorEvent();
    }
}
*/

//switch the states
int switchState(int newState) {
	printf("\nnew State: %d", newState);

	if(newState == S_INIT) {
		currentState = S_INIT;
	}

	if(newState == S_SLEEP) {
		currentState = S_SLEEP;
	}

	if(newState == S_CHECK) {
		currentState = S_CHECK;
	}
	
	if(newState == S_TRANSMIT) {
		currentState = S_TRANSMIT;
	}

	if(newState == S_ERROR_TEMP) {
		currentState = S_ERROR_TEMP;
	}
	
	//otherwise it will go into the error state either way?
	if(newState == S_ERROR_PERM) {
		currentState = S_ERROR_PERM;
	}
}

//action procedures here for now, also might want to put into another file later
void checkTimerExpired(void){
    //the timer expired, 
    switchState(S_CHECK);
    //check data from periperal sensors and store it into a register
    
    //reset the timer
    
    //check new data with previous data (if it exists), if it is outside expected range, send an alert, if not, return to sleep
    
    
}
void bleTimerExpired(void){
    //the transmit data timer expired
    switchState(S_TRANSMIT);
    //send the data via the bluetooth to the smartphone android application
    //check if successful - remove previous data(?) and return to sleep
    //if not, reset timer, and save previous data

}
void noBandageDetected(void){
    //no connection between the communications module and the bandage module detected
    //set the current state to temp error and wait for check timer to expire to see if initialized (?)
    switchState(S_ERROR_TEMP);


}
void errorEvent(void){
    //a more permanent error has occurred, do nothing or else relay problem via bluetooth
    switchState(S_ERROR_PERM);

}
void dataChanged(void){
    //data changed prior to the ble_timer expiring
    switchState(S_TRANSMIT);

}

//not actually an event, just something that happens when there isn't anything else to do
void returnToSleep(void){
    switchState(S_SLEEP);
}


/*
//send the next event to process
enum events getNewEvent(int trigger){
    //get the next event here
    /*  E_CHECK_TIMER_EXPIRES, E_BLE_TIMER EXPIRES, E_DATA_CHANGE, E_NO_BANDAGE_DETECTED, E_ERROR  
    
    //if the check timer expires then set event to the check timer to call the function from the LUT
    if(checkTimerInterrupt) {
        newEvent = E_CHECK_TIMER_EXPIRES;
    }
    
    else if(bleTimerInterrupt) {
        newEvent = E_BLE_TIMER EXPIRES;
    }
    
    else if(dataChangeInterrupt) {
        newEvent = E_DATA_CHANGE;
    }
    
    else if(noBandageError) {
        newEvent = E_NO_BANDAGE_DETECTED;
    }
    
    else if(otherError) {
        newEvent = E_ERROR;
    }
    
    else {
        returnToSleep();
    }
}

*/
