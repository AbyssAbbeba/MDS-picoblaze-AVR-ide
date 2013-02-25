/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file manages the generic HID IN/OUT task.
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

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf_usb.h"
#include "hid_task.h"
#include "lib_mcu/usb/usb_drv.h"
#include "usb_descriptors.h"
#include "modules/usb/device_chap9/usb_standard_request.h"
#include "usb_specific_request.h"
#include "lib_mcu/util/start_boot.h"
#include <util/delay.h>


//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________
#define MOSI	PORTB1

//_____ D E C L A R A T I O N S ____________________________________________

         volatile U8    cpt_sof=0;
extern            U8    jump_bootloader;
                  U8    g_last_joy       =0;
				  U8 length = 0;
				 U8 ispdata_received[64];									// aquire length of OUT report
				 U8 ispdata_to_send[64];
				  U8 i,indata_flag = 0;
				  
		   	static enum state {PERMITTED, FORBIDDEN} IN_permition;
		   	static enum command {RESET_ON, RESET_OFF, DATA} cmd_type;
						
void hid_report_out  (void);
void hid_report_in   (void);

//! @brief This function initializes the target board ressources.
//!
void hid_task_init(void)
{
   Leds_init();
   ///*void*/ SPI_MasterInit	// Set MOSI and SCK output, all others input */
			DDRB = 0b00000111;
			PRR0 &= ~(1<<PRSPI);
	// spi enable, master, nejnizsi rychlost, sample pri sestupné
			SPCR |= (1<<SPE);
			SPCR |=(1<<MSTR);
			SPCR |=(1<<SPR1);
			SPCR |=(1<<SPR0);
			SPCR |=(1<<CPHA);

	IN_permition = FORBIDDEN;

   Leds_off();
   Led0_off(); // actually ON
   
}

//! @brief Entry point of the HID generic communication task
//! This function manages IN/OUT repport management.
//!
void hid_task(void)
{
   if(!Is_device_enumerated())          // Check USB HID is enumerated
      return;

   hid_report_out();
   hid_report_in();
}


//! @brief Get data report from Host
//!
void hid_report_out(void)
{
   Usb_select_endpoint(EP_HID_OUT);
   if(Is_usb_receive_out())
   {
		Led1_on();
		
		
		static U8 znak;	
		 // finds out size of the incoming dat
		 
		 length = sizeof(UEBCLX); 
		 
		 
		 if (length == 1){
			 // reset ON or OFF?
			 znak = UEDATX;
			 if (znak =='R') {
				 	PORTB &= ~(1<< PORTB0);
				    _delay_ms(20);
			 } 	
			 else {
					PORTB |= (1<< PORTB0);		
			 }	
			  Led1_off();
		 	  Usb_ack_receive_out();
			  return;
		  }
		  else {
				// read data from buffer
			  	for (i = 0; i <= length; i++){
			       ispdata_received[i] = UEDATX;
				  }	
				  // send and receive data
				 // waits 20ms after reset (after pulling SS pin down)	
				for (i = 0; i <= length; i++)
					{
						SPDR = ispdata_received[i];                                             
						while(!(SPSR & (1<<SPIF)));	/* Wait for transmission complete */
						ispdata_to_send[i] = SPDR; 
					}
					// DATA ready to be sent
				IN_permition = PERMITTED;
				  			   
		  }		   														  
		// ------------------------------------------------------																	  //
		// reads buffer to memory																	  

		}	

      Led1_off();
      Usb_ack_receive_out();
  

   //** Check if we received DFU mode command from host
   if(jump_bootloader)
   {
      U32 volatile tempo;
      Leds_off();
      Usb_detach();                          // Detach actual generic HID application
      for(tempo=0;tempo<70000;tempo++);      // Wait some time before
      start_boot();                          // Jumping to booltoader
   }
}


//! @brief Send data report to Host
//!
void hid_report_in(void)
{

   Usb_select_endpoint(EP_HID_IN);
   if(!Is_usb_write_enabled())
      return;                                // Not ready to send report
   
			if (IN_permition == PERMITTED){
								//  fill buffer
									Led1_on();
									for (i = 0; i <= length; i++){					//  poslani prijatych dat
									UEDATX = ispdata_to_send[i];
									}	
									Led1_off();
									IN_permition = FORBIDDEN;
									 Usb_ack_in_ready(); 
								}											
								else {
									 return;
									 }
								 
	
}
																	  //
																			
		                      // Send data over the USB


//! @brief  This function increments the cpt_sof counter each times
//! the USB Start Of Frame interrupt subroutine is executed (1ms)
//! Usefull to manage time delays
//!
void sof_action()
{
   cpt_sof++;
    //static U8 blik;
    //if (UEINTX &= (1<<RWAL)) {
				//// turn red ON for 500ms
				//Led1_off();		// actualy ON
				//blik = cpt_sof;	
				//}
	//if (UEINTX &= (1<<RXOUTI)){
						//// turn red ON for 500ms
				//Led1_off();		// actualy ON
				//blik = cpt_sof;	
		//
				//}
	//
	//if (blik == (cpt_sof + 500)) Led1_on();
	//Led1_on();
}
