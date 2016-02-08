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
 
 #define EVENTS 5
 #define STATES 6 //?
 
 //main states for the state machine for the mcu
 enum states { S_SLEEP, S_CHECK, S_TRANSMIT, S_ERROR_TEMP, S_ERROR_PERM, S_INIT } currentState;
 
 //main event definitins, things that will trigger a transition
 //data change is equivalent to an alert
 enum events { E_CHECK_TIMER_EXPIRES, E_BLE_TIMER_EXPIRES, E_DATA_CHANGE, E_NO_BANDAGE_DETECTED, E_ERROR } newEvent; 


//function prototypes for each action (maybe put into .h file later)
void checkTimerExpired(void);
void bleTimerExpired(void);
void noBandageDetected(void);
void errorEvent(void); //a more permanent error caught
void dataChanged(void);
void returnToSleep(void);
enum events getNewEvent(void);


//LUT TABLE
//implemented a "do nothing" function for the events that are not relevent to a particular 
void(*state_table [STATES][EVENTS]) (void) = {
    {checkTimerExpired, bleTimerExpired, returnToSleep, noBandageDetected, errorEvent }, //procedures for state sleep
    {checkTimerExpired, bleTimerExpired, dataChanged, noBandageDetected, errorEvent },//procedures for state check
    {returnToSleep, bleTimerExpired, returnToSleep, noBandageDetected, errorEvent}, //procedure for transmit
    {checkTimerExpired, bleTimerExpired, dataChanged, noBandageDetected, errorEvent}, //procedures for state error temp
    {returnToSleep, bleTimerExpired, returnToSleep, returnToSleep, errorEvent}, //procedures for state error perm
    {returnToSleep, bleTimerExpired, dataChanged, noBandageDetected, errorEvent} //procedures for initialize state
};

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

//action procedures here for now, also might want to put into another file later
void checkTimerExpired(void){
    //the timer expired, 
    currentState = S_CHECK;
    //check data from periperal sensors and store it into a register
    
    //reset the timer
    
    //check new data with previous data (if it exists), if it is outside expected range, send an alert, if not, return to sleep
    
    
}
void bleTimerExpired(void){
    //the transmit data timer expired
    currentState = S_TRANSMIT;
    //send the data via the bluetooth to the smartphone android application
    //check if successful - remove previous data(?) and return to sleep
    //if not, reset timer, and save previous data

}
void noBandageDetected(void){
    //no connection between the communications module and the bandage module detected
    //set the current state to temp error and wait for check timer to expire to see if initialized (?)
    currentState = S_ERROR_TEMP;


}
void errorEvent(void){
    //a more permanent error has occurred, do nothing or else relay problem via bluetooth
    currentState = S_ERROR_PERM;

}
void dataChanged(void){
    //data changed prior to the ble_timer expiring
    currentState = S_TRANSMIT;

}

//not actually an event, just something that happens when there isn't anything else to do
void returnToSleep(void){
    currentState = S_SLEEP;
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
