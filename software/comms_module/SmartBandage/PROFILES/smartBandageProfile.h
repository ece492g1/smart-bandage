/**************************************************************************************************
  Filename:       simpleGATTprofile.h
  Revised:        $Date: 2010-08-06 08:56:11 -0700 (Fri, 06 Aug 2010) $
  Revision:       $Revision: 23333 $

  Description:    This file contains the Simple GATT profile definitions and
                  prototypes.

  Copyright 2010 - 2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

#ifndef SIMPLEGATTPROFILE_H
#define SIMPLEGATTPROFILE_H

#include "hci_tl.h"
#include "gatt.h"


#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Service UUID
#define SB_BLE_SERV_UUID               		0xF0F0
    
// Characteristic UUIDs
#define SB_BLE_TEMPERATURE_UUID            	(SB_BLE_SERV_UUID +1+ SB_CHARACTERISTIC_TEMPERATURE)
#define SB_BLE_HUMIDITY_UUID            	(SB_BLE_SERV_UUID +1+ SB_CHARACTERISTIC_HUMIDITY)
#define SB_BLE_BANDAGEID_UUID           	(SB_BLE_SERV_UUID +1+ SB_CHARACTERISTIC_BANDAGEID)
#define SB_BLE_BANDAGESTATE_UUID            (SB_BLE_SERV_UUID +1+ SB_CHARACTERISTIC_BANDAGESTATE)
#define SB_BLE_BATTCHARGE_UUID      	    (SB_BLE_SERV_UUID +1+ SB_CHARACTERISTIC_BATTCHARGE)
#define SB_BLE_EXTPOWER_UUID    	        (SB_BLE_SERV_UUID +1+ SB_CHARACTERISTIC_EXTPOWER)
#define SB_BLE_MOISTUREMAP_UUID	            (SB_BLE_SERV_UUID +1+ SB_CHARACTERISTIC_MOISTUREMAP)
#define SB_BLE_SYSTEMTIME_UUID	            (SB_BLE_SERV_UUID +1+ SB_CHARACTERISTIC_SYSTEMTIME)

// For each characteristic the server has three entries, plus on for the service
#define SERVAPP_NUM_PROP_PER_CHARACTERISTIC 3
#define SERVAPP_NUM_ATTR_SUPPORTED         (SB_NUM_CHARACTERISTICS*SERVAPP_NUM_PROP_PER_CHARACTERISTIC + 1)

// Simple Keys Profile Services bit fields
#define SB_BLE_SERVICE               0x00000001

// Length of Characteristics in bytes
#define SB_BLE_TEMPERATURE_LEN           8
#define SB_BLE_HUMIDITY_LEN   	         2
#define SB_BLE_BANDAGEID_LEN   	         2
#define SB_BLE_BANDAGESTATE_LEN	         2
#define SB_BLE_BATTCHARGE_LEN   	     2
#define SB_BLE_EXTPOWER_LEN   	         1
#define SB_BLE_MOISTUREMAP_LEN           10
#define SB_BLE_SYSTEMTIME_LEN            4

/*********************************************************************
 * TYPEDEFS
 */
typedef enum {
	SB_CHARACTERISTIC_TEMPERATURE = 0,
	SB_CHARACTERISTIC_HUMIDITY = 1,
	SB_CHARACTERISTIC_BANDAGEID = 2,
	SB_CHARACTERISTIC_BANDAGESTATE = 3,
	SB_CHARACTERISTIC_BATTCHARGE = 4,
	SB_CHARACTERISTIC_EXTPOWER = 5,
	SB_CHARACTERISTIC_MOISTUREMAP = 6,
	SB_CHARACTERISTIC_SYSTEMTIME = 7,

	SB_NUM_CHARACTERISTICS = 8
} SB_CHARACTERISTIC;
  
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

// Callback when a characteristic value has changed
typedef void (*simpleProfileChange_t)( uint8 paramID );

typedef struct
{
  simpleProfileChange_t        pfnSimpleProfileChange;  // Called when characteristic value changes
} simpleProfileCBs_t;

#define SB_PROFILE_UUID_LEN ATT_BT_UUID_SIZE

typedef struct {
	uint16 uuid;
	uint8 uuidptr[SB_PROFILE_UUID_LEN];
	uint8 props;
	uint8 perms;
	uint8*value;
	uint8 length;
	char* description;
} SB_PROFILE_CHARACTERISTIC;
    

/*********************************************************************
 * API FUNCTIONS 
 */


/*
 * SimpleProfile_AddService- Initializes the Simple GATT Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 */

extern bStatus_t SB_Profile_AddService( uint32 services );

/*
 * SimpleProfile_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t SB_Profile_RegisterAppCBs( simpleProfileCBs_t *appCallbacks );

/*
 * SimpleProfile_SetParameter - Set a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t SB_Profile_SetParameter( SB_CHARACTERISTIC param, uint8 len, void *value );

extern bStatus_t SB_Profile_Set16bParameter( SB_CHARACTERISTIC param, uint16 value, uint8 valueIndex );

/*
 * SimpleProfile_GetParameter - Get a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t SB_Profile_GetParameter( SB_CHARACTERISTIC param, void *value, int maxlength );


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEGATTPROFILE_H */
