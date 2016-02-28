#ifndef SB_FSM_H
#define SB_FSM_H

#include "hci_tl.h"
#include "Board.h"


//main states for the state machine for the mcu
typedef enum {
	S_SLEEP,
	S_CHECK,
	S_TRANSMIT,
	S_ERROR_TEMP,
	S_ERROR_PERM,
	S_INIT,

	// This element should be left -- gives self-correcting code for the correct number of states supported
	SB_NUM_STATES
} SB_State;
 
//main event definitins, things that will trigger a transition
//data change is equivalent to an alert
typedef enum {
	E_CHECK_TIMER_EXPIRED,
	E_BLE_TIMER_EXPIRED,
	E_DATA_CHANGE,
	E_NO_BANDAGE_DETECTED,
	E_ERROR,

	// This element should be left -- gives self-correcting code for the correct number of states supported
	SB_NUM_EVENTS
} SB_Event;

//EDITED HERE
// the transition stages of the state machine for the mcu
typedef enum {
	enterSleep,
	enterCheck,
	enterTransmit,
	enterTempError,
	enterPermError,
	enterInit //this is needed for the case where you are going from the temp error and trying to initialize again

} SB_State_Transition;

//EDITED HERE
struct SB_CallbackFunc_struct;
struct SB_CallbackFunc_struct {
	struct SB_CallbackFunc_struct *next;
	void(*function);//(SB_State_Transition transition);
	SB_State_Transition transition;
};
typedef struct SB_CallbackFunc_struct SB_CallbackFunc;

//EDITED HERE
#define NUM_TRANSITIONS 6

typedef struct {
	SB_CallbackFunc *callbacks;//[NUM_TRANSITIONS]; //?
} transitionTable;

typedef struct {
	SB_State lastState;
	SB_State currentState;
	SB_Event lastEvent;
	SB_Error currentError;
	SB_Error lastError;
} SB_SystemState;


//function prototypes
SB_State SB_switchState(SB_State); //?
SB_Event SB_getNewEvent(void);
SB_State SB_handleEvent(SB_Event);
SB_State SB_currentState();


#endif
