 #define EVENTS 5
 #define STATES 6 

 //main states for the state machine for the mcu
 enum states { S_SLEEP, S_CHECK, S_TRANSMIT, S_ERROR_TEMP, S_ERROR_PERM, S_INIT } currentState;
 
 //main event definitins, things that will trigger a transition
 //data change is equivalent to an alert
 enum events { E_CHECK_TIMER_EXPIRES, E_BLE_TIMER_EXPIRES, E_DATA_CHANGE, E_NO_BANDAGE_DETECTED, E_ERROR } newEvent;

//function prototypes
void checkTimerExpired(void);
void bleTimerExpired(void);
void noBandageDetected(void);
void errorEvent(void); //a more permanent error caught
void dataChanged(void);
void returnToSleep(void);
int switchState(int); //?
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

