/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file contains the low level macros and definition for evk527 board
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

#ifndef EVK_527_H
#define EVK_527_H

//_____ I N C L U D E S ____________________________________________________
#include "config.h"

#if (TARGET_BOARD==EVK527)

#include "lib_mcu/flash/flash_drv.h"   // Because pin JTAG multiplexage

//_____ M A C R O S ________________________________________________________

//! @defgroup EVK_527_module evk527
//! This module contains low level hardware abstraction layer for evk527 board
//! @image html evk_527.gif
//! @{


      //! @defgroup EVK_527_leds Leds Management
      //! Macros to manage Leds on evk527
      //! @{
#define  LED_PORT       PORTD
#define  LED_DDR        DDRD
#define  LED_PIN        PIND
                    
#define  LED0_BIT       PIND0
#define  LED1_BIT       PIND1

        
#define  Leds_init()    (LED_DDR  |=  (1<<LED0_BIT) | (1<<LED1_BIT))
#define  Led0_init()    (LED_DDR  |=  (1<<LED0_BIT))
#define  Led1_init()    (LED_DDR  |=  (1<<LED1_BIT))

#define  Leds_on()      (LED_PORT &= ~((1<<LED0_BIT) | (1<<LED1_BIT) ))
#define  Leds_off()     (LED_PORT |=  (1<<LED0_BIT) | (1<<LED1_BIT) )//(LED_PORT &= ~((1<<LED0_BIT) | (1<<LED1_BIT) ))
#define  Leds_set_val(val) (Leds_off(),LED_PORT |= (val<<5)&((1<<LED0_BIT) | (1<<LED1_BIT) ))
#define  Leds_get_val()    (LED_PORT>>5)


#define  Led0_on()      (LED_PORT |= (1<<LED0_BIT))
#define  Led1_off()      (LED_PORT |= (1<<LED1_BIT))

        
#define  Led0_off()     (LED_PORT &= ~(1<<LED0_BIT))
#define  Led1_on()     (LED_PORT &= ~(1<<LED1_BIT))

        
#define  Led0_toggle()  (LED_PIN |= (1<<LED0_BIT))
#define  Led1_toggle()  (LED_PIN |= (1<<LED1_BIT))

        
#define  Is_led0_on()   (LED_PIN & (1<<LED0_BIT) ? TRUE : FALSE)
#define  Is_led1_on()   (LED_PIN & (1<<LED1_BIT) ? TRUE : FALSE)





#endif   // TARGET_BOARD==EVK527

#endif   // EVK_527_H
