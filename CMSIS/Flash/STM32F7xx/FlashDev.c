/* -----------------------------------------------------------------------------
 * Copyright (c) 2015 - 2017 ARM Ltd.
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
 * $Date:        21. March 2017
 * $Revision:    V1.1.0
 *
 * Project:      Flash Device Description for STMicroelectronics STM32F7xx
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.1.0
 *    Reworked Algorithms
 *  Version 1.0.0
 *    Initial release
 */

#include "FlashOS.h"        // FlashOS Structures

#if defined FLASH_MEM
#if defined STM32F75x_64
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
      "STM32F75x 64K AXI Flash",     // Device Name
           ONCHIP,                             // Device Type
      0x08000000,                         
      0x00010000,                         // Device Size in Bytes (64K)
      1024,                               // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      1000,                               // Program Page Timeout 1000 mSec
      6000,                               // Erase Sector Timeout 6000 mSec
      // Specify Size and Address of Sectors
      0x08000, 0x000000,                  // Sector Size  32kB (2 Sectors)
      
      SECTOR_END
  };
#endif // STM32F7x2_512
#if defined STM32F75x_64_TCM
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
      "STM32F75x 64K TCM Flash",     // Device Name
           ONCHIP,                             // Device Type
      0x00200000,                         
      0x00010000,                         // Device Size in Bytes (64K)
      1024,                               // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      1000,                               // Program Page Timeout 1000 mSec
      6000,                               // Erase Sector Timeout 6000 mSec
      // Specify Size and Address of Sectors
      0x08000, 0x000000,                  // Sector Size  32kB (2 Sectors)
      
      SECTOR_END
  };
#endif // STM32F7x2_512
#if defined STM32F7x_2048
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
    #ifdef FLASH_TCM
      "STM32F7x TCM 2MB Flash",           // Device Name
      ONCHIP,                             // Device Type
      0x00200000,                         // Device Start Address
    #else
      "STM32F7x 2MB Flash",               // Device Name
      ONCHIP,                             // Device Type
      0x08000000,                         // Device Start Address
    #endif
      0x00200000,                         // Device Size in Bytes (2048kB)
      1024,                               // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      1000,                               // Program Page Timeout 1000 mSec
      6000,                               // Erase Sector Timeout 6000 mSec
      // Specify Size and Address of Sectors
      0x08000, 0x000000,                  // Sector Size  32kB (4 Sectors)
      0x20000, 0x020000,                  // Sector Size 128kB (1 Sectors)
      0x40000, 0x040000,                  // Sector Size 256kB (7 Sectors)
      SECTOR_END
  };
#endif // STM32F7x_2048

#if defined STM32F7x_2048dual
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
    #ifdef FLASH_TCM
      "STM32F7x TCM dual bank 2MB Flash", // Device Name
      ONCHIP,                             // Device Type
      0x00200000,                         // Device Start Address
    #else
      "STM32F7x dual bank 2MB Flash",     // Device Name
      ONCHIP,                             // Device Type
      0x08000000,                         // Device Start Address
    #endif
      0x00200000,                         // Device Size in Bytes (2048kB)
      1024,                               // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      1000,                               // Program Page Timeout 1000 mSec
      6000,                               // Erase Sector Timeout 6000 mSec
      // Specify Size and Address of Sectors
      0x04000, 0x000000,                  // Sector Size  16kB (4 Sectors)
      0x10000, 0x010000,                  // Sector Size  64kB (1 Sectors)
      0x20000, 0x020000,                  // Sector Size 128kB (7 Sectors)
      0x04000, 0x100000,                  // Sector Size  16kB (4 Sectors)
      0x10000, 0x110000,                  // Sector Size  64kB (1 Sectors)
      0x20000, 0x120000,                  // Sector Size 128kB (7 Sectors)
      SECTOR_END
  };
#endif // STM32F7x_2048dual

#if defined STM32F7x_1024
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
    #ifdef FLASH_TCM
      "STM32F7x TCM 1MB Flash",           // Device Name
      ONCHIP,                             // Device Type
      0x00200000,                         // Device Start Address
    #else
      "STM32F7x 1MB Flash",               // Device Name
      ONCHIP,                             // Device Type
      0x08000000,                         // Device Start Address
    #endif
      0x00100000,                         // Device Size in Bytes (1024kB)
      1024,                               // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      1000,                               // Program Page Timeout 1000 mSec
      6000,                               // Erase Sector Timeout 6000 mSec
      // Specify Size and Address of Sectors
      0x08000, 0x000000,                  // Sector Size  32kB (4 Sectors)
      0x20000, 0x020000,                  // Sector Size 128kB (1 Sectors)
      0x40000, 0x040000,                  // Sector Size 256kB (3 Sectors)
      SECTOR_END
  };
#endif // STM32F7x_1024

#if defined STM32F7x_1024dual
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
    #ifdef FLASH_TCM
      "STM32F7x TCM dual bank 1MB Flash", // Device Name
      ONCHIP,                             // Device Type
      0x00200000,                         // Device Start Address
    #else
      "STM32F7x dual bank 1MB Flash",     // Device Name
      ONCHIP,                             // Device Type
      0x08000000,                         // Device Start Address
    #endif
      0x00100000,                         // Device Size in Bytes (1024kB)
      1024,                               // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      1000,                               // Program Page Timeout 1000 mSec
      6000,                               // Erase Sector Timeout 6000 mSec
      // Specify Size and Address of Sectors
      0x04000, 0x000000,                  // Sector Size  16kB (4 Sectors)
      0x10000, 0x010000,                  // Sector Size  64kB (1 Sectors)
      0x20000, 0x020000,                  // Sector Size 128kB (3 Sectors)
      0x04000, 0x080000,                  // Sector Size  16kB (4 Sectors)
      0x10000, 0x090000,                  // Sector Size  64kB (1 Sectors)
      0x20000, 0x0A0000,                  // Sector Size 128kB (3 Sectors)
      SECTOR_END
  };
#endif // STM32F7x_1024dual

#if defined STM32F7x2_512
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
    #ifdef FLASH_TCM
      "STM32F72x/3x TCM 512KB Flash",     // Device Name
      ONCHIP,                             // Device Type
      0x00200000,                         // Device Start Address
    #else
      "STM32F72x/3x 512KB Flash",         // Device Name
      ONCHIP,                             // Device Type
      0x08000000,                         // Device Start Address
    #endif
      0x00080000,                         // Device Size in Bytes (512kB)
      1024,                               // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      1000,                               // Program Page Timeout 1000 mSec
      6000,                               // Erase Sector Timeout 6000 mSec
      // Specify Size and Address of Sectors
      0x04000, 0x000000,                  // Sector Size  16kB (4 Sectors)
      0x10000, 0x010000,                  // Sector Size  64kB (1 Sectors)
      0x20000, 0x020000,                  // Sector Size 128kB (3 Sectors)
      SECTOR_END
  };
#endif // STM32F7x2_512

#endif // FLASH_MEM


#if defined FLASH_OTP

#if defined STM32F7x_2048 || defined STM32F7x_2048dual || defined STM32F7x_1024 || defined STM32F7x_1024dual
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
    #ifdef FLASH_TCM
      "STM32F7xx TCM Flash OTP",          // Device Name
      ONCHIP,                             // Device Type
      0x0010F000,                         // Device Start Address
    #else
      "STM32F7xx Flash OTP",              // Device Name
      ONCHIP,                             // Device Type
      0x1FF0F000,                         // Device Start Address
    #endif
      0x00000410,                         // Device Size in Bytes (1040)
      1040,                               // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      3000,                               // Program Page Timeout 3 Sec
      3000,                               // Erase Sector Timeout 3 Sec
      // Specify Size and Address of Sectors
      0x0410, 0x000000,                   // Sector Size 1040B
      SECTOR_END
  };
#endif // STM32F7x_2048

#if defined STM32F7x2_512
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
    #ifdef FLASH_TCM
      "STM32F72x/3x TCM Flash OTP",       // Device Name
      ONCHIP,                             // Device Type
      0x00107800,                         // Device Start Address
    #else
      "STM32F72x/3x Flash OTP",           // Device Name
      ONCHIP,                             // Device Type
      0x1FF07800,                         // Device Start Address
    #endif
      0x00000210,                         // Device Size in Bytes (528)
      528,                                // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      3000,                               // Program Page Timeout 3 Sec
      3000,                               // Erase Sector Timeout 3 Sec
      // Specify Size and Address of Sectors
      0x0210, 0x000000,                   // Sector Size 528B
      SECTOR_END
  };
#endif // STM32F7x2_512

#endif // FLASH_OTP


#if defined FLASH_OPT

#if defined STM32F7x_1024
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
      "STM32F74x/5x Flash Options",       // Device Name
      ONCHIP,                             // Device Type
      0x1FFF0000,                         // Device Start Address
      0x00000008,                         // Device Size in Bytes (8)
      8,                                  // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      3000,                               // Program Page Timeout 3 Sec
      3000,                               // Erase Sector Timeout 3 Sec
      // Specify Size and Address of Sectors
      0x0008, 0x000000,                   // Sector Size 8B
      SECTOR_END
  };
#endif // STM32F7x_1024

#if defined STM32F7x_2048dual
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
      "STM32F76x/7x Flash Options",       // Device Name
      ONCHIP,                             // Device Type
      0x1FFF0000,                         // Device Start Address
      0x00000008,                         // Device Size in Bytes (8)
      8,                                  // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      3000,                               // Program Page Timeout 3 Sec
      3000,                               // Erase Sector Timeout 3 Sec
      // Specify Size and Address of Sectors
      0x0008, 0x000000,                   // Sector Size 8B
      SECTOR_END
  };
#endif // STM32F7x_2048dual

#if defined STM32F7x2_512
  struct FlashDevice const FlashDevice  =  {
      FLASH_DRV_VERS,                     // Driver Version, do not modify!
      "STM32F72x/3x Flash Options",       // Device Name
      ONCHIP,                             // Device Type
      0x1FFF0000,                         // Device Start Address
      0x0000000C,                         // Device Size in Bytes (12)
      12,                                 // Programming Page Size
      0,                                  // Reserved, must be 0
      0xFF,                               // Initial Content of Erased Memory
      3000,                               // Program Page Timeout 3 Sec
      3000,                               // Erase Sector Timeout 3 Sec
      // Specify Size and Address of Sectors
      0x000C, 0x000000,                   // Sector Size 12B
      SECTOR_END
  };
#endif // STM32F7x2_512

#endif // FLASH_OPT
