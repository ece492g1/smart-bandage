/**************************************************************************************************
  Filename:       simpleGATTprofile.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    This file contains the Simple GATT profile sample GATT service 
                  profile for use with the BLE sample application.

  Copyright 2010 - 2015 Texas Instruments Incorporated. All rights reserved.

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

/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "smartBandageProfile.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0xFFF0
CONST uint8 simpleProfileServUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SB_BLE_SERV_UUID), HI_UINT16(SB_BLE_SERV_UUID)
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static simpleProfileCBs_t *simpleProfile_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// Simple Profile Service attribute
static CONST gattAttrType_t simpleProfileService = { ATT_BT_UUID_SIZE, simpleProfileServUUID };

// Characteristic values
static uint8 charValTemperature[SB_BLE_TEMPERATURE_LEN];
static uint8 charValHumidity[SB_BLE_HUMIDITY_LEN];
static uint8 charValBandageID[SB_BLE_BANDAGEID_LEN];
static uint8 charValBandageState[SB_BLE_BANDAGESTATE_LEN];
static uint8 charValBattCharge[SB_BLE_BATTCHARGE_LEN];
static uint8 charValExtPower[SB_BLE_EXTPOWER_LEN];
static uint8 charValMoistureMap[SB_BLE_MOISTUREMAP_LEN];
static uint8 charValSystemTime[SB_BLE_SYSTEMTIME_LEN];

// Characteristic structs
static SB_PROFILE_CHARACTERISTIC characteristics[SB_NUM_CHARACTERISTICS] = {
	// Temperature characteristic
	{
		.uuid   	 = SB_BLE_TEMPERATURE_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_TEMPERATURE_UUID), HI_UINT16(SB_BLE_TEMPERATURE_UUID) },
		.props 		 = GATT_PROP_READ,
		.perms		 = GATT_PERMIT_READ,
		.value 		 = charValTemperature,
		.length		 = SB_BLE_TEMPERATURE_LEN,
		.description = "Temperature",
	},

	// Humidity characteristic
	{
		.uuid   	 = SB_BLE_HUMIDITY_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_HUMIDITY_UUID), HI_UINT16(SB_BLE_HUMIDITY_UUID) },
		.props  	 = GATT_PROP_READ,
		.perms		 = GATT_PERMIT_READ,
		.value  	 = charValHumidity,
		.length 	 = SB_BLE_HUMIDITY_LEN,
		.description = "Humidity",
	},

	// BandageID characteristic
	{
		.uuid   	 = SB_BLE_BANDAGEID_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_BANDAGEID_UUID), HI_UINT16(SB_BLE_BANDAGEID_UUID) },
		.props  	 = GATT_PROP_READ,
		.perms		 = GATT_PERMIT_READ,
		.value  	 = charValBandageID,
		.length 	 = SB_BLE_BANDAGEID_LEN,
		.description = "BandageID",
	},

	// BandageState characteristic
	{
		.uuid   	 = SB_BLE_BANDAGESTATE_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_BANDAGESTATE_UUID), HI_UINT16(SB_BLE_BANDAGESTATE_UUID) },
		.props  	 = GATT_PROP_READ,
		.perms		 = GATT_PERMIT_READ,
		.value  	 = charValBandageState,
		.length 	 = SB_BLE_BANDAGESTATE_LEN,
		.description = "BandageState",
	},

	// BattCharge characteristic
	{
		.uuid   	 = SB_BLE_BATTCHARGE_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_BATTCHARGE_UUID), HI_UINT16(SB_BLE_BATTCHARGE_UUID) },
		.props  	 = GATT_PROP_READ,
		.perms		 = GATT_PERMIT_READ,
		.value  	 = charValBattCharge,
		.length 	 = SB_BLE_BATTCHARGE_LEN,
		.description = "BattCharge",
	},

	// ExtPower characteristic
	{
		.uuid   	 = SB_BLE_EXTPOWER_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_EXTPOWER_UUID), HI_UINT16(SB_BLE_EXTPOWER_UUID) },
		.props  	 = GATT_PROP_READ,
		.perms		 = GATT_PERMIT_READ,
		.value  	 = charValExtPower,
		.length 	 = SB_BLE_EXTPOWER_LEN,
		.description = "ExternalPower",
	},

	// MoistureMap characteristic
	{
		.uuid   	 = SB_BLE_MOISTUREMAP_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_MOISTUREMAP_UUID), HI_UINT16(SB_BLE_MOISTUREMAP_UUID) },
		.props  	 = GATT_PROP_READ,
		.perms		 = GATT_PERMIT_READ,
		.value  	 = charValMoistureMap,
		.length 	 = SB_BLE_MOISTUREMAP_LEN,
		.description = "MoistureMap",
	},

	// SystemTime characteristic
	{
		.uuid   	 = SB_BLE_SYSTEMTIME_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_SYSTEMTIME_UUID), HI_UINT16(SB_BLE_SYSTEMTIME_UUID) },
		.props  	 = GATT_PROP_READ | GATT_PROP_WRITE,
		.perms		 = GATT_PERMIT_READ | GATT_PERMIT_WRITE,
		.value  	 = charValSystemTime,
		.length 	 = SB_BLE_SYSTEMTIME_LEN,
		.description = "SystemTime",
	},
};

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t simpleProfileAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] =
{
  // Simple Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&simpleProfileService            /* pValue */
  },
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t simpleProfile_ReadAttrCB(uint16_t connHandle,
                                          gattAttribute_t *pAttr, 
                                          uint8_t *pValue, uint16_t *pLen,
                                          uint16_t offset, uint16_t maxLen,
                                          uint8_t method);
static bStatus_t simpleProfile_WriteAttrCB(uint16_t connHandle,
                                           gattAttribute_t *pAttr,
                                           uint8_t *pValue, uint16_t len,
                                           uint16_t offset, uint8_t method);

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t simpleProfileCBs =
{
  simpleProfile_ReadAttrCB,  // Read callback function pointer
  simpleProfile_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleProfile_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t SB_Profile_AddService( uint32 services )
{
	uint8 status, i = 1;
	SB_CHARACTERISTIC c;

	// Init the profile attribute table
	for (c = (SB_CHARACTERISTIC)0; c < SB_NUM_CHARACTERISTICS; ++c) {
		// Declare the characteristic
		simpleProfileAttrTbl[i  ].type.len    = ATT_BT_UUID_SIZE;
		simpleProfileAttrTbl[i  ].type.uuid   = characterUUID;
		simpleProfileAttrTbl[i  ].permissions = GATT_PERMIT_READ;
		simpleProfileAttrTbl[i  ].handle 	  = NULL;
		simpleProfileAttrTbl[i++].pValue 	  = &characteristics[c].props;

		// Characteristic value
		simpleProfileAttrTbl[i  ].type.len    = ATT_BT_UUID_SIZE;
		simpleProfileAttrTbl[i  ].type.uuid   = characteristics[c].uuidptr;
		simpleProfileAttrTbl[i  ].permissions = characteristics[c].perms;
		simpleProfileAttrTbl[i  ].handle 	  = NULL;
		simpleProfileAttrTbl[i++].pValue 	  = characteristics[c].value;

		// Characteristic description
		simpleProfileAttrTbl[i  ].type.len    = ATT_BT_UUID_SIZE;
		simpleProfileAttrTbl[i  ].type.uuid   = charUserDescUUID;
		simpleProfileAttrTbl[i  ].permissions = GATT_PERMIT_READ;
		simpleProfileAttrTbl[i  ].handle 	  = NULL;
		simpleProfileAttrTbl[i++].pValue 	  = (uint8*)characteristics[c].description;
	}

	if ( services & SB_BLE_SERVICE )
	{
		// Register GATT attribute list and CBs with GATT Server App
		status = GATTServApp_RegisterService( simpleProfileAttrTbl,
											GATT_NUM_ATTRS( simpleProfileAttrTbl ),
											GATT_MAX_ENCRYPT_KEY_SIZE,
											&simpleProfileCBs );
	}
	else
	{
		status = SUCCESS;
	}

	return ( status );
}

/*********************************************************************
 * @fn      SimpleProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t SB_Profile_RegisterAppCBs( simpleProfileCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    simpleProfile_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}

/*********************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SB_Profile_SetParameter( SB_CHARACTERISTIC param, uint8 len, void *value )
{
	if ( len != characteristics[param].length ) {
		return bleInvalidRange;
	}

	VOID memcpy( characteristics[param].value, value, characteristics[param].length );


	return SUCCESS;
}

/*********************************************************************
 * @fn      SB_Profile_Set16bParameter
 *
 * @brief   Set a 16bit parameter within an array of 16bit parameters.
 *
 * @param   param - Profile parameter ID
 * @param   value - 16bit value to write
 * @param   valueIndex - Index of the 16bit parameter within the array to write to.
 *
 * @return  bStatus_t
 */
bStatus_t SB_Profile_Set16bParameter( SB_CHARACTERISTIC param, uint16 value, uint8 valueIndex )
{
	if ( sizeof(uint16)*valueIndex >= characteristics[param].length ) {
		return bleInvalidRange;
	}

	VOID memcpy( characteristics[param].value + valueIndex*sizeof(uint16), &value, sizeof(uint16) );

	return SUCCESS;
}

/*********************************************************************
 * @fn      SimpleProfile_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SB_Profile_GetParameter( SB_CHARACTERISTIC param, void *value, int maxlength )
{
	if (characteristics[param].length > maxlength) {
	  return bleInvalidRange;
	}

	memcpy(value, characteristics[param].value, characteristics[param].length);

	return SUCCESS;
}

/*********************************************************************
 * @fn          simpleProfile_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 * @param       method - type of read message
 *
 * @return      SUCCESS, blePending or Failure
 */
static bStatus_t simpleProfile_ReadAttrCB(uint16_t connHandle,
                                          gattAttribute_t *pAttr,
                                          uint8_t *pValue, uint16_t *pLen,
                                          uint16_t offset, uint16_t maxLen,
                                          uint8_t method)
{
	bStatus_t status = SUCCESS;

	// If attribute permissions require authorization to read, return error
	if ( gattPermitAuthorRead( pAttr->permissions ) ) {
		// Insufficient authorization
		return ( ATT_ERR_INSUFFICIENT_AUTHOR );
	}

	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}

	if ( pAttr->type.len == ATT_BT_UUID_SIZE ) {
		uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
		uint8 c = uuid - SB_BLE_SERV_UUID - 1;
		if (c >= SB_NUM_CHARACTERISTICS) {
			// Invalid characteristic
			*pLen = 0;
			status = ATT_ERR_ATTR_NOT_FOUND;
		} else {
			*pLen = characteristics[c].length;
			memcpy( pValue, pAttr->pValue, characteristics[c].length );
		}
	} else {
		// 128-bit UUID
		*pLen = 0;
		status = ATT_ERR_INVALID_HANDLE;
	}

	return ( status );
}

/*********************************************************************
 * @fn      simpleProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t simpleProfile_WriteAttrCB(uint16_t connHandle,
                                           gattAttribute_t *pAttr,
                                           uint8_t *pValue, uint16_t len,
                                           uint16_t offset, uint8_t method)
{
	bStatus_t status = SUCCESS;
	uint8 notifyApp = 0xFF;

	// If attribute permissions require authorization to write, return error
	if ( gattPermitAuthorWrite( pAttr->permissions ) ) {
		// Insufficient authorization
		return ( ATT_ERR_INSUFFICIENT_AUTHOR );
	}

	if ( pAttr->type.len == ATT_BT_UUID_SIZE ) {
		// 16-bit UUID
		uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
		uint8 c = uuid - SB_BLE_SERV_UUID - 1;

		switch ( uuid )
		{
			case SB_BLE_SYSTEMTIME_UUID:
				// Ensure the length and offset don't cause us to overwrite
				if  (offset >= characteristics[c].length || ((uint16)characteristics[c].length) - offset < len) {
					status = ATT_ERR_INVALID_VALUE_SIZE;
				}

				//Write the value
				if ( status == SUCCESS ) {
					memcpy(pAttr->pValue + offset, pValue, len);

					// Notify the application that system time changed
					notifyApp = SB_CHARACTERISTIC_SYSTEMTIME;
				}

				break;

			case GATT_CLIENT_CHAR_CFG_UUID:
				status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
														 offset, GATT_CLIENT_CFG_NOTIFY );
				break;

			default:
				// Characteristic not found, or doesn't have write permissions
				if (c < SB_NUM_CHARACTERISTICS) {
					status = ATT_ERR_WRITE_NOT_PERMITTED;
				} else {
					status = ATT_ERR_ATTR_NOT_FOUND;
				}
		}
	} else {
		// 128-bit UUID
		status = ATT_ERR_INVALID_HANDLE;
	}

	// If a characteristic value changed then callback function to notify application of change
	if ( (notifyApp != 0xFF ) && simpleProfile_AppCBs && simpleProfile_AppCBs->pfnSimpleProfileChange ) {
		simpleProfile_AppCBs->pfnSimpleProfileChange( notifyApp );
	}

	return ( status );
}

/*********************************************************************
*********************************************************************/
