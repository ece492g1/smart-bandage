/*
 * flash.c
 *
 *  Created on: Mar 2, 2016
 *      Author: michaelblouin
 */

#include "hal_flash.h"
#include "hal_types.h"
#include <driverlib/vims.h>
#include <driverlib/flash.h>
#include <xdc/runtime/System.h>

#include "clock.h"

#include "flash.h"

/*********************************************************************
 * CONSTANTS
 */

// NV page configuration
#define SB_FLASH_BEGIN_ADDR              ((SB_FLASH_POINTER_T)&firstFlashByte)
#define SB_FLASH_END_ADDR                ((SB_FLASH_POINTER_T)(SB_FLASH_BEGIN_ADDR + (SB_FLASH_NUM_PAGES * SB_FLASH_PAGE_SIZE) - 1))
#define SB_FLASH_PAGE_SIZE               HAL_FLASH_PAGE_SIZE
#define SB_FLASH_NUM_PAGES               SB_NV_FLASH_PAGES
#define SB_FLASH_PAGE_FIRST				 (SB_FLASH_BEGIN_ADDR/SB_FLASH_PAGE_SIZE)
#define SB_FLASH_PAGE_LAST				 ((SB_FLASH_BEGIN_ADDR + (SB_FLASH_NUM_PAGES * SB_FLASH_PAGE_SIZE))/SB_FLASH_PAGE_SIZE - 1)

// The CC26xx has 4 32kB flash memory banks
#define SB_FLASH_BANK_PAGE_COUNT		 8
#define SB_FLASH_BANK_SIZE               (SB_FLASH_BANK_PAGE_COUNT * SB_FLASH_PAGE_SIZE)

#define SB_FLASH_READINGS_AREA_BEGIN_ADDR (SB_FLASH_BEGIN_ADDR + SB_FLASH_PAGE_HDR_SIZE)
#define SB_FLASH_READINGS_AREA_End_ADDR   SB_FLASH_END_ADDR

// NV page header size in bytes
#define SB_FLASH_PAGE_HDR_SIZE           sizeof(SB_FlashHeader)
#define SB_FLASH_PAGE_HDR_OFFSET		 0

// Length in bytes of a flash word
#define SB_FLASH_WORD_SIZE               HAL_FLASH_WORD_SIZE

#define SB_FLASH_MARKER					 0x5150
#define SB_FLASH_MARKER_SIZE			 uint16

/*********************************************************************
 * TYPEDEFS
 */
typedef struct {
	SB_FLASH_MARKER_SIZE marker;
	SB_FLASH_COUNT_T     entryCount;
	SB_FLASH_PAGE_T  	 startPage;
	SB_FLASH_OFFSET_T    startOffset;
	SB_TIMESTAMP_T		 timestamp;
	uint8 				 readingSizeBytes;

	/*
	 * Used to point to the next instance of the header that is saved whenever the peripheral readies for power down.
	 * This is because a write to flash is performed as a logical AND -- meaning it is not possible to set a bit to `1`
	 * after it is written `0`. Therefore, we keep this value as all 1's until we have to save power state at which point
	 * we write the address of the next header position. This forms a sort of linked-list of headers throughout the data.
	 */
	SB_FLASH_PAGE_T   nextHeaderPage;
	SB_FLASH_OFFSET_T nextHeaderOffset;
} SB_FlashHeader;

/*********************************************************************
 * Forward defines
 */
SB_Error writeBuf(uint8 * buf, uint8 count, SB_FLASH_PAGE_T page, SB_FLASH_OFFSET_T offset);
SB_Error writeAligned(uint8 * buf, uint8 count, SB_FLASH_PAGE_T page, SB_FLASH_OFFSET_T offset);
void SBFlashRead(uint8 pg, uint16 offset, uint8 *buf, uint16 cnt);
//SB_Error SBFlashWrite(SB_FLASH_POINTER_T address, uint8 *buf, uint16 cnt);
SB_Error SB_flashGetFirstReading(SB_FLASH_READING_TYPE *reading, uint32_t *refTimestamp);
SB_Error SBFlashWrite(uint8 pg, uint16 offset, uint8 *buf, uint16 count);
SB_Error erasePage(uint8 pg);
static void enableFlashCache( uint8 state );
static uint8 disableFlashCache();

/*********************************************************************
 * Local variables
 */

// The variable marks the location of the first piece of NV ram
// dedicated to saving data
#pragma DATA_SECTION(firstFlashByte, ".sb_nv_mem")
const uint8 firstFlashByte = 0x51;

SB_FlashHeader header;

SB_Error loadNextHeader(SB_FLASH_PAGE_T pg, SB_FLASH_OFFSET_T offset, SB_FlashHeader *target, uint8 readingSizeBytes) {
	SB_Error result;
	if (NULL == target) {
		return InvalidParameter;
	}

	// Check for an existing header in flash memory
	SBFlashRead(SB_FLASH_PAGE_FIRST, SB_FLASH_PAGE_HDR_OFFSET, (uint8*)target, sizeof(SB_FlashHeader));

	// Check for invalid data in the loaded header. If its find reinitialize it and erase the first page
	uint32 totalSize = target->entryCount * target->readingSizeBytes;
	if (target->marker != SB_FLASH_MARKER || (target->readingSizeBytes != readingSizeBytes || totalSize >= (uint32)(SB_FLASH_NUM_PAGES * SB_FLASH_PAGE_SIZE))) {
		// Initialize new header at start page and offset after the header position
		target->marker = SB_FLASH_MARKER;
		target->startPage = SB_FLASH_PAGE_FIRST;
		target->startOffset = SB_FLASH_PAGE_HDR_SIZE;
		target->entryCount = 0;
		target->readingSizeBytes = readingSizeBytes;
		target->nextHeaderPage = ~0;
		target->nextHeaderOffset = ~0;

		if (SB_clockIsSet()) {
			target->timestamp = SB_clockGetTime();
		} else {
			target->timestamp = UINT32_MAX;
		}

		if (NoError != (result = erasePage(SB_FLASH_PAGE_FIRST))) {
			return result;
		}
	}

	return NoError;
}

SB_Error SB_flashTimeSet() {
	if (header.timestamp != UINT32_MAX) {
		// Time was already set
		return NoError;
	}

	header.timestamp = SB_clockGetTime();

	// Adjust the timestamp so that reading times are relevant to it.
	SB_FLASH_READING_TYPE readingBuf;
	SB_Error result = SB_flashGetFirstReading(&readingBuf, NULL);
	if (result == NoDataAvailable) {
		return NoError;
	} else if (result != NoError) {
		return result;
	}

	header.timestamp -= readingBuf.timeDiff;

	return NoError;
}

inline uint32_t SB_flashGetReferenceTime() {
	if (header.timestamp == UINT32_MAX) {
		return 0;
	}

	return header.timestamp;
}

SB_Error SB_flashInit(uint8 readingSizeBytes, bool reinit) {
	SB_Error result;
#ifdef SB_DEBUG
	System_printf("SB Flash NV Storage Config:\n SB Flash Page No: %d\n SB Flash Base Addr: %x\n SB Flash Num Pages: %d\n SB Flash Last Page: %d\n SB Flash Last Addr: %x.\n Sector size: %d\n Reading Size (bytes): %d\n",
			SB_FLASH_PAGE_FIRST,
			SB_FLASH_BEGIN_ADDR,
			SB_FLASH_NUM_PAGES,
			SB_FLASH_PAGE_LAST,
			SB_FLASH_END_ADDR,
			FlashSectorSizeGet(),
			readingSizeBytes);
	System_flush();
#endif

	// Load the first header in the header chain
	if (NoError != (result = loadNextHeader(SB_FLASH_PAGE_FIRST, SB_FLASH_PAGE_HDR_OFFSET, &header, readingSizeBytes))) {
		return result;
	}

	// Continue loading until we get to the last header
	while (header.nextHeaderPage != ((SB_FLASH_PAGE_T) ~0) && header.nextHeaderOffset != ((SB_FLASH_OFFSET_T) ~0)) {
		if (NoError != (result = loadNextHeader(header.nextHeaderPage, header.nextHeaderOffset, &header, readingSizeBytes))) {
			return result;
		}
	}

#ifndef SB_FLASH_NO_INIT_WRITE
	// Write the header to the header position

	result = writeBuf((uint8*)&header, sizeof(header), SB_FLASH_PAGE_FIRST, SB_FLASH_PAGE_HDR_OFFSET);
	if (result != NoError) {
# ifdef SB_DEBUG
		System_printf("Flash header write failed: %d\n", result);
		System_flush();
# endif
		return result;
	}

# ifdef SB_FLASH_SANITY_CHECKS
	{
		// Sanity check
		SB_FlashHeader checkHeader;
		SBFlashRead(SB_FLASH_PAGE_FIRST, SB_FLASH_PAGE_HDR_OFFSET, (uint8*)&checkHeader, sizeof(SB_FlashHeader));

		if (header.marker != checkHeader.marker) {
#  ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header marker wrote %x, but read %x at address %x\n", header.marker, checkHeader.marker, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#  endif
			return SanityCheckFailed;
		}

		if (header.entryCount != checkHeader.entryCount) {
#  ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header entryCount wrote %x, but read %x at address %x\n", header.entryCount, checkHeader.entryCount, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#  endif
			return SanityCheckFailed;
		}

		if (header.startPage != checkHeader.startPage) {
#  ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header readingSizeBytes wrote %x, but read %x at address %x\n", header.startPage, checkHeader.startPage, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#  endif
			return SanityCheckFailed;
		}

		if (header.startOffset != checkHeader.startOffset) {
#  ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header readingSizeBytes wrote %x, but read %x at address %x\n", header.startOffset, checkHeader.startOffset, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#  endif
			return SanityCheckFailed;
		}

		if (header.timestamp != checkHeader.timestamp) {
#  ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header readingSizeBytes wrote %x, but read %x at address %x\n", header.timestamp, checkHeader.timestamp, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#  endif
			return SanityCheckFailed;
		}

		if (header.readingSizeBytes != checkHeader.readingSizeBytes) {
#  ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header readingSizeBytes wrote %x, but read %x at address %x\n", header.readingSizeBytes, checkHeader.readingSizeBytes, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#  endif
			return SanityCheckFailed;
		}

		if (header.nextHeaderPage != checkHeader.nextHeaderPage) {
#  ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header nextHeaderPage wrote %x, but read %x at address %x\n", header.nextHeaderPage, checkHeader.nextHeaderPage, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#  endif
			return SanityCheckFailed;
		}

		if (header.nextHeaderOffset != checkHeader.nextHeaderOffset) {
#  ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header nextHeaderOffset wrote %x, but read %x at address %x\n", header.nextHeaderOffset, checkHeader.nextHeaderOffset, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#  endif
			return SanityCheckFailed;
		}
	}
# endif
#endif

	return NoError;
}
//TODO: This function not yet working accross page boundaries
// Should write a function to check if a write will cross a page boundary, and if yes
// erase the next page.
SB_Error SB_flashWriteReadings(SB_FLASH_READING_TYPE * readings) {
	SB_Error result;

	if (NULL == readings) {
		return InvalidParameter;
	}

	SB_FLASH_PAGE_T page = header.startPage + (header.entryCount * header.readingSizeBytes + header.startOffset) / SB_FLASH_PAGE_SIZE;
	SB_FLASH_OFFSET_T offset = (header.entryCount * header.readingSizeBytes + header.startOffset) % SB_FLASH_PAGE_SIZE;

	// We're on a new, unused page - erase it to ensure we can write to it
	if (offset < header.readingSizeBytes && page > header.startPage) {
		if (NoError != (result = erasePage(page))) {
			return result;
		}
	}

	// Write the data
	if (NoError != (result = writeBuf((uint8_t*)readings, header.readingSizeBytes, page, offset))) {
		return result;
	}

	// Increment the entry count
	++header.entryCount;

	return NoError;
}

SB_FLASH_COUNT_T SB_flashReadingCount() {
	return header.entryCount;
}

const SB_FLASH_COUNT_T* SB_flashReadingCountRef() {
	return &header.entryCount;
}

SB_Error SB_flashGetFirstReading(SB_FLASH_READING_TYPE *reading, uint32_t *refTimestamp) {
	if (header.entryCount == 0) {
		return NoDataAvailable;
	}

	SBFlashRead(header.startPage, header.startOffset, (uint8_t*)reading, header.readingSizeBytes);

	if (NULL != refTimestamp) {
		*refTimestamp = header.timestamp;
	}

	return NoError;
}

SB_Error SB_flashReadNext(SB_FLASH_READING_TYPE * reading, uint32_t * refTimestamp) {
	SB_flashGetFirstReading(reading, refTimestamp);

	header.startOffset += header.readingSizeBytes;
	if (header.startOffset >= SB_FLASH_PAGE_SIZE) {
		header.startOffset -= header.readingSizeBytes;
		header.startPage = (header.startPage + 1) % SB_FLASH_NUM_PAGES;
	}

	if (--header.entryCount == 0) {
		// There are now no entries stored. Clear flash memory and reset.
		uint8_t i;
		for (i = 0; i < SB_FLASH_NUM_PAGES; ++i) {
			erasePage(i + SB_FLASH_PAGE_FIRST);
		}

		// After performing page erases this will reset the header to default values
		return loadNextHeader(SB_FLASH_PAGE_FIRST, 0, &header, header.readingSizeBytes);
	}

	return NoError;
}

SB_Error SB_flashPrepShutdown();

// Write the buffer after 4-byte aligning it
// This code assumes that `count` is a `uint8` - which means that it does not have to check
// for crossing more than one page/bank boundary. To increase the size of count would require
// more advanced boundary checking.
SB_Error writeBuf(uint8 * buf, uint8 count, SB_FLASH_PAGE_T page, SB_FLASH_OFFSET_T offset) {
	uint8 i, alignmentBuf[SB_FLASH_WORD_SIZE];
	SB_Error error;

	// If the data is not word-aligned at the start
	if (offset % SB_FLASH_WORD_SIZE != 0) {
		// Read the data within the same word but before the current write into the buf
		SBFlashRead(page, offset - (offset % SB_FLASH_WORD_SIZE), alignmentBuf, SB_FLASH_WORD_SIZE);

		// Add the non-aligned start bits to the alignmentBuffer
		for (i = 0; i < (SB_FLASH_WORD_SIZE - (offset % SB_FLASH_WORD_SIZE)) && i < count; ++i) {
			alignmentBuf[i + (offset % SB_FLASH_WORD_SIZE)] = buf[i];
		}

		if (NoError != (error = SBFlashWrite(page, offset - (offset % SB_FLASH_WORD_SIZE), alignmentBuf, SB_FLASH_WORD_SIZE))) {
			return error;
		}

		offset += i;
		buf    += i;
		count  -= i;

		if (offset >= SB_FLASH_PAGE_SIZE) {
			++page;
		}
	}

	// If the data is not word-aligned at the end
	if ((offset + count) % SB_FLASH_WORD_SIZE != 0) {
		SB_FLASH_OFFSET_T  startOffset = offset + count - ((offset + count) % SB_FLASH_WORD_SIZE);
		SB_FLASH_PAGE_T    startPage = page;

		// It is only possible to cross a single page boundary because `count` is a `uint8`
		if ((offset + count) >= SB_FLASH_PAGE_SIZE) {
			++startPage;
			startOffset -= SB_FLASH_PAGE_SIZE;
		}

		// Read the data within the same word but before the current write into the buf
		SBFlashRead(startPage, startOffset, alignmentBuf, SB_FLASH_WORD_SIZE);

		// Add the non-aligned start bits to the alignmentBuffer
		for (i = 0; i < (SB_FLASH_WORD_SIZE - ((offset + count) % SB_FLASH_WORD_SIZE)) && i < count; ++i) {
			alignmentBuf[i] = buf[count - ((offset + count) % SB_FLASH_WORD_SIZE) + i];
		}

		SBFlashWrite(startPage, startOffset, alignmentBuf, SB_FLASH_WORD_SIZE);

		count  -= i;
	}

	// At this point the data is aligned at the start and the end
	if (count > 0) {
		return writeAligned(buf, count, page, offset);
	}

	return NoError;
}

// Write a buffer that is 4-byte aligned and both the start and end
// It is best to call writeBuf() rather than this function as it will first ensure that the write
// you are trying to perform is properly aligned.
//
// This code assumes that `count` is a `uint8` - which means that it does not have to check
// for crossing more than one page/bank boundary. To increase the size of count would require
// more advanced boundary checking.
SB_Error writeAligned(uint8 * buf, uint8 count, SB_FLASH_PAGE_T page, SB_FLASH_OFFSET_T offset) {
	SB_FLASH_COUNT_T thisCount = count, writtenBytes = 0;
	SB_FLASH_POINTER_T address = page * SB_FLASH_PAGE_SIZE + offset;
	uint8 currentMemoryBank = page / SB_FLASH_BANK_PAGE_COUNT;
	SB_Error result;

	while (writtenBytes < count) {
		bool bankOverrun = false, pageOverrun = false;
		// If applicable adjust the count so that we don't go over a memory bank boundary
//		if (((address + thisCount) / SB_FLASH_PAGE_SIZE) / SB_FLASH_BANK_PAGE_COUNT > currentMemoryBank) {
//			thisCount = SB_FLASH_BANK_SIZE * (currentMemoryBank + 1) - address;
//			bankOverrun = true;
//		}

		// If applicable adjust the count so that we don't go over a page bank boundary
		if (SB_FLASH_PAGE_SIZE * ((SB_FLASH_POINTER_T)page + 1) <= address) {
			// Write starts on the next page
			++page;
			offset -= SB_FLASH_PAGE_SIZE;
		} else if ((address + thisCount) / SB_FLASH_PAGE_SIZE > page) {
			// The write starts on one page and extends over the boundary. Cut into smaller writes
			thisCount = SB_FLASH_PAGE_SIZE * ((SB_FLASH_POINTER_T)page + 1) - address;
			pageOverrun = true;
		}

		result = SBFlashWrite(page, offset, buf, thisCount);
		if (result != NoError) {
			return result;
		}

		// Increment current state
		writtenBytes += thisCount;
		address += thisCount;

		// Go to the next bank if the initial write would have crossed the boundary
		if (pageOverrun) {
			++page;
		} else if (thisCount < count) {
			++currentMemoryBank;
		}
	}

	return NoError;
}

void SBFlashRead(uint8 pg, uint16 offset, uint8 *buf, uint16 cnt) {
	halIntState_t cs;

	// Calculate the offset into the containing flash bank as it gets mapped into XDATA.
	uint8 *ptr = (uint8*)(pg * SB_FLASH_PAGE_SIZE + offset);

	// Enter Critical Section.
	HAL_ENTER_CRITICAL_SECTION(cs);

	// Read data.
	while (cnt--)
	{
		*buf++ = *ptr++;
	}

	// Exit Critical Section.
	HAL_EXIT_CRITICAL_SECTION(cs);
}

uint8* getAddress( uint8 pg, uint16 offset )
{
#ifndef FEATURE_OAD
  // Calculate the offset into the containing flash bank as it gets mapped into XDATA.
  uint8 *flashAddr = (uint8 *)(offset + HAL_NV_START_ADDR) + ((pg % HAL_NV_PAGE_BEG )* HAL_FLASH_PAGE_SIZE);

  return flashAddr;
#else //FEATURE_OAD
  // The actual address is a 4-KiloByte multiple of the page number plus offset in bytes.
  return (uint8*)((pg << 12) + offset);
#endif //FEATURE_OAD
}

SB_Error SBFlashWrite(uint8 pg, uint16 offset, uint8 *buf, uint16 count) {
	uint32 result = 0;
	halIntState_t cs;
	uint32 addr = (offset) + ((pg % HAL_NV_PAGE_BEG )* HAL_FLASH_PAGE_SIZE);

	if ((count % SB_FLASH_WORD_SIZE) != 0) {
		return InvalidParameter;
	}

	// Enter Critical Section.
	HAL_ENTER_CRITICAL_SECTION(cs);

	uint8 state = disableFlashCache();

	// Write the data
	result |= FlashProgram( buf, addr, count );

	enableFlashCache(state);

	// Exit Critical Section.
	HAL_EXIT_CRITICAL_SECTION(cs);

	switch (result) {
	case FAPI_STATUS_SUCCESS:
		break;
	case FAPI_STATUS_INCORRECT_DATABUFFER_LENGTH:
		return InvalidParameter;
	case FAPI_STATUS_FSM_ERROR:
	default:
		return UnknownError;
	}

	return NoError;
}

static void enableFlashCache ( uint8 state )
{
  if ( state != VIMS_MODE_DISABLED )
  {
    // Enable the Cache.
    VIMSModeSet( VIMS_BASE, VIMS_MODE_ENABLED );
  }
}

static uint8 disableFlashCache ( void )
{
  uint8 state = VIMSModeGet( VIMS_BASE );

  // Check VIMS state
  if ( state != VIMS_MODE_DISABLED )
  {
    // Invalidate cache
    VIMSModeSet( VIMS_BASE, VIMS_MODE_DISABLED );

    // Wait for disabling to be complete
    while ( VIMSModeGet( VIMS_BASE ) != VIMS_MODE_DISABLED );

  }

  return state;
}

SB_Error erasePage(uint8 pg) {
	uint32 result = 0;
	halIntState_t cs;
	uint32 addr = ((pg % HAL_NV_PAGE_BEG )* HAL_FLASH_PAGE_SIZE);

	// Enter Critical Section.
	HAL_ENTER_CRITICAL_SECTION(cs);

	uint8 state = disableFlashCache();

	// Write the data
	result = FlashSectorErase(addr);

	enableFlashCache(state);

	// Exit Critical Section.
	HAL_EXIT_CRITICAL_SECTION(cs);

	switch (result) {
	case FAPI_STATUS_SUCCESS:
		break;
	case FAPI_STATUS_INCORRECT_DATABUFFER_LENGTH:
		return InvalidParameter;
	case FAPI_STATUS_FSM_ERROR:
	default:
		return UnknownError;
	}

	return NoError;
}
