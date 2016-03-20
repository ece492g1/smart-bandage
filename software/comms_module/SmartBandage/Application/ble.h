/*
 * ble.h
 *
 *  Created on: Feb 15, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_BLE_H_
#define APPLICATION_BLE_H_

#include "Board.h"

/*********************************************************************
 * CONSTANTS
 */
// Advertising interval when device is discoverable (units of 625us, 160=100ms)
#define DEFAULT_ADVERTISING_INTERVAL          160

// By setting this to zero, the device will go into the waiting state after
// being discoverable for 30.72 second, and will not being advertising again
// until the enabler is set back to TRUE
#define DEFAULT_ADVERTISING_OFF_TIME		  0

// Limited discoverable mode advertises for 30.72s, and then stops
// General discoverable mode advertises indefinitely
#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_GENERAL

#ifndef FEATURE_OAD
// Minimum connection interval (units of 1.25ms, 80=100ms) if automatic
// parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     100

// Maximum connection interval (units of 1.25ms, 800=1000ms) if automatic
// parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     1000
#else
// Minimum connection interval (units of 1.25ms, 8=10ms) if automatic
// parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     8

// Maximum connection interval (units of 1.25ms, 8=10ms) if automatic
// parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     8
#endif // FEATURE_OAD

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY         0

// Supervision timeout value (units of 10ms, 1000=10s) if automatic parameter
// update request is enabled
#define DEFAULT_DESIRED_CONN_TIMEOUT          1000

// Whether to enable automatic parameter update request when a connection is
// formed
#define DEFAULT_ENABLE_UPDATE_REQUEST         TRUE

// Connection Pause Peripheral time value (in seconds)
#define DEFAULT_CONN_PAUSE_PERIPHERAL         6

#define DEFAULT_INITIAL_ADVERTISING			  FALSE

// Default passkey "000000"
#define DEFAULT_PASSKEY						  0

#define DEFAULT_PAIRING_MORE				  GAPBOND_PAIRING_MODE_WAIT_FOR_REQ

#define DEFAULT_MITM_PROTECTION				  TRUE

#define DEFAULT_IO_CAP					      GAPBOND_IO_CAP_DISPLAY_ONLY

#define DEFAULT_BONDING_ENABLED			      TRUE

#ifdef FEATURE_OAD
// The size of an OAD packet.
#define OAD_PACKET_SIZE                       ((OAD_BLOCK_SIZE) + 2)
#endif // FEATURE_OAD

// Internal Events for RTOS application
#define SBP_STATE_CHANGE_EVT                  0x0001
#define SBP_CHAR_CHANGE_EVT                   0x0002
#define SBP_PERIODIC_EVT                      0x0004
#define SBP_CONN_EVT_END_EVT                  0x0008


//extern void SB_bleInit();
extern void SB_processBLEMessages();
extern void SimpleBLEPeripheral_init(void);
extern SB_Error SB_enableBLE();
extern SB_Error SB_disableBLE();
extern bool SB_bleConnected();

#endif /* APPLICATION_BLE_H_ */
