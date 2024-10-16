/* -----------------------------------------------------------------------------
 * Copyright (c) 2017 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software. Permission is granted to anyone to use this 
 * software for any purpose, including commercial applications, and to alter 
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not 
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be 
 *    appreciated but is not required. 
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be 
 *    misrepresented as being the original software. 
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *   
 *
 * $Date:        03. January 2017
 * $Revision:    V1.0.0
 *  
 * Project:      Flash Device Description for 
 *               Micron M29EW Parallel NOR Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.0.0
 *    Initial release
 */ 

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef PC28F128M29EWxx
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "PC28F128M29EWxx_STM32756G-EVAL", // Device Name 
   EXT16BIT,                   // Device Type
   0x60000000,                 // Device Start Address
   0x01000000,                 // Device Size in Bytes (16MB)
   4096,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x20000, 0x000000,          // Sector Size 128kB (128 Sectors)
   SECTOR_END

};
#endif
