/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file contains the low level macros and definition for the USB PLL
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


#ifndef PLL_DRV_H
#define PLL_DRV_H

//_____ I N C L U D E S ____________________________________________________

//! @defgroup PLL PLL driver
//! PLL Module
//! @{
//_____ M A C R O S ________________________________________________________

   //! @defgroup PLL_macros PLL Macros
   //! These functions allow to control the PLL
   //! @{

#ifndef PLL_OUT_FRQ
   #define  PLL_OUT_FRQ  PLL_OUT_96MHZ
   #warning PLL_OUT_FRQ not defined in config file using 96MHz Default
#endif
           

#define PLL_IN_PRESCAL_DISABLE         ( 0<<PINDIV )
#define PLL_IN_PRESCAL_ENABLE          ( 1<<PINDIV )

#define PLL_OUT_32MHZ     ( (0<<PDIV3)| (O<<PDIV2) | (1<<PDIV1)| (0<<PDIV0))
#define PLL_OUT_40MHZ     ( (0<<PDIV3)| (O<<PDIV2) | (1<<PDIV1)| (1<<PDIV0))
#define PLL_OUT_48MHZ     ( (0<<PDIV3)| (1<<PDIV2) | (0<<PDIV1)| (0<<PDIV0))
#define PLL_OUT_56MHZ     ( (0<<PDIV3)| (1<<PDIV2) | (0<<PDIV1)| (1<<PDIV0))
#define PLL_OUT_64MHZ     ( (0<<PDIV3)| (1<<PDIV2) | (1<<PDIV1)| (0<<PDIV0))
#define PLL_OUT_72MHZ     ( (0<<PDIV3)| (1<<PDIV2) | (1<<PDIV1)| (1<<PDIVO))
#define PLL_OUT_80MHZ     ( (1<<PDIV3)| (0<<PDIV2) | (0<<PDIV1)| (0<<PDIVO))
#define PLL_OUT_88MHZ     ( (1<<PDIV3)| (0<<PDIV2) | (0<<PDIV1)| (1<<PDIV0))
#define PLL_OUT_96MHZ     ( (1<<PDIV3)| (0<<PDIV2) | (1<<PDIV1)| (0<<PDIV0))
#define PLL_OUT_MSK       ( (1<<PDIV3)| (1<<PDIV2) | (1<<PDIV1)| (1<<PDIV0))

#if (PLL_OUT_FRQ==PLL_OUT_96MHZ)
   #define PLL_USB_DIV  (1<<PLLUSB) 
#else
   #define PLL_USB_DIV  (0<<PLLUSB)    
#endif       
      
#define PLL_HS_TMR_PSCAL_NULL          ( (0<<PLLTM1) | (0<<PLLTM0) )
#define PLL_HS_TMR_PSCAL_1             ( (0<<PLLTM1) | (1<<PLLTM0) )
#define PLL_HS_TMR_PSCAL_1DOT5         ( (1<<PLLTM1) | (0<<PLLTM0) )
#define PLL_HS_TMR_PSCAL_2             ( (1<<PLLTM1) | (1<<PLLTM0) )

#define PLL_HS_TMR_PSCAL_MSK           ( (1<<PLLTM1) | (1<<PLLTM0) )

#define Pll_set_hs_tmr_pscal_null()    (PLLFRQ&=~PLL_HS_TMR_PSCAL_MSK,PLLFRQ|=PLL_HS_TMR_PSCAL_NULL)      
#define Pll_set_hs_tmr_pscal_1()       (PLLFRQ&=~PLL_HS_TMR_PSCAL_MSK,PLLFRQ|=PLL_HS_TMR_PSCAL_1)      
#define Pll_set_hs_tmr_pscal_1dot5()   (PLLFRQ&=~PLL_HS_TMR_PSCAL_MSK,PLLFRQ|=PLL_HS_TMR_PSCAL_1DOT5)
#define Pll_set_hs_tmr_pscal_2()       (PLLFRQ&=~PLL_HS_TMR_PSCAL_MSK,PLLFRQ|=PLL_HS_TMR_PSCAL_2)


      //! @brief Start the PLL at only 48 MHz, regarding CPU frequency
      //! Start the USB PLL with clockfactor
      //! clockfactor can be PLLx06 or PLLx03
#if USB_LOW_SPEED_DEVICE==ENABLE
#define Start_pll(in_prescal)       \
           (Set_RC_pll_clock(),PLLFRQ &= ~PLL_OUT_MSK,PLLFRQ|= PLL_OUT_FRQ| PLL_USB_DIV , PLLCSR = (in_prescal | (1<<PLLE))) 
#else
#define Start_pll(in_prescal)       \
           (PLLFRQ &= ~PLL_OUT_MSK,PLLFRQ|= PLL_OUT_FRQ| PLL_USB_DIV , PLLCSR = (in_prescal | (1<<PLLE)))
#endif
      //! return 1 when PLL locked
#define Is_pll_ready()       (PLLCSR & (1<<PLOCK) )

      //! Test PLL lock bit and wait until lock is set
#define Wait_pll_ready()     while (!(PLLCSR & (1<<PLOCK)))

      //! Stop the PLL
#define Stop_pll()           (PLLCSR  &= (~(1<<PLLE)),PLLCSR=0 ) 
      
      //! Select the internal RC as clock source for PLL
#define Set_RC_pll_clock()    (PLLFRQ |= (1<<PINMUX))      
      
      //! Select XTAL as clock source for PLL
#define Set_XTAL_pll_clock()    (PLLFRQ &= ~(1<<PINMUX))        

      // Start the PLL in autofactor mode
      // regarding FOSC define
#if   (FOSC==8000)
      //! Start the PLL in autofactor mode
      //! regarding FOSC define
   #define Pll_start_auto()   Start_pll(PLL_IN_PRESCAL_DISABLE)
#elif (FOSC==16000)
   #define Pll_start_auto()   Start_pll(PLL_IN_PRESCAL_ENABLE)
#else
   #error   "FOSC should be defined in config.h"
#endif

   //! @}


/*
      Example 
      
   Pll_start_auto();
   Pll_set_hs_tmr_pscal_1dot5();
   Timerhs_clear();
   Timerhs_set_waveform_mode(TIMERHS_WGM_FAST_PWM);
   Timerhs_set_nb_bit(8);
   
   Timerhs_enable_pwm_a_and_na();
   Timerhs_enable_pwm_b_and_nb();
   Timerhs_enable_pwm_d_and_nd();
   
   Timerhs_set_compare_a(dt);
   Timerhs_set_compare_b(dt);
   Timerhs_set_compare_d(dt);
   Timerhs_set_clock(TIMERHS_CLK_BY_1);

   while (1)
   {  
      for(tempo=1;tempo;tempo++);
      Timerhs_set_compare_a(dt++);
      Timerhs_set_compare_b(dt++);
      Timerhs_set_compare_d(dt++);
   }
*/


//! @}
#endif  // PLL_DRV_H


