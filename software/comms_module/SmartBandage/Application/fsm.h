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
