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
#include "../Application/Board.h"

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
static bool _readingsNotificationStateChanged = false;
uint16_t * getExtraDataPtr(uint8_t dataNo);

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

static uint8 charValReadings[SB_BLE_READINGS_LEN];
static uint8 charValReadingSize[SB_BLE_READINGSIZE_LEN];
static uint8 charValReadingCount[SB_BLE_READINGCOUNT_LEN];
static uint8 charValReadingDataOffsets[SB_BLE_READINGDATAOFFSETS_LEN];

static uint8 charValExtraPtr[SB_BLE_EXTRAPTR_LEN];

static gattCharCfg_t *readingsCharConfig;

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

	// Readings characteristic
	{
		.uuid   	 = SB_BLE_READINGS_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_READINGS_UUID), HI_UINT16(SB_BLE_READINGS_UUID) },
		.props  	 = GATT_PROP_READ | GATT_PROP_INDICATE,//| GATT_PROP_NOTIFY | GATT_PROP_INDICATE,
		.perms		 = GATT_PERMIT_READ,
		.value  	 = charValReadings,
		.length 	 = SB_BLE_READINGS_LEN,
		.description = "Readings",
	},

	// ReadingsSize characteristic
	{
		.uuid   	 = SB_BLE_READINGSIZE_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_READINGSIZE_UUID), HI_UINT16(SB_BLE_READINGSIZE_UUID) },
		.props  	 = GATT_PROP_READ,
		.perms		 = GATT_PERMIT_READ,
		.value  	 = charValReadingSize,
		.length 	 = SB_BLE_READINGSIZE_LEN,
		.description = "ReadingSize",
	},

	// ReadingCount characteristic
	{
		.uuid   	 = SB_BLE_READINGCOUNT_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_READINGCOUNT_UUID), HI_UINT16(SB_BLE_READINGCOUNT_UUID) },
		.props  	 = GATT_PROP_READ  | GATT_PROP_WRITE_NO_RSP,
		.perms		 = GATT_PERMIT_READ | GATT_PERMIT_WRITE,
		.value  	 = charValReadingCount,
		.length 	 = SB_BLE_READINGCOUNT_LEN,
		.description = "ReadingCount",
	},

	// ReadingDataOffsets characteristic
	{
		.uuid   	 = SB_BLE_READINGDATAOFFSETS_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_READINGDATAOFFSETS_UUID), HI_UINT16(SB_BLE_READINGDATAOFFSETS_UUID) },
		.props  	 = GATT_PROP_READ,
		.perms		 = GATT_PERMIT_READ,
		.value  	 = charValReadingDataOffsets,
		.length 	 = SB_BLE_READINGDATAOFFSETS_LEN,
		.description = "ReadingsDataOffsets",
	},

	// Extra Ptr characteristic
	{
		.uuid   	 = SB_BLE_EXTRAPTR_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_EXTRAPTR_UUID), HI_UINT16(SB_BLE_EXTRAPTR_UUID) },
		.props  	 = GATT_PROP_READ | GATT_PROP_WRITE,
		.perms		 = GATT_PERMIT_READ | GATT_PERMIT_WRITE,
		.value  	 = charValExtraPtr,
		.length 	 = SB_BLE_EXTRAPTR_LEN,
		.description = "Extra Pointer",
	},

	// Extra Data characteristic
	{
		.uuid   	 = SB_BLE_EXTRADATA_UUID,
		.uuidptr	 = { LO_UINT16(SB_BLE_EXTRADATA_UUID), HI_UINT16(SB_BLE_EXTRADATA_UUID) },
		.props  	 = GATT_PROP_READ | GATT_PROP_WRITE,
		.perms		 = GATT_PERMIT_READ | GATT_PERMIT_WRITE,
		.value  	 = NULL,
		.length 	 = SB_BLE_EXTRADATA_LEN,
		.description = "Extra Data",
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

		// Characteristic configuration (notify only)
		if (characteristics[c].props & GATT_PROP_NOTIFY || characteristics[c].props & GATT_PROP_INDICATE) {
			simpleProfileAttrTbl[i  ].type.len    = ATT_BT_UUID_SIZE;
			simpleProfileAttrTbl[i  ].type.uuid   = clientCharCfgUUID;
			simpleProfileAttrTbl[i  ].permissions = GATT_PERMIT_READ | GATT_PERMIT_WRITE;
			simpleProfileAttrTbl[i  ].handle 	  = NULL;
			simpleProfileAttrTbl[i++].pValue 	  = (uint8_t*) &readingsCharConfig;
		}

		// Characteristic description
		simpleProfileAttrTbl[i  ].type.len    = ATT_BT_UUID_SIZE;
		simpleProfileAttrTbl[i  ].type.uuid   = charUserDescUUID;
		simpleProfileAttrTbl[i  ].permissions = GATT_PERMIT_READ;
		simpleProfileAttrTbl[i  ].handle 	  = NULL;
		simpleProfileAttrTbl[i++].pValue 	  = (uint8*)characteristics[c].description;
	}

	readingsCharConfig = (gattCharCfg_t *) ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );

	if (NULL == readingsCharConfig) {
		return INVALID_MEM_SIZE;
	}

	GATTServApp_InitCharCfg( INVALID_CONNHANDLE, readingsCharConfig );

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
 * @fn      SB_Profile_SetParameter
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
inline bStatus_t SB_Profile_SetParameter( SB_CHARACTERISTIC param, uint8 len, const void *value )
{
	return SB_Profile_SetParameterPartial( param, len, 0, value );
}

/*********************************************************************
 * @fn      SB_Profile_SetParameterPartial
 *
 * @brief   Partially set a parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   offset - The offset (from the start of data) where writing should start
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SB_Profile_SetParameterPartial( SB_CHARACTERISTIC param, uint8 len, uint8_t offset, const void *value )
{
	if ( (offset + len) > characteristics[param].length || len == 0 ) {
		return bleInvalidRange;
	}

	VOID memcpy( characteristics[param].value + offset, value, len );


	return SUCCESS;
}

/*********************************************************************
 * @fn      SB_Profile_SetParameterPartial
 *
 * @brief   Get a pointer to the characteristic buffer at the specified offset.
 * 			The pointer returned is valid only for `len` bytes.
 * 			This should only be used for sufficiently large values where data should
 * 			be written directly to the buffer.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   offset - The offset (from the start of data) where writing should start
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  NULL if invalid range, or the pointer to characteristic memory
 */
uint8* SB_Profile_GetCharacteristicWritePTR( SB_CHARACTERISTIC param, uint8 len, uint8_t offset )
{
	if ( (offset + len) > characteristics[param].length || len == 0 ) {
		return NULL;
	}

	return characteristics[param].value + offset;
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
inline bStatus_t SB_Profile_Set16bParameter( SB_CHARACTERISTIC param, uint16 value, uint8 valueIndex )
{
	return SB_Profile_SetParameterPartial( param, sizeof(uint16), valueIndex*sizeof(uint16), &value );
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

bStatus_t SB_Profile_MarkParameterUpdated( SB_CHARACTERISTIC param ) {
	bStatus_t status;

	if (param >= SB_NUM_CHARACTERISTICS || !(characteristics[param].props & GATT_PROP_NOTIFY || characteristics[param].props & GATT_PROP_INDICATE)) {
		return INVALIDPARAMETER;
	}

	if (param != SB_CHARACTERISTIC_READINGS)  {
		return INVALIDPARAMETER;
	}

	uint8_t entity = ICall_getEntityId();
	if (entity == ICALL_INVALID_ENTITY_ID) {
		return INVALID_TASK;
	}

	status = GATTServApp_ProcessCharCfg(
		readingsCharConfig,
		characteristics[param].value,
		false,
		simpleProfileAttrTbl,
		GATT_NUM_ATTRS( simpleProfileAttrTbl ),
		entity,
		simpleProfile_ReadAttrCB );

	return status;
}

bool SB_Profile_ReadingsNotificationsEnabled() {
	uint8_t i;
	gattAttribute_t * attr = NULL;

	for (i = 0; i < SERVAPP_NUM_ATTR_SUPPORTED; ++i) {
		if (simpleProfileAttrTbl[i].pValue == (uint8_t*)&readingsCharConfig) {
			attr = &simpleProfileAttrTbl[i];
			break;
		}
	}

	if (NULL == attr) {
		return false;
	}

	for (i = 0; i < linkDBNumConns; ++i) {
		if ((readingsCharConfig[i].value & GATT_CLIENT_CFG_NOTIFY) || (readingsCharConfig[i].value & GATT_CLIENT_CFG_INDICATE)) {
			return true;
		}
	}

	return false;
}

bool SB_Profile_NotificationStateChanged(SB_CHARACTERISTIC param ) {
	if (param == SB_CHARACTERISTIC_READINGS) {
		bool value = _readingsNotificationStateChanged;
		_readingsNotificationStateChanged = false;

		return value;
	}

	return false;
}

bStatus_t SB_Profile_ClearNotificationState() {
	uint8_t i, result;

	uint8_t entity = ICall_getEntityId();
	if (entity == ICALL_INVALID_ENTITY_ID) {
		return INVALID_TASK;
	}

	for (i = 0; i < linkDBNumConns; ++i) {
		if (SUCCESS != (GATTServApp_WriteCharCfg(readingsCharConfig[i].connHandle, readingsCharConfig, 0))) {
			return result;
		}
	}

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

	if ( pAttr->type.len == ATT_BT_UUID_SIZE ) {
		uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
		uint8 c = uuid - SB_BLE_SERV_UUID - 1;

		// Validate the characteristic
		if (c >= SB_NUM_CHARACTERISTICS) {
			// Invalid characteristic
			*pLen = 0;
			return ATT_ERR_ATTR_NOT_FOUND;
		}

		// Validate offset
		if ( offset > characteristics[c].length) {
			*pLen = 0;
			return ( ATT_ERR_INVALID_OFFSET );
		}

		*pLen = characteristics[c].length - offset;

		// Ensure we don't write too much data
		if (*pLen > maxLen) {
			*pLen = maxLen;
		}

		// Copy the data to the output buffer
		if (c == SB_CHARACTERISTIC_EXTRADATA) {
			uint16_t * value = getExtraDataPtr(*charValExtraPtr);
			if (NULL == value) {
				return ATT_ERR_UNSUPPORTED_REQ;
			}

			memcpy( pValue, value + offset, *pLen );
		} else {
			memcpy( pValue, pAttr->pValue + offset, *pLen );
		}
	} else {
		// 128-bit UUID
		*pLen = 0;
		status = ATT_ERR_INVALID_HANDLE;
	}

	return ( status );
}

uint16_t * getExtraDataPtr(uint8_t dataNo) {
	switch (dataNo) {
	case 1:
		return &SB_GlobalDeviceConfiguration.CheckSleepIntervalMS;
	case 2:
		return &SB_GlobalDeviceConfiguration.BLECheckInterval;
	case 3:
		return &SB_GlobalDeviceConfiguration.CheckReadDelayMS;
	case 4:
		return &SB_GlobalDeviceConfiguration.MaxTransmitStateTimeS;

	default:
		return NULL;
	}
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
				break;
			}

			//Write the value
			memcpy(pAttr->pValue + offset, pValue, len);

			// Notify the application that system time changed
			notifyApp = SB_CHARACTERISTIC_SYSTEMTIME;

			break;

		case SB_BLE_READINGCOUNT_UUID:
			// Ensure the length and offset don't cause us to overwrite
			if  (offset >= characteristics[c].length || ((uint16)characteristics[c].length) - offset < len) {
				status = ATT_ERR_INVALID_VALUE_SIZE;
				break;
			}

			// The value does not actually get changed by the write

			// Notify the application that the write was performed
			notifyApp = SB_CHARACTERISTIC_READINGCOUNT;

			break;

		case SB_BLE_EXTRAPTR_UUID:
			// Ensure the length and offset don't cause us to overwrite
			if  (offset >= characteristics[c].length || ((uint16)characteristics[c].length) - offset < len) {
				status = ATT_ERR_INVALID_VALUE_SIZE;
				break;
			}

			//Write the value
			memcpy(pAttr->pValue + offset, pValue, len);

			// Notify the application that system time changed
			notifyApp = SB_CHARACTERISTIC_EXTRAPTR;
			break;

		case SB_BLE_EXTRADATA_UUID:
			// Ensure the length and offset don't cause us to overwrite
			if  (offset >= characteristics[c].length || ((uint16)characteristics[c].length) - offset < len) {
				status = ATT_ERR_INVALID_VALUE_SIZE;
				break;
			}

			{
				// Check the value being written
				uint16_t *value = getExtraDataPtr(*charValExtraPtr);

				if (NULL == value) {
					status = ATT_ERR_UNSUPPORTED_REQ;
					break;
				}

				//Write the value
				memcpy(value + offset, pValue, len);
			}

			// Notify the application that system time changed
			notifyApp = SB_CHARACTERISTIC_EXTRADATA;
			break;

		case GATT_CLIENT_CHAR_CFG_UUID:
			status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
													 offset, GATT_CLIENT_CFG_INDICATE );

//			_readingsNotificationStateChanged = true;
			notifyApp = SB_CHARACTERISTIC_READINGS;

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
