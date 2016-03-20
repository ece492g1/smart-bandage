/*
 * ble.c
 *
 *  Created on: Feb 15, 2016
 *      Author: michaelblouin
 */
#include <string.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>
#include <xdc/runtime/System.h>

#include "hci_tl.h"
#include "gatt.h"
#include "gapgattserver.h"
#include "gattservapp.h"
//#include "devinfoservice.h"
#include "../PROFILES/smartBandageProfile.h"

#include "peripheral.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"
#include "ble.h"

#include "readingsManager.h"

/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct
{
  appEvtHdr_t hdr;  // event header.
} sbpEvt_t;


/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Entity ID globally used to check for source and/or destination of messages
static ICall_EntityID selfEntity;

// Semaphore globally used to post events to the application thread
static ICall_Semaphore sem;

// Queue object used for app messages
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

static volatile bool bleConnected = false;

#if defined(FEATURE_OAD)
// Event data from OAD profile.
static Queue_Struct oadQ;
static Queue_Handle hOadQ;
#endif //FEATURE_OAD

// Task configuration
//Task_Struct sbpTask;
//Char sbpTaskStack[SBP_TASK_STACK_SIZE];

// Profile state and parameters
//static gaprole_States_t gapProfileState = GAPROLE_INIT;

// GAP - SCAN RSP data (max size = 31 bytes)
static uint8_t scanRspData[] =
{
  // human-readable name of the device
  0xE,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  'S',
  'm',
  'a',
  'r',
  't',
  ' ',
  'B',
  'a',
  'n',
  'd',
  'a',
  'g',
  'e',

  // connection interval range
  0x05,   // length of this data
  GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
  LO_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),   // 100ms
  HI_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),
  LO_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),   // 1s
  HI_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),

  // Tx power level
  0x02,   // length of this data
  GAP_ADTYPE_POWER_LEVEL,
  0       // 0dBm
};

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
static uint8_t advertData[] =
{
  // Flags; this sets the device to use limited discoverable
  // mode (advertises for 30 seconds at a time) instead of general
  // discoverable mode (advertises indefinitely)
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // service UUID, to notify central devices what services are included
  // in this peripheral
  0x03,   // length of this data
  GAP_ADTYPE_16BIT_MORE,      // some of the UUID's, but not all
  LO_UINT16(SB_BLE_SERV_UUID),
  HI_UINT16(SB_BLE_SERV_UUID)
};

// GAP GATT Attributes
static uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "Simple BLE Peripheral";

// Globals used for ATT Response retransmission
static gattMsgEvent_t *pAttRsp = NULL;
static uint8_t rspTxRetry = 0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

//static void SimpleBLEPeripheral_taskFxn(UArg a0, UArg a1);

static uint8_t SimpleBLEPeripheral_processStackMsg(ICall_Hdr *pMsg);
static uint8_t SimpleBLEPeripheral_processGATTMsg(gattMsgEvent_t *pMsg);
static void SimpleBLEPeripheral_processAppMsg(sbpEvt_t *pMsg);
static void SimpleBLEPeripheral_processStateChangeEvt(gaprole_States_t newState);
static void SimpleBLEPeripheral_processCharValueChangeEvt(uint8_t paramID);

static void SimpleBLEPeripheral_sendAttRsp(void);
static void SimpleBLEPeripheral_freeAttRsp(uint8_t status);

static void SimpleBLEPeripheral_stateChangeCB(gaprole_States_t newState);
#ifndef FEATURE_OAD
static void SimpleBLEPeripheral_charValueChangeCB(uint8_t paramID);
#endif //!FEATURE_OAD
static void SimpleBLEPeripheral_enqueueMsg(uint8_t event, uint8_t state);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t SB_gapRoleCBs =
{
  SimpleBLEPeripheral_stateChangeCB     // Profile State Change Callbacks
};

// GAP Bond Manager Callbacks
static gapBondCBs_t SB_BondMgrCBs =
{
  NULL, // Passcode callback (not used by application)
  NULL  // Pairing / Bonding state Callback (not used by application)
};

// Simple GATT Profile Callbacks
static simpleProfileCBs_t SB_simpleProfileCBs =
{
  SimpleBLEPeripheral_charValueChangeCB // Characteristic value change callback
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleBLEPeripheral_init
 *
 * @brief   Called during initialization and contains application
 *          specific initialization (ie. hardware initialization/setup,
 *          table initialization, power up notification, etc), and
 *          profile initialization/setup.
 *
 * @param   None.
 *
 * @return  None.
 */
void SimpleBLEPeripheral_init(void)
{
	// ******************************************************************
	// N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
	// ******************************************************************
	// Register the current thread as an ICall dispatcher application
	// so that the application can send and receive messages.
	ICall_registerApp(&selfEntity, &sem);

	// Set device's Sleep Clock Accuracy
	//HCI_EXT_SetSCACmd(40);

	// Create an RTOS queue for message from profile to be sent to app.
	appMsgQueue = Util_constructQueue(&appMsg);

	// Setup the GAP
	GAP_SetParamValue(TGAP_CONN_PAUSE_PERIPHERAL, DEFAULT_CONN_PAUSE_PERIPHERAL);

	// Setup the GAP Peripheral Role Profile
	{
		// For all hardware platforms, device starts advertising upon initialization
		uint8_t initialAdvertEnable = DEFAULT_INITIAL_ADVERTISING;
		uint16_t advertOffTime = DEFAULT_ADVERTISING_OFF_TIME;
		uint8_t enableUpdateRequest = DEFAULT_ENABLE_UPDATE_REQUEST;
		uint16_t desiredMinInterval = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
		uint16_t desiredMaxInterval = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
		uint16_t desiredSlaveLatency = DEFAULT_DESIRED_SLAVE_LATENCY;
		uint16_t desiredConnTimeout = DEFAULT_DESIRED_CONN_TIMEOUT;

		// Set the GAP Role Parameters
		GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initialAdvertEnable);
		GAPRole_SetParameter(GAPROLE_ADVERT_OFF_TIME, sizeof(uint16_t), &advertOffTime);

		GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof(scanRspData), scanRspData);
		GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), advertData);

		GAPRole_SetParameter(GAPROLE_PARAM_UPDATE_ENABLE, sizeof(uint8_t), &enableUpdateRequest);
		GAPRole_SetParameter(GAPROLE_MIN_CONN_INTERVAL, sizeof(uint16_t), &desiredMinInterval);
		GAPRole_SetParameter(GAPROLE_MAX_CONN_INTERVAL, sizeof(uint16_t), &desiredMaxInterval);
		GAPRole_SetParameter(GAPROLE_SLAVE_LATENCY, sizeof(uint16_t), &desiredSlaveLatency);
		GAPRole_SetParameter(GAPROLE_TIMEOUT_MULTIPLIER, sizeof(uint16_t), &desiredConnTimeout);
	}

	// Set the GAP Characteristics
	GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName);

	// Set advertising interval
	{
		uint16_t advInt = DEFAULT_ADVERTISING_INTERVAL;

		GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MIN, advInt);
		GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MAX, advInt);
		GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, advInt);
		GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, advInt);
	}

	// Setup the GAP Bond Manager
	{
		uint32_t passkey = DEFAULT_PASSKEY; // passkey "000000"
		uint8_t pairMode = GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
		uint8_t mitm = DEFAULT_MITM_PROTECTION;
		uint8_t ioCap = DEFAULT_IO_CAP;
		uint8_t bonding = DEFAULT_BONDING_ENABLED;

		GAPBondMgr_SetParameter(GAPBOND_DEFAULT_PASSCODE, sizeof(uint32_t), &passkey);
		GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8_t), &pairMode);
		GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8_t), &mitm);
		GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8_t), &ioCap);
		GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8_t), &bonding);
	}

	// Initialize GATT attributes
	GGS_AddService(GATT_ALL_SERVICES);           // GAP
	GATTServApp_AddService(GATT_ALL_SERVICES);   // GATT attributes
//	DevInfo_AddService();                        // Device Information Service

#ifndef FEATURE_OAD
	SB_Profile_AddService(GATT_ALL_SERVICES); // Simple GATT Profile
#endif //!FEATURE_OAD

#ifndef FEATURE_OAD
	// Register callback with SimpleGATTprofile
	SB_Profile_RegisterAppCBs(&SB_simpleProfileCBs);
#endif //!FEATURE_OAD

	// Start the Device
	VOID GAPRole_StartDevice(&SB_gapRoleCBs);

	// Start Bond Manager
	VOID GAPBondMgr_Register(&SB_BondMgrCBs);

	// Register with GAP for HCI/Host messages
	GAP_RegisterForMsgs(selfEntity);

	// Register for GATT local events and ATT Responses pending for transmission
	GATT_RegisterForMsgs(selfEntity);
}

SB_Error SB_enableBLE() {
	uint8_t status = SUCCESS;
	uint8_t advertEnable = true;

	// Set the GAP Role Parameters
	status |= GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advertEnable);

	return status == SUCCESS ? NoError : UnknownError;
}

SB_Error SB_disableBLE() {
	uint8_t status = SUCCESS;
	uint8_t advertEnable = false;

	// Set the GAP Role Parameters
	status |= GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advertEnable);

	status |= GAPRole_TerminateConnection();

	return status == SUCCESS ? NoError : UnknownError;
}

void SB_processBLEMessages() {

    ICall_EntityID dest;
    ICall_ServiceEnum src;
    ICall_HciExtEvt *pMsg = NULL;

    if (ICall_fetchServiceMsg(&src, &dest,
                              (void **)&pMsg) == ICALL_ERRNO_SUCCESS)
    {
      uint8 safeToDealloc = TRUE;

      if ((src == ICALL_SERVICE_CLASS_BLE) && (dest == selfEntity))
      {
        ICall_Event *pEvt = (ICall_Event *)pMsg;

        // Check for BLE stack events first
        if (pEvt->signature == 0xffff)
        {
          if (pEvt->event_flag & SBP_CONN_EVT_END_EVT)
          {
            // Try to retransmit pending ATT Response (if any)
            SimpleBLEPeripheral_sendAttRsp();
          }
        }
        else
        {
          // Process inter-task message
          safeToDealloc = SimpleBLEPeripheral_processStackMsg((ICall_Hdr *)pMsg);
        }
      }

      if (pMsg && safeToDealloc)
      {
        ICall_freeMsg(pMsg);
      }
    }

    // If RTOS queue is not empty, process app message.
    while (!Queue_empty(appMsgQueue))
    {
      sbpEvt_t *pMsg = (sbpEvt_t *)Util_dequeueMsg(appMsgQueue);
      if (pMsg)
      {
        // Process message.
        SimpleBLEPeripheral_processAppMsg(pMsg);

        // Free the space from the message.
        ICall_free(pMsg);
      }
    }
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_processStackMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  TRUE if safe to deallocate incoming message, FALSE otherwise.
 */
static uint8_t SimpleBLEPeripheral_processStackMsg(ICall_Hdr *pMsg)
{
  uint8_t safeToDealloc = TRUE;

  switch (pMsg->event)
  {
    case GATT_MSG_EVENT:
      // Process GATT message
      safeToDealloc = SimpleBLEPeripheral_processGATTMsg((gattMsgEvent_t *)pMsg);
      break;

    case HCI_GAP_EVENT_EVENT:
      {
        // Process HCI message
        switch(pMsg->status)
        {
          case HCI_COMMAND_COMPLETE_EVENT_CODE:
            // Process HCI Command Complete Event
            break;

          default:
            break;
        }
      }
      break;

    default:
      // do nothing
      break;
  }

  return (safeToDealloc);
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_processGATTMsg
 *
 * @brief   Process GATT messages and events.
 *
 * @return  TRUE if safe to deallocate incoming message, FALSE otherwise.
 */
static uint8_t SimpleBLEPeripheral_processGATTMsg(gattMsgEvent_t *pMsg)
{
  // See if GATT server was unable to transmit an ATT response
  if (pMsg->hdr.status == blePending)
  {
    // No HCI buffer was available. Let's try to retransmit the response
    // on the next connection event.
    if (HCI_EXT_ConnEventNoticeCmd(pMsg->connHandle, selfEntity,
                                   SBP_CONN_EVT_END_EVT) == SUCCESS)
    {
      // First free any pending response
      SimpleBLEPeripheral_freeAttRsp(FAILURE);

      // Hold on to the response message for retransmission
      pAttRsp = pMsg;

      // Don't free the response message yet
      return (FALSE);
    }
  }
  else if (pMsg->method == ATT_FLOW_CTRL_VIOLATED_EVENT)
  {
    // ATT request-response or indication-confirmation flow control is
    // violated. All subsequent ATT requests or indications will be dropped.
    // The app is informed in case it wants to drop the connection.

    // Display the opcode of the message that caused the violation.
	  System_printf("FC Violated:%d\n", pMsg->msg.flowCtrlEvt.opcode);
  }
  else if (pMsg->method == ATT_MTU_UPDATED_EVENT)
  {
    // MTU size updated
    System_printf("MTU Size:%d\n", pMsg->msg.mtuEvt.MTU);
  } else if (pMsg->method == ATT_HANDLE_VALUE_NOTI) {
	  System_printf("GATT handle value notification\n");
  } else if (pMsg->method == ATT_HANDLE_VALUE_IND) {
	  System_printf("GATT handle value indication\n");
  } else if (pMsg->method == ATT_HANDLE_VALUE_CFM) {
	  SB_Error error;
	  if (NoError != (error = SB_currentReadingsRead())) {
		  System_printf("Error handling GATT value confirmation: %d\n", error);
	  }
  } else {
	  System_printf("Unknown GATT MSG: %d\n", pMsg->method);
  }

  // Free message payload. Needed only for ATT Protocol messages
  GATT_bm_free(&pMsg->msg, pMsg->method);

  // It's safe to free the incoming message
  return (TRUE);
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_sendAttRsp
 *
 * @brief   Send a pending ATT response message.
 *
 * @param   none
 *
 * @return  none
 */
static void SimpleBLEPeripheral_sendAttRsp(void)
{
  // See if there's a pending ATT Response to be transmitted
  if (pAttRsp != NULL)
  {
    uint8_t status;

    // Increment retransmission count
    rspTxRetry++;

    // Try to retransmit ATT response till either we're successful or
    // the ATT Client times out (after 30s) and drops the connection.
    status = GATT_SendRsp(pAttRsp->connHandle, pAttRsp->method, &(pAttRsp->msg));
    if ((status != blePending) && (status != MSG_BUFFER_NOT_AVAIL))
    {
      // Disable connection event end notice
      HCI_EXT_ConnEventNoticeCmd(pAttRsp->connHandle, selfEntity, 0);

      // We're done with the response message
      SimpleBLEPeripheral_freeAttRsp(status);
    }
    else
    {
      // Continue retrying
    	System_printf("Rsp send retry: %d\n", rspTxRetry);
    }
  }
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_freeAttRsp
 *
 * @brief   Free ATT response message.
 *
 * @param   status - response transmit status
 *
 * @return  none
 */
static void SimpleBLEPeripheral_freeAttRsp(uint8_t status)
{
  // See if there's a pending ATT response message
  if (pAttRsp != NULL)
  {
    // See if the response was sent out successfully
    if (status == SUCCESS)
    {
    	System_printf("Rsp sent, retry:%d\n", rspTxRetry);
    }
    else
    {
      // Free response payload
      GATT_bm_free(&pAttRsp->msg, pAttRsp->method);

      System_printf("Rsp retry failed:%d\n", rspTxRetry);
    }

    // Free response message
    ICall_freeMsg(pAttRsp);

    // Reset our globals
    pAttRsp = NULL;
    rspTxRetry = 0;
  }
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_processAppMsg
 *
 * @brief   Process an incoming callback from a profile.
 *
 * @param   pMsg - message to process
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_processAppMsg(sbpEvt_t *pMsg)
{
  switch (pMsg->hdr.event)
  {
    case SBP_STATE_CHANGE_EVT:
      SimpleBLEPeripheral_processStateChangeEvt((gaprole_States_t)pMsg->hdr.state);
      break;

    case SBP_CHAR_CHANGE_EVT:
      SimpleBLEPeripheral_processCharValueChangeEvt(pMsg->hdr.state);
      break;

    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_stateChangeCB
 *
 * @brief   Callback from GAP Role indicating a role state change.
 *
 * @param   newState - new state
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_stateChangeCB(gaprole_States_t newState)
{
  SimpleBLEPeripheral_enqueueMsg(SBP_STATE_CHANGE_EVT, newState);
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_processStateChangeEvt
 *
 * @brief   Process a pending GAP Role state change event.
 *
 * @param   newState - new state
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_processStateChangeEvt(gaprole_States_t newState)
{
	uint8_t error;
  switch ( newState )
  {
    case GAPROLE_STARTED:
      {
        uint8_t ownAddress[B_ADDR_LEN];
        GAPRole_GetParameter(GAPROLE_BD_ADDR, ownAddress);

        // Display device address
        System_printf(Util_convertBdAddr2Str(ownAddress));
        System_printf("BLE Initialized.\n");
      }
      break;

    case GAPROLE_ADVERTISING:
    	System_printf("BLE Advertising\n");
      break;

    case GAPROLE_CONNECTED:
      {
    	  bleConnected = true;
        uint8_t peerAddress[B_ADDR_LEN];

        GAPRole_GetParameter(GAPROLE_CONN_BD_ADDR, peerAddress);

        System_printf("BLE Connected %s\n", Util_convertBdAddr2Str(peerAddress));
      }
      break;

    case GAPROLE_CONNECTED_ADV:
    	System_printf("BLE Connected Advertising\n");
      break;

    case GAPROLE_WAITING:
    	bleConnected = false;
    	if (0 != (error = SB_Profile_ClearNotificationState())) {
			System_printf("Clearing gatt notification state failed! %d\n", error);
		}
      SimpleBLEPeripheral_freeAttRsp(bleNotConnected);

      System_printf("BLE Disconnected\n");
      break;

    case GAPROLE_WAITING_AFTER_TIMEOUT:
    	bleConnected = false;
    	if (0 != (error = SB_Profile_ClearNotificationState())) {
			System_printf("Clearing gatt notification state failed! %d\n", error);
		}
      SimpleBLEPeripheral_freeAttRsp(bleNotConnected);

      System_printf("BLE Timed Out\n");
      break;

    case GAPROLE_ERROR:
    	System_printf("BLE Error\n");
      break;

    default:
    	System_printf("\n");
      break;
  }
}

bool SB_bleConnected() {
	return bleConnected;
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_charValueChangeCB
 *
 * @brief   Callback from Simple Profile indicating a characteristic
 *          value change.
 *
 * @param   paramID - parameter ID of the value that was changed.
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_charValueChangeCB(uint8_t paramID)
{
  SimpleBLEPeripheral_enqueueMsg(SBP_CHAR_CHANGE_EVT, paramID);
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_processCharValueChangeEvt
 *
 * @brief   Process a pending Simple Profile characteristic value change
 *          event.
 *
 * @param   paramID - parameter ID of the value that was changed.
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_processCharValueChangeEvt(uint8_t paramID)
{
	uint8_t newValue[4];

	switch(paramID)
	{
		case SB_CHARACTERISTIC_SYSTEMTIME:
			// TODO: Integrate this with the rest of the system, and the storage mechanism
			SB_Profile_GetParameter(SB_CHARACTERISTIC_SYSTEMTIME, &newValue, 4);

			System_printf("System time set: %d\n", *(uint32_t*)newValue);
			break;

		case SB_CHARACTERISTIC_READINGCOUNT:
			SB_Profile_GetParameter(SB_CHARACTERISTIC_SYSTEMTIME, &newValue, 2);

			System_printf("Readings read.\n Reading count set: %d\n", *(uint16_t*)newValue);

			SB_currentReadingsRead();

		default:
			// should not reach here!
			break;
	}
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   state - message state.
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_enqueueMsg(uint8_t event, uint8_t state)
{
  sbpEvt_t *pMsg;

  // Create dynamic pointer to message.
  if ((pMsg = ICall_malloc(sizeof(sbpEvt_t))))
  {
    pMsg->hdr.event = event;
    pMsg->hdr.state = state;

    // Enqueue the message.
    Util_enqueueMsg(appMsgQueue, sem, (uint8*)pMsg);
  }
}

/*********************************************************************
*********************************************************************/
