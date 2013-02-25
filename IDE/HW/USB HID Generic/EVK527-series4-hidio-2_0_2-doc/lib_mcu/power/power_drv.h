/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file contains the Power Management low level driver definition
//!
//!  This module allows to configure the different power mode of the AVR core and
//!  also to setup the the internal clock prescaler
//!
//! - Compiler:           IAR EWAVR and GNU GCC for AVR
//! - Supported devices:  ATmega32U4
//!
//! \author               Atmel Corporation: http://www.atmel.com \n
//!                       Support and FAQ: http://support.atmel.no/
//!
//! ***************************************************************************

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _POWER_DRV_H_
#define _POWER_DRV_H_

#ifdef  __GNUC__
   #include <avr/power.h>
   
   #if (__AVR_LIBC_VERSION__<=10602UL) //for AVRLIBC bug #23871 : clock_prescale_set() not available for ATmega32U4
   typedef enum
   {
       clock_div_1 = 0,
       clock_div_2 = 1,
       clock_div_4 = 2,
       clock_div_8 = 3,
       clock_div_16 = 4,
       clock_div_32 = 5,
       clock_div_64 = 6,
       clock_div_128 = 7,
       clock_div_256 = 8
   } clock_div_t;

   #define clock_prescale_set(x) \
   { \
           uint8_t tmp = _BV(CLKPCE); \
           __asm__ __volatile__ ( \
                   "in __tmp_reg__,__SREG__" "\n\t" \
                   "cli" "\n\t" \
                   "sts %1, %0" "\n\t" \
                   "sts %1, %2" "\n\t" \
                   "out __SREG__, __tmp_reg__" \
                   : /* no outputs */ \
                   : "d" (tmp), \
                     "M" (_SFR_MEM_ADDR(CLKPR)), \
                     "d" (x) \
                   : "r0"); \
   }


   #define clock_prescale_get()  (clock_div_t)(CLKPR & (uint8_t)((1<<CLKPS0)|(1<<CLKPS1)|(1<<CLKPS2)|(1<<CLKPS3)))

   #endif


#endif
//! @defgroup powermode Power management drivers
//!
//! @{

//_____ M A C R O S ________________________________________________________

#define Setup_idle_mode()                        (SMCR=0,SMCR |= (1<<SE))
#define Setup_power_down_mode()                   (SMCR=0,SMCR |= (1<<SE)+(1<<SM1))
#define Setup_adc_noise_reduction_mode()         (SMCR=0,SMCR |= (1<<SE)+(1<<SM0))
#define Setup_power_save_mode()                  (SMCR=0,SMCR |= (1<<SE)+(1<<SM1)+(1<<SM0))
#define Setup_standby_mode()                     (SMCR=0,SMCR |= (1<<SE)+(1<<SM2)+(1<<SM1))
#define Setup_ext_standby_mode()                  (SMCR=0,SMCR |= (1<<SE)+(1<<SM2)+(1<<SM1)+(1<<SM0))

//! Clear_prescaler.
//!
//! This function reset the internal CPU core clock prescaler
//!
//!
//! @param none
//!
//! @return none.
//!
#ifdef  __GNUC__
   #define Clear_prescaler()                       clock_prescale_set(0)
#else
   #define Clear_prescaler()                       Set_cpu_prescaler(0)
#endif

//! Set_prescaler.
//!
//! This function configure the internal CPU core clock prescaler value
//!
//!
//! @param x: prescaler new value
//!
//! @return none.
//!
#ifdef  __GNUC__
   #define Set_cpu_prescaler(x)                        clock_prescale_set(x)
#else
   extern void Set_cpu_prescaler(U8 x);
#endif


#define Sleep_instruction()              {asm("SLEEP");}

//Backward compatibility
#define Set_power_down_mode()              set_power_down_mode()
#define Set_idle_mode()            set_idle_mode()

//_____ D E C L A R A T I O N ______________________________________________

void set_idle_mode(void);
void set_power_down_mode(void);
void set_adc_noise_reduction_mode(void);
void set_power_save_mode(void);
void set_standby_mode(void);
void set_ext_standby_mode(void);

//! Enter_idle_mode.
//!
//! This function makes the AVR core enter idle mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_idle_mode()                 (set_idle_mode())

//! Enter_power_down_mode.
//!
//! This function makes the AVR core enter power down mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_power_down_mode()           (set_power_down_mode())

//! Enter_adc_noise_reduction_mode.
//!
//! This function makes the AVR core enter adc noise reduction mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_adc_noise_reduction_mode()  (set_adc_noise_reduction_mode())

//! Enter_power_save_mode.
//!
//! This function makes the AVR core enter power save mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_power_save_mode()           (set_power_save_mode())

//! Enter_standby_mode.
//!
//! This function makes the AVR core enter standby mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_standby_mode()              (set_standby_mode())

//! Enter_ext_standby_mode.
//!
//! This function makes the AVR core enter extended standby mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_ext_standby_mode()          (set_ext_standby_mode())


//! @}




//! @defgroup clockmode Clock management drivers
//!
//! @{

//_____ M A C R O S ________________________________________________________

// Clock control
#define   Enable_external_clock()       (CLKSEL0 |= (1<<EXTE))
#define   Disable_external_clock()      (CLKSEL0 &= ~(1<<EXTE))
#define   Enable_RC_clock()             (CLKSEL0 |= (1<<RCE))
#define   Disable_RC_clock()            (CLKSEL0 &= ~(1<<RCE))

// Clock state
#define   External_clock_ready()        (((CLKSTA&(1<<EXTON)) != 0) ? TRUE : FALSE)
#define   RC_clock_ready()              (((CLKSTA&(1<<RCON)) != 0) ? TRUE : FALSE)

// Clock selection
#define   Select_external_clock()       (CLKSEL0 |= (1<<CLKS))
#define   Select_RC_clock()             (CLKSEL0 &= ~(1<<CLKS))

// Clock settings : when using a clock source, only the other clock source setting can be modified
// Set the source setting of the next clock source to use before switching to it
#define   Load_ext_clock_config(cfg)    (CLKSEL1 = (CLKSEL1&0xF0) | ((cfg&0x0F)<<EXCKSEL0), \
                                         CLKSEL0 = (CLKSEL0&0xCF) | (((cfg&0x30)>>4)<<EXSUT0))

#define   Load_RC_clock_config(cfg)     (CLKSEL1 = (CLKSEL1&0x0F) | ((cfg&0x0F)<<RCCKSEL0), \
                                         CLKSEL0 = (CLKSEL0&0x3F) | (((cfg&0x30)>>4)<<RCSUT0))

//_____ C L O C K   D E F I N I T I O N S ______________________________________
// Configuration byte defined as SUT<1:0> & CKSEL<3:0> (CKSEL0 is the LSb)

// Interal RC oscillator (frequency between 7.3 and 8.1 MHz)
#define   OSC_INTRC_0MS                  0x02
#define   OSC_INTRC_4MS                  0x12
#define   OSC_INTRC_65MS                 0x22

// External crystal, frequency between 0.3 and 0.9 MHz
#define   OSC_XTAL_RANGE1_258CK_4MS      0x08
#define   OSC_XTAL_RANGE1_258CK_65MS     0x18
#define   OSC_XTAL_RANGE1_1KCK_0MS       0x28
#define   OSC_XTAL_RANGE1_1KCK_4MS       0x38
#define   OSC_XTAL_RANGE1_1KCK_65MS      0x09
#define   OSC_XTAL_RANGE1_16KCK_0MS      0x19
#define   OSC_XTAL_RANGE1_16KCK_4MS      0x29
#define   OSC_XTAL_RANGE1_16KCK_65MS     0x39

// External crystal, frequency between 0.9 and 3 MHz
#define   OSC_XTAL_RANGE2_258CK_4MS      0x0A
#define   OSC_XTAL_RANGE2_258CK_65MS     0x1A
#define   OSC_XTAL_RANGE2_1KCK_0MS       0x2A
#define   OSC_XTAL_RANGE2_1KCK_4MS       0x3A
#define   OSC_XTAL_RANGE2_1KCK_65MS      0x0B
#define   OSC_XTAL_RANGE2_16KCK_0MS      0x1B
#define   OSC_XTAL_RANGE2_16KCK_4MS      0x2B
#define   OSC_XTAL_RANGE2_16KCK_65MS     0x3B

// External crystal, frequency between 3 and 8 MHz
#define   OSC_XTAL_RANGE3_258CK_4MS      0x0C
#define   OSC_XTAL_RANGE3_258CK_65MS     0x1C
#define   OSC_XTAL_RANGE3_1KCK_0MS       0x2C
#define   OSC_XTAL_RANGE3_1KCK_4MS       0x3C
#define   OSC_XTAL_RANGE3_1KCK_65MS      0x0D
#define   OSC_XTAL_RANGE3_16KCK_0MS      0x1D
#define   OSC_XTAL_RANGE3_16KCK_4MS      0x2D
#define   OSC_XTAL_RANGE3_16KCK_65MS     0x3D

// External crystal, frequency between 8 and 16 MHz
#define   OSC_XTAL_RANGE4_258CK_4MS      0x0E
#define   OSC_XTAL_RANGE4_258CK_65MS     0x1E
#define   OSC_XTAL_RANGE4_1KCK_0MS       0x2E
#define   OSC_XTAL_RANGE4_1KCK_4MS       0x3E
#define   OSC_XTAL_RANGE4_1KCK_65MS      0x0F
#define   OSC_XTAL_RANGE4_16KCK_0MS      0x1F
#define   OSC_XTAL_RANGE4_16KCK_4MS      0x2F
#define   OSC_XTAL_RANGE4_16KCK_65MS     0x3F

// External clock
#define   OSC_EXTCLK_0MS                 0x00
#define   OSC_EXTCLK_4MS                 0x10
#define   OSC_EXTCLK_65MS                0x20


//_____ D E C L A R A T I O N ______________________________________________

void Clock_switch_external(void);
void Clock_switch_internal(void);

//! @}


#endif  // _POWER_DRV_H_

