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
 * Project:      Flash Programming Functions for 
 *               Micron M29EW Parallel NOR Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.0.0
 *    Initial release
 */ 


#include "..\FlashOS.H"        // FlashOS Structures

#define M8(adr)  (*((volatile unsigned char  *) (adr)))
#define M16(adr) (*((volatile unsigned short *) (adr)))
#define M32(adr) (*((volatile unsigned long  *) (adr)))

#define ADDR_SHIFT    (1)  /* sjift address according data width ? */

#define __DSB() do {\
                   __schedule_barrier();\
                   __dsb(0xF);\
                   __schedule_barrier();\
                } while (0U)

#define NOR_WRITE(ADDR, DATA) do{ \
                                 M16((ADDR)) = (DATA); \
                                 __DSB(); \
                               } while(0)


union fsreg {                  // Flash Status Register
  struct b  {
    unsigned int q0:1;
    unsigned int q1:1;
    unsigned int q2:1;        // alternative toggle bit
    unsigned int q3:1;        // erase timer bit
    unsigned int q4:1;
    unsigned int q5:1;        // error bit
    unsigned int q6:1;        // toggle bit
    unsigned int q7:1;        // data polling bit
  } b;
  unsigned int v;
} fsr;

unsigned long base_adr;


/*
 * Check if Program/Erase completed using the toggle bit
 *    Parameter:      adr:  Block Start Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int Polling (unsigned long adr) {
  unsigned int q6;

  fsr.v = M16(adr);
  q6 = fsr.b.q6;
  do {
    fsr.v = M16(adr);
    if (fsr.b.q6 == q6) return (0);  // Done
    q6 = fsr.b.q6;
    // if (fsr.b.q3 == 1) break;        // VPP not high enough
  } while (fsr.b.q5 == 0);           // Check for Timeout
  fsr.v = M16(adr);
  q6 = fsr.b.q6;
  fsr.v = M16(adr);
  if (fsr.b.q6 == q6) return (0);    // Done
  M16(adr) = 0xF0;                   // Reset Device
  return (1);                        // Failed
}


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {
  volatile unsigned int dummy;

  base_adr = adr;

  /* Enable GPIOD, GPIOE, GPIOF, GPIOG clock  RCC_AHB1ENR */
  M32(0x40023830U) = M32(0x40023830U) | 0x00000078U;
  dummy = M32(0x40023830U);                                  /* Delay after RCC peripheral clock enable */

  M32(0x40020C20U) = 0xCCCC00CCU;                            /* GPIOD_AFRL    AF12    D2..D3, NOE, NWG, NWAIT, NE1 */
  M32(0x40020C24U) = 0xCCCCCCCCU;                            /* GPIOD_AFRH    AF12    D13..D15, A16..A18, D0..D1 */
  M32(0x40020C00U) = 0xAAAAAA0AU;                            /* GPIOD_MODER   Alternate Function */
  M32(0x40020C08U) = 0xFFFFFF0FU;                            /* GPIOD_OSPEEDR Very High speed */
  M32(0x40020C04U) = 0x00000000U;                            /* GPIOD_OTYPER  Output push-pull */
  M32(0x40020C0CU) = 0x55555505U;                            /* GPIOD_PUPDR   Pull-up */

  M32(0x40021020U) = 0xCCCCCC00U;                            /* GPIOE_AFRL    AF12    A19..A23 */
  M32(0x40021024U) = 0xCCCCCCCCU;                            /* GPIOE_AFRH    AF12    D4..D12 */
  M32(0x40021000U) = 0xAAAAAAA0U;                            /* GPIOE_MODER   Alternate Function */
  M32(0x40021008U) = 0xFFFFFFF0U;                            /* GPIOE_OSPEEDR Very High speed */
  M32(0x40021004U) = 0x00000000U;                            /* GPIOD_OTYPER  Output push-pull */
  M32(0x4002100CU) = 0x55555550U;                            /* GPIOD_PUPDR   Pull-up */

  M32(0x40021420U) = 0x00CCCCCCU;                            /* GPIOF_AFRL    AF12    A0..A5 */
  M32(0x40021424U) = 0xCCCC0000U;                            /* GPIOF_AFRH    AF12    A6..A9 */
  M32(0x40021400U) = 0xAA000AAAU;                            /* GPIOF_MODER   Alternate Function */
  M32(0x40021408U) = 0xFF000FFFU;                            /* GPIOF_OSPEEDR Very High speed */
  M32(0x40021404U) = 0x00000000U;                            /* GPIOD_OTYPER  Output push-pull */
  M32(0x4002140CU) = 0x55000555U;                            /* GPIOD_PUPDR   Pull-up */

  M32(0x40021820U) = 0x00CCCCCCU;                            /* GPIOG_AFRL    AF12    A10..A15 */
  M32(0x40021800U) = 0x00000AAAU;                            /* GPIOG_MODER   Alternate Function */
  M32(0x40021808U) = 0x00000FFFU;                            /* GPIOG_OSPEEDR Very High speed */
  M32(0x40021804U) = 0x00000000U;                            /* GPIOD_OTYPER  Output push-pull */
  M32(0x4002180CU) = 0x00000555U;                            /* GPIOD_PUPDR   Pull-up */

  /* Enable FMC clock  RCC_AHB3ENR */
  M32(0x40023838U) = M32(0x40023838U) | 0x00000001U;
  dummy = M32(0x40023838U);                                  /* Delay after RCC peripheral clock enable */

  /* Configure FMC */
  M32(0xA0000000U) = 0x0010B0D8U;                            /* NOR control IF */
  M32(0xA0000004U) = 0x00110834U;                            /* NOR timing */
  M32(0xA0000104U) = 0x0FFFFFFFU;                            /* NOR extended timing */
  M32(0xA0000000U) = M32(0xA0000000U) | 0x00000001U;         /* enable bank1 */
  dummy = M32(0xA0000000U);

  return (0);
}



/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
  return (0);
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {

  // Start Chip Erase Command
  NOR_WRITE((base_adr + (0x0555 << ADDR_SHIFT)), 0xAA);
  NOR_WRITE((base_adr + (0x02AA << ADDR_SHIFT)), 0x55);
  NOR_WRITE((base_adr + (0x0555 << ADDR_SHIFT)), 0x80);
  NOR_WRITE((base_adr + (0x0555 << ADDR_SHIFT)), 0xAA);
  NOR_WRITE((base_adr + (0x02AA << ADDR_SHIFT)), 0x55);
  NOR_WRITE((base_adr + (0x0555 << ADDR_SHIFT)), 0x10);

  return (Polling(base_adr));  // Wait until Erase completed
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {

  // Start Erase Sector Command
  NOR_WRITE((base_adr + (0x0555 << ADDR_SHIFT)), 0xAA);
  NOR_WRITE((base_adr + (0x02AA << ADDR_SHIFT)), 0x55);
  NOR_WRITE((base_adr + (0x0555 << ADDR_SHIFT)), 0x80);
  NOR_WRITE((base_adr + (0x0555 << ADDR_SHIFT)), 0xAA);
  NOR_WRITE((base_adr + (0x02AA << ADDR_SHIFT)), 0x55);
  NOR_WRITE((adr), 0x30);

  return (Polling(adr));       // Wait until Erase completed
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
  int i;

  for (i = 0; i < ((sz+1)/2); i++)  {
    // Start Program Command
    NOR_WRITE((base_adr + (0x0555 << ADDR_SHIFT)), 0xAA);
    NOR_WRITE((base_adr + (0x02AA << ADDR_SHIFT)), 0x55);
    NOR_WRITE((base_adr + (0x0555 << ADDR_SHIFT)), 0xA0);
    NOR_WRITE((adr), *((unsigned short *) buf));
    if (Polling(adr) != 0) return (1);
    buf += 2;
    adr += 2;
  }
  return (0);
}
