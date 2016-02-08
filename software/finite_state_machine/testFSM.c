// Test program to test the states of the Finite State Machine

#include <stdbool.h>
#include <stdio.h>
#include "fsm.c"



//define the interrupts and errors as booleans for testing purposes
bool checkTimerInterrupt;
bool bleTimerInterrupt;
bool dataChangeInterrupt;
bool noBandageError;
bool otherError;

//where things will actually be happening
void main(void) {
//start out in the initialize state
    currentState = S_INIT;

    // iterate through the states
    for(int i = 0; i < STATES; i++) {

        //iterate through each event 
        for(int j = 0; j <EVENTS; j++) {
            if (((j >= 0) && (j < EVENTS)) && ((i >= 0) && (i < STATES))) {
                state_table[i][j] ();
            
                printf("\n%d %d", i, j);
                printf("\ncurrent state: ");
                printf("%d", currentState);
                
            }
            else {
            // value outside range of LUT. call error - not necessarily same as error from bandage
            errorEvent();
            }
        }
    }   
}




    
