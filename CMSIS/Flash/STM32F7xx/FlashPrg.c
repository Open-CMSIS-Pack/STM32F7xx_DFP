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
 * Project:      Flash Programming Functions for STMicroelectronics STM32F7xx
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.1.0
 *    Reworked Algorithms
 *  Version 1.0.0
 *    Initial release
 */

#include "FlashOS.h"        // FlashOS Structures

typedef volatile unsigned char    vu8;
typedef          unsigned char     u8;
typedef volatile unsigned short   vu16;
typedef          unsigned short    u16;
typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

// Peripheral Memory Map
#define IWDG_BASE         0x40003000
#define FLASH_BASE        0x40023C00

#define IWDG            ((IWDG_TypeDef *) IWDG_BASE)
#define FLASH           ((FLASH_TypeDef*) FLASH_BASE)

// Independent WATCHDOG
typedef struct {
  vu32 KR;
  vu32 PR;
  vu32 RLR;
  vu32 SR;
} IWDG_TypeDef;

// Flash Registers
typedef struct {
  vu32 ACR;
  vu32 KEYR;
  vu32 OPTKEYR;
  vu32 SR;
  vu32 CR;
  vu32 OPTCR;
  vu32 OPTCR1;
  vu32 OPTCR2;
} FLASH_TypeDef;


// Flash Keys
#define RDPRT_KEY       0x00A5
#define FLASH_KEY1      0x45670123
#define FLASH_KEY2      0xCDEF89AB
#define FLASH_OPTKEY1   0x08192A3B
#define FLASH_OPTKEY2   0x4C5D6E7F

// Flash Control Register definitions
#define FLASH_PG                ((unsigned int)0x00000001)
#define FLASH_SER               ((unsigned int)0x00000002)
#define FLASH_MER               ((unsigned int)0x00000004)
#define FLASH_SNB_POS           ((unsigned int)0x00000003)
#define FLASH_SNB_MSK           ((unsigned int)0x000000F8)
#define FLASH_PSIZE_POS         ((unsigned int)0x00000008)
#define FLASH_PSIZE_MSK         ((unsigned int)0x00000300)
#define FLASH_MERB              ((unsigned int)0x00008000)
#define FLASH_STRT              ((unsigned int)0x00010000)
#define FLASH_LOCK              ((unsigned int)0x80000000)

// Flash Option Control Register definitions
#define FLASH_OPTLOCK           ((unsigned int)0x00000001)
#define FLASH_OPTSTRT           ((unsigned int)0x00000002)


#define FLASH_PSIZE_Byte        ((unsigned int)0x00000000)
#define FLASH_PSIZE_HalfWord    ((unsigned int)0x00000100)
#define FLASH_PSIZE_Word        ((unsigned int)0x00000200)
#define FLASH_PSIZE_DoubleWord  ((unsigned int)0x00000300)
#define CR_PSIZE_MASK           ((unsigned int)0xFFFFFCFF)


// Flash Status Register definitions
#define FLASH_EOP               ((unsigned int)0x00000001)
#define FLASH_OPERR             ((unsigned int)0x00000002)
#define FLASH_WRPERR            ((unsigned int)0x00000010)
#define FLASH_PGAERR            ((unsigned int)0x00000020)
#define FLASH_PGPERR            ((unsigned int)0x00000040)
#define FLASH_PGSERR            ((unsigned int)0x00000080)
#define FLASH_BSY               ((unsigned int)0x00010000)

#define FLASH_PGERR             (FLASH_PGSERR | FLASH_PGPERR | FLASH_PGAERR | FLASH_WRPERR)

void DSB(void) {
    __asm("DSB");
}


/*
 * Get Sector Number
 *    Parameter:      adr:  Sector Address
 *    Return Value:   Sector Number
 */
#if defined FLASH_MEM
unsigned long GetSecNum (unsigned long adr) {
  unsigned long n;

#if   defined STM32F7x_2048 || defined STM32F7x_1024
  n = (adr >> 12) & 0x001FF;                            // Only bits 12..20

  if      (n >= 0x40) {
    n = 4 + (n >> 6);                                   // 256kB Sectors
  }
  else if (n >= 0x20) {
    n = 3 + (n >> 5);                                   // 128KB Sector
  }
  else                {
    n = 0 + (n >> 3);                                   //  64kB Sectors
  }

#elif defined STM32F7x_2048dual || defined STM32F7x_1024dual || defined STM32F7x2_512
#if defined STM32F7x_1024dual
  n = (adr >> 12) & 0x0007F;                            // Only bits 12..18
#else
  n = (adr >> 12) & 0x000FF;                            // Only bits 12..19
#endif

  if      (n >= 0x20) {
    n = 4 + (n >> 5);                                   // 128kB Sector
  }
  else if (n >= 0x10) {
    n = 3 + (n >> 4);                                   //  64kB Sector
  }
  else                {
    n = 0 + (n >> 2);                                   //  16kB Sector
  }

#if   defined STM32F7x_2048dual
  if (adr & 0x00100000) {
    n |= (1U << 4);                                     // Sector in second half, bit 4 selects the bank
  }
#elif defined STM32F7x_1024dual
  if (adr & 0x00080000) {
    n |= (1U << 4);                                     // Sector in second half, bit 4 selects the bank
  }
#endif
#elif defined STM32F75x_64 || STM32F75x_64_TCM
 n = (adr >> 12) & 0x0007F;  
n = 0 + (n >> 3);  
#else
  #error device not defined
#endif

  return (n);                                           // Sector Number
}
#endif // FLASH_MEM


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM || defined FLASH_OTP
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  FLASH->KEYR = FLASH_KEY1;                             // Unlock Flash
  FLASH->KEYR = FLASH_KEY2;

  FLASH->ACR  = 0x00000000;                             // Zero Wait State, no Cache, no Prefetch
  FLASH->SR  |= FLASH_PGERR;                            // Reset Error Flags

  if ((FLASH->OPTCR & 0x20) == 0x00) {                  // Test if IWDG is running (IWDG in HW mode)
    // Set IWDG time out to ~32.768 second
    IWDG->KR  = 0x5555;                                 // Enable write access to IWDG_PR and IWDG_RLR
    IWDG->PR  = 0x06;                                   // Set prescaler to 256
    IWDG->RLR = 4095;                                   // Set reload value to 4095
  }

  return (0);                                           // Done
}
#endif // FLASH_MEM || FLASH_OTP

#ifdef FLASH_OPT
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  FLASH->OPTKEYR  = FLASH_OPTKEY1;                      // Unlock Option Bytes
  FLASH->OPTKEYR  = FLASH_OPTKEY2;

  FLASH->SR      |= FLASH_PGERR;                        // Reset Error Flags

  if ((FLASH->OPTCR & 0x20) == 0x00) {                  // Test if IWDG is running (IWDG in HW mode)
    // Set IWDG time out to ~32.768 second
    IWDG->KR  = 0x5555;                                 // Enable write access to IWDG_PR and IWDG_RLR
    IWDG->PR  = 0x06;                                   // Set prescaler to 256
    IWDG->RLR = 4095;                                   // Set reload value to 4095
  }

  return (0);                                           // Done
}
#endif // FLASH_OPT


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM || defined FLASH_OTP
int UnInit (unsigned long fnc) {

  FLASH->CR |=  FLASH_LOCK;                             // Lock Flash

  return (0);                                           // Done
}
#endif // FLASH_MEM || FLASH_OTP

#ifdef FLASH_OPT
int UnInit (unsigned long fnc) {

  FLASH->OPTCR |= FLASH_OPTLOCK;                        // Lock Option Bytes

  return (0);                                           // Done
}
#endif // FLASH_OPT


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM
int EraseChip (void) {

  FLASH->CR |=  FLASH_MER;

#if defined STM32F7x_2048dual || defined STM32F7x_1024dual
  FLASH->CR |=  FLASH_MERB;
#endif

  FLASH->CR |=  FLASH_STRT;                             // Start Erase

  while (FLASH->SR & FLASH_BSY) {
    IWDG->KR = 0xAAAA;                                  // Reload IWDG
  }

  FLASH->CR &= ~FLASH_MER;                              // Mass Erase Disabled

  return (0);                                           // Done
}
#endif // FLASH_MEM

#ifdef FLASH_OPT
int EraseChip (void) {

  FLASH->SR    |= FLASH_PGERR;                          // Reset Error Flags

#if defined STM32F7x2_512
//  FLASH->OPTCR2 = 0x800000FF;                           // Default value
  FLASH->OPTCR2 = 0x00000000;
#endif
  FLASH->OPTCR1 = 0x00400080;                           // Default value
  FLASH->OPTCR  = 0xFFFFAAFC;                           // Default value without LOCK (reserved bits are ignored)
//FLASH->OPTCR |= 0x00FF0000;                           // Reset Write protection
//FLASH->OPTCR |= 0x0000AA00;                           // Reset Read protection
//FLASH->OPTCR |= 0x000000E0;                           // Reset User Option bytes
//FLASH->OPTCR |= 0x0000000C;                           // Reset BOR level
  FLASH->OPTCR |= FLASH_OPTSTRT;                        // Program values
  DSB();

  while (FLASH->SR & FLASH_BSY) {
    IWDG->KR = 0xAAAA;                                  // Reload IWDG
  }

  if (FLASH->SR & FLASH_PGERR) {                        // Check for Error
    FLASH->SR |= FLASH_PGERR;                           // Reset Error Flags
    return (1);                                         // Failed
  }

  return (0);                                           // Done
}
#endif // FLASH_OPT


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseSector (unsigned long adr) {
  unsigned long n;

  n = GetSecNum(adr);                                   // Get Sector Number
  FLASH->SR |= FLASH_PGERR;                             // Reset Error Flags

  FLASH->CR  =  FLASH_SER;                              // Sector Erase Enabled
  FLASH->CR |=  ((n << FLASH_SNB_POS) & FLASH_SNB_MSK); // Sector Number
  FLASH->CR |=  FLASH_STRT;                             // Start Erase
  DSB();

  while (FLASH->SR & FLASH_BSY) {
    IWDG->KR = 0xAAAA;                                  // Reload IWDG
  }

  FLASH->CR &= ~FLASH_SER;                              // Page Erase Disabled

  if (FLASH->SR & FLASH_PGERR) {                        // Check for Error
    FLASH->SR |= FLASH_PGERR;                           // Reset Error Flags
    return (1);                                         // Failed
  }
  return (0);                                           // Done
}
#endif // FLASH_MEM

#if defined FLASH_OPT || defined FLASH_OTP
int EraseSector (unsigned long adr) {
  /* erase sector is not needed for
     Flash Option bytes
     Flash One Time Probrammable bytes
  */
  return (0);                                           // Done
}
#endif


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
#ifdef FLASH_MEM
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  sz = (sz + 3) & ~3;                                   // Adjust size for Words

  FLASH->SR |= FLASH_PGERR;                             // Reset Error Flags
  FLASH->CR  =  0;                                      // Reset CR

  while (sz) {
    FLASH->CR |= (FLASH_PG              |               // Programming Enabled
                  FLASH_PSIZE_Word);                    // Programming Enabled (Word)

    M32((adr & 0x001FFFFF) | 0x08000000) = *((u32 *)buf);  // Program Double Word
    DSB();

    while (FLASH->SR & FLASH_BSY){
      IWDG->KR = 0xAAAA;                                // Reload IWDG
    }

    FLASH->CR &= ~FLASH_PG;                             // Programming Disabled

    if (FLASH->SR & FLASH_PGERR) {                      // Check for Error
      FLASH->SR |= FLASH_PGERR;                         // Reset Error Flags
      return (1);                                       // Failed
    }

    adr += 4;                                           // Go to next Word
    buf += 4;
    sz  -= 4;
  }

  return (0);                                           // Done
}
#endif // FLASH_MEM

#if defined FLASH_OTP
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  sz = (sz + 3) & ~3;                                   // Adjust size for Words

  FLASH->SR |= FLASH_PGERR;                             // Reset Error Flags
  FLASH->CR  =  0;                                      // Reset CR

  while (sz) {
    FLASH->CR |= (FLASH_PG              |               // Programming Enabled
                  FLASH_PSIZE_Word);                    // Programming Enabled (Word)

#if defined  STM32F7x2_512
    M32((adr & 0x000007FF) | 0x1FF07800) = *((u32 *)buf);  // Program Double Word
#else
    M32((adr & 0x00000FFF) | 0x1FF0F000) = *((u32 *)buf);  // Program Double Word
#endif
    DSB();

    while (FLASH->SR & FLASH_BSY){
      IWDG->KR = 0xAAAA;                                // Reload IWDG
    }

    FLASH->CR &= ~FLASH_PG;                             // Programming Disabled

    if (FLASH->SR & FLASH_PGERR) {                      // Check for Error
      FLASH->SR |= FLASH_PGERR;                         // Reset Error Flags
      return (1);                                       // Failed
    }

    adr += 4;                                           // Go to next Word
    buf += 4;
    sz  -= 4;
  }

  return (0);                                           // Done
}
#endif  // FLASH_OTP

#ifdef FLASH_OPT
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  u32 optcr;
  u32 optcr1;
#if defined STM32F7x2_512
  u32 optcr2;
#endif

  optcr  = *((u32 *)(buf + 0));
  optcr1 = *((u32 *)(buf + 4));
#if defined STM32F7x2_512
  optcr2 = *((u32 *)(buf + 8));
#endif

  FLASH->SR    |= FLASH_PGERR;                          // Reset Error Flags

#if defined STM32F7x2_512
  FLASH->OPTCR2 = (optcr2 & 0x800000FF);                // Write OPTCR2 values
#endif
  FLASH->OPTCR1 = (optcr1             );                // Write OPTCR1 values
#if defined STM32F7x_2048dual
  FLASH->OPTCR  = (optcr  & 0xFFFFFFFC);                // Write OPTCR values
#else
  FLASH->OPTCR  = (optcr  & 0xC0FFFFFC);                // Write OPTCR values
#endif
  FLASH->OPTCR |= FLASH_OPTSTRT;                        // Program values
  DSB();

  while (FLASH->SR & FLASH_BSY) {
    IWDG->KR = 0xAAAA;                                  // Reload IWDG
  }

  if (FLASH->SR & FLASH_PGERR) {                        // Check for Error
    FLASH->SR |= FLASH_PGERR;                           // Reset Error Flags
    return (1);                                         // Failed
  }

  return (0);                                           // Done
}
#endif //  FLASH_OPT


/*
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */

#ifdef FLASH_OPT
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf) {
  u32 optcr;
  u32 optcr1;
#if defined  STM32F7x2_512
  u32 optcr2;
#endif

  optcr  = *((u32 *)(buf + 0));
  optcr1 = *((u32 *)(buf + 4));
#if defined STM32F7x2_512
  optcr2 = *((u32 *)(buf + 8));
#endif

  /* check FLASH_OPTCR */
#if defined STM32F7x_2048dual
  if ((optcr  & 0xFFFFFFFC) != (FLASH->OPTCR  & 0xFFFFFFFC))
#else
  if ((optcr  & 0xC0FFFFFC) != (FLASH->OPTCR  & 0xC0FFFFFC))
#endif
  {
    return (adr + 0);
  }

  /* check FLASH_OPTCR1 */
  if ((optcr1             ) != (FLASH->OPTCR1             )) {
    return (adr + 1);
  }

#if defined STM32F7x2_512
  /* check FLASH_OPTCR2 */
  if ((optcr2  & 0x800000FF) != (FLASH->OPTCR2 & 0x800000FF)) {
    return (adr + 2);
  }
#endif

  return (adr + sz);
}
#endif // FLASH_OPT
