/*
 * flash.h
 *
 *  Created on: Mar 2, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_FLASH_H_
#define APPLICATION_FLASH_H_

#include "hci_tl.h"
#include "Board.h"
#include "readingsManager.h"

#define SB_FLASH_COUNT_T   		uint32_t
#define SB_FLASH_PAGE_T    		uint16_t
#define SB_FLASH_OFFSET_T  		uint16_t
#define SB_FLASH_POINTER_T 		uint32_t
#define SB_FLASH_READING_TYPE	SB_PeripheralReadings

/*********************************************************************
 * @fn      SB_flashInit
 *
 * @brief   Initialize flash storage
 *
 * @param   readingSizeBytes - the size of a block of readings in bytes
 *
 * @param   reinit			 - true to force reinitialize and drop all items in flash mem.
 *
 * @return  NoError if properly initialized, otherwise the error that occured
 */
SB_Error SB_flashInit(uint8 readingSizeBytes, bool reinit);

/*********************************************************************
 * @fn      SB_flashWriteReadings
 *
 * @brief   Write a block of readings to flash storage
 *
 * @param   readings        - The readings to write to flash memory.
 * 							  Must be the size of readingSizeBytes given in SB_flashInit()
 *
 * @return  NoError if properly written, otherwise the error
 */
SB_Error SB_flashWriteReadings(SB_FLASH_READING_TYPE * readings);

/*********************************************************************
 * @fn      SB_flashReadingCount
 *
 * @brief   Get the number of readings stored in memory
 *
 * @return  The reading count
 */
SB_FLASH_COUNT_T SB_flashReadingCount();

/*********************************************************************
 * @fn      SB_flashWriteReadings
 *
 * @brief   Get the number of readings stored in memory
 *
 * @return  NoError if properly written, otherwise the error
 */
const SB_FLASH_COUNT_T* SB_flashReadingCountRef();

/*********************************************************************
 * @fn      SB_flashGetReading
 *
 * @brief   Gets a reading from flash storage
 *
 * @param   index           - The index of the reading to get
 *
 * @param   readings        - Pointer to the memory location where the reading should be placed
 * 							  Memory location must be at least readingSizeBytes as specified in SB_flashInit()
 *
 * @return  NoError if properly read, otherwise the error. If error `reading` will be NULL.
 */
SB_Error SB_flashGetReading(SB_FLASH_COUNT_T index, SB_FLASH_READING_TYPE * reading, uint32_t * refTimestamp);

/*********************************************************************
 * @fn      SB_flashReadNext
 *
 * @brief   Gets the next flash reading from storage and removes the entry returned
 *
 * @param   readings        - Pointer to the memory location where the reading should be placed
 * 							  Memory location must be at least readingSizeBytes as specified in SB_flashInit()
 *
 * @return  NoError if properly read, otherwise the error. If error `reading` will be NULL.
 */
SB_Error SB_flashReadNext(SB_FLASH_READING_TYPE * reading, uint32_t * refTimestamp);

/*********************************************************************
 * @fn      SB_flashPrepShutdown
 *
 * @brief   Synchronizes all data to the flash in preparation for a powerdown where SRAM is not preserved.
 *
 * @return  NoError if properly read, otherwise the error
 */
SB_Error SB_flashPrepShutdown();

/*********************************************************************
 * @fn      SB_flashTimeSet
 *
 * @brief   Notifies the flash module that the system time has been updated.
 *
 * @return  NoError if the value is properly updated
 */
SB_Error SB_flashTimeSet();

/*********************************************************************
 * @fn      SB_flashGetReferenceTime
 *
 * @brief   Gets the current flash reference time.
 *
 * @return  The current flash timestamp
 */
uint32_t SB_flashGetReferenceTime();

/*********************************************************************
 * @fn      SB_flashHasTime
 *
 * @brief   Checks if the flash module has a time set
 *
 * @return  True if the time is currently set
 */
bool SB_flashHasTime();

#endif /* APPLICATION_FLASH_H_ */
