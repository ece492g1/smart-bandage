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
#define SB_FLASH_PAGE_HDR_OFFSET		 4

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
} SB_FlashHeader;

/*********************************************************************
 * Forward defines
 */
SB_Error writeBuf(uint8 * buf, uint8 count, SB_FLASH_PAGE_T page, SB_FLASH_OFFSET_T offset);
SB_Error writeAligned(uint8 * buf, uint8 count, SB_FLASH_PAGE_T page, SB_FLASH_OFFSET_T offset);
void SBFlashRead(uint8 pg, uint16 offset, uint8 *buf, uint16 cnt);
//SB_Error SBFlashWrite(SB_FLASH_POINTER_T address, uint8 *buf, uint16 cnt);
SB_Error SBFlashWrite(uint8 pg, uint16 offset, uint8 *buf, uint16 count);
static void enableFlashCache( uint8 state );
static uint8 disableFlashCache();

/*********************************************************************
 * Local variables
 */

// The variable marks the location of the first piece of NV ram
// dedicated to saving data
#pragma DATA_SECTION(firstFlashByte, ".sb_nv_mem")
const uint8 firstFlashByte = 0x51;

SB_Error SB_flashInit(uint8 readingSizeBytes, bool reinit) {SB_FlashHeader header;
	SB_Error result;
#ifdef SB_DEBUG
	System_printf("SB Flash NV Storage Config:\n SB Flash Page No: %d\n SB Flash Base Addr: %x\n SB Flash Num Pages: %d\n SB Flash Last Page: %d\n SB Flash Last Addr: %x.\n Sector size: %d\n Reading Size (bytes): %d\n",
			SB_FLASH_PAGE_FIRST,
			SB_FLASH_BEGIN_ADDR,
			SB_FLASH_NUM_PAGES,
			SB_FLASH_PAGE_LAST,
			SB_FLASH_END_ADDR,
			FlashSectorSizeGet(),
			readingSizeBytes); //SB_FLASH_PAGE_LAST);
	System_flush();
#endif

	// Check for an existing header in flash memory
	SBFlashRead(SB_FLASH_PAGE_FIRST, SB_FLASH_PAGE_HDR_OFFSET, (uint8*)&header, sizeof(SB_FlashHeader));

	if (header.marker == SB_FLASH_MARKER) {
		uint32 totalSize = header.entryCount * header.readingSizeBytes;
		if (header.readingSizeBytes != readingSizeBytes || totalSize >= (uint32)(SB_FLASH_NUM_PAGES * SB_FLASH_PAGE_SIZE)) {
			// If the reading size has changed or the data is too large to fit into memory
			// invalidate the old data and move to the end of the previous region for flash leveling
			uint8 pageDiff = (header.startOffset + header.entryCount*header.readingSizeBytes) / SB_FLASH_PAGE_SIZE;
			header.startPage += pageDiff;
			header.startOffset = (header.startOffset + header.entryCount * header.readingSizeBytes) % SB_FLASH_PAGE_SIZE;
			header.entryCount = 0;
			header.readingSizeBytes = readingSizeBytes;
		}
	} else {
		// Initialize new header at start page and offset after the header position
		header.marker = SB_FLASH_MARKER;
		header.startPage = SB_FLASH_PAGE_FIRST;
		header.startOffset = SB_FLASH_PAGE_HDR_SIZE;
		header.entryCount = 0;
		header.readingSizeBytes = readingSizeBytes;
	}

#ifndef SB_FLASH_NO_INIT_WRITE
	// Write the header to the header position

	result = writeBuf((uint8*)&header, sizeof(header), SB_FLASH_PAGE_FIRST, SB_FLASH_PAGE_HDR_OFFSET);
	if (result != NoError) {
#ifdef SB_DEBUG
		System_printf("Flash header write failed: %d\n", result);
		System_flush();
#endif
		return result;
	}
#endif

#ifdef SB_FLASH_SANITY_CHECKS
	{
		// Sanity check
		SB_FlashHeader checkHeader;
		SBFlashRead(SB_FLASH_PAGE_FIRST, SB_FLASH_PAGE_HDR_OFFSET, (uint8*)&checkHeader, sizeof(SB_FlashHeader));

		if (header.marker != checkHeader.marker) {
#ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header marker wrote %x, but read %x at address %x\n", header.marker, checkHeader.marker, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#endif
			return SanityCheckFailed;
		}

		if (header.entryCount != checkHeader.entryCount) {
#ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header entryCount wrote %x, but read %x at address %x\n", header.entryCount, checkHeader.entryCount, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#endif
			return SanityCheckFailed;
		}

		if (header.startPage != checkHeader.startPage) {
#ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header readingSizeBytes wrote %x, but read %x at address %x\n", header.startPage, checkHeader.startPage, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#endif
			return SanityCheckFailed;
		}

		if (header.startOffset != checkHeader.startOffset) {
#ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header readingSizeBytes wrote %x, but read %x at address %x\n", header.startOffset, checkHeader.startOffset, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#endif
			return SanityCheckFailed;
		}

		if (header.timestamp != checkHeader.timestamp) {
#ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header readingSizeBytes wrote %x, but read %x at address %x\n", header.timestamp, checkHeader.timestamp, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#endif
			return SanityCheckFailed;
		}

		if (header.readingSizeBytes != checkHeader.readingSizeBytes) {
#ifdef SB_DEBUG
			System_printf("Flash failed sanity check! Header readingSizeBytes wrote %x, but read %x at address %x\n", header.readingSizeBytes, checkHeader.readingSizeBytes, SB_FLASH_BEGIN_ADDR); //SB_FLASH_PAGE_LAST);
			System_flush();
#endif
			return SanityCheckFailed;
		}
	}
#endif

	return NoError;
}

SB_Error SB_flashWriteReadings(void * readings);

SB_FLASH_COUNT_T SB_flashReadingCount();

SB_Error SB_flashGetReading(SB_FLASH_COUNT_T index, void * reading);

SB_Error SB_flashPrepShutdown();

// Write the buffer after 4-byte aligning it
// This code assumes that `count` is a `uint8` - which means that it does not have to check
// for crossing more than one page/bank boundary. To increase the size of count would require
// more advanced boundary checking.
SB_Error writeBuf(uint8 * buf, uint8 count, SB_FLASH_PAGE_T page, SB_FLASH_OFFSET_T offset) {
	uint8 i, alignmentBuf[SB_FLASH_WORD_SIZE];

	// If the data is not word-aligned at the start
	if (offset % SB_FLASH_WORD_SIZE != 0) {
		// Read the data within the same word but before the current write into the buf
		SBFlashRead(page, offset - (offset % SB_FLASH_WORD_SIZE), alignmentBuf, SB_FLASH_WORD_SIZE);

		// Add the non-aligned start bits to the alignmentBuffer
		for (i = 0; i < (SB_FLASH_WORD_SIZE - (offset % SB_FLASH_WORD_SIZE)) && i < count; ++i) {
			alignmentBuf[i + offset % SB_FLASH_WORD_SIZE] = buf[i];
		}

		SBFlashWrite(page, offset, alignmentBuf, SB_FLASH_WORD_SIZE);

		offset += i;
		buf    += i;
		count  -= i;

		if (offset >= SB_FLASH_PAGE_SIZE) {
			++page;
		}
	}

	// If the data is not word-aligned at the end
	if ((offset + count) % SB_FLASH_WORD_SIZE != 0) {
		int32  startOffset = offset; // Signed on purpose
		SB_FLASH_POINTER_T startPage = page;

		// It is only possible to cross a single page boundary because `count` is a `uint8`
		if ((startOffset + count) >= SB_FLASH_PAGE_SIZE) {
			++startPage;
			startOffset -= SB_FLASH_PAGE_SIZE;
		}

		SB_FLASH_POINTER_T startAddr = startOffset + count - ((startOffset + count) % SB_FLASH_WORD_SIZE);

		// Read the data within the same word but before the current write into the buf
		SBFlashRead(startPage, startAddr, alignmentBuf, SB_FLASH_WORD_SIZE);

		// Add the non-aligned start bits to the alignmentBuffer
		for (i = 0; i < (SB_FLASH_WORD_SIZE - (offset % SB_FLASH_WORD_SIZE)) && i < count; ++i) {
			alignmentBuf[i] = buf[count - (offset % SB_FLASH_WORD_SIZE) + i];
		}

		SBFlashWrite(startPage, startAddr, alignmentBuf, SB_FLASH_WORD_SIZE);

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
		// If applicable adjust the count so that we don't go over a memory bank boundary
		if (((address + thisCount) / SB_FLASH_PAGE_SIZE) / SB_FLASH_BANK_PAGE_COUNT > currentMemoryBank) {
			thisCount = SB_FLASH_BANK_SIZE * (currentMemoryBank + 1) - address;
		}

		result = SBFlashWrite(page, offset, buf, thisCount);
		if (result != NoError) {
			return result;
		}

		// Increment current state
		writtenBytes += thisCount;
		address += thisCount;

		// Go to the next bank if the initial write would have crossed the boundary
		if (thisCount < count) {
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
