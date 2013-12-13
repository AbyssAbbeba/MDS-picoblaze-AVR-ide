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
#define MOSI				PORTB1
#define VERSION				0x10		// version in BCD    1.0
#define BLINK_TIME			250


//_____ D E C L A R A T I O N S ____________________________________________
U16		cnt = 0;
U8		reset = 0;
U8		reset_flag;
U8		error,cpt_sof;
U8		flags = 0b00000001;
		
				
extern			U8    jump_bootloader;
U8		 length;               
U8		 data_received_buff[64];		// aquire length of OUT report
U8		 data_to_send_buff[64];
U8		 i = 0;
	
		enum transfer 
		{
				out_SET_SPEED		= 'S', 
				out_RESET			= 'R',
				out_DATA_TRANSFER	= 'D',
				out_STATUS		    = 'T'
		}      transfer_type;
	
		enum item_type
		{
				item_short,
				item_long
		}	;
	
	
// PERMITION OF IN TASK				  
static enum state  
{		
		in_FORBIDDEN,
		in_STATUS,
		in_ERROR,
		in_DATA,
		in_read_and_send_short_item,
		in_Completed,
		in_set_and_send_short_item		
}      IN_permition;


						
// FUNCTION DECLARATION
void spi_conf                (U8 status);
U8		  get_flags          (void);
void	  set_flags          (U8);
U8		  get_version        (void);	
void      set_speed          (U8);		
U8        read_speed	     (void);

enum item_type GetItemType   (void);
void      GetReceivedData    (void);
			
void      hid_report_out     (void);
void      hid_report_in      (void);
void	  send_short_item    (U8, U8, U8, U8);

//! @brief This function initializes the target board ressources.
//!
void hid_task_init(void)
{
//	 configuration of I/O ports driver
Leds_init();		
			
//	 Set MOSI and SCK output, all others input */
DDRB = 0b00000111;
PRR0 &= ~(1<<PRSPI);

// spi enable, master, nejnizsi rychlost, sample pri sestupné
// set as master
SPCR |=(1<<MSTR);

// speed (fastest)
SPCR |=(1<<SPR1);
SPCR |=(1<<SPR0);
SPCR &=~(1<<SPI2X);

// phase
SPCR |=(1<<CPHA);
// spi enable									
SPCR |= (1<<SPE); 
			
// start  conditions			
IN_permition    =		in_FORBIDDEN;

Led0_off();
Led1_off();	
   
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
			// some local variables
			enum item_type	ReceivedItem;
			
			// clear ready bit in flags 
			flags &= ~(1 << 0); 
			// this function reads USB buffer and aquire received data length
			GetReceivedData();
			
			// gets item type from received data
			ReceivedItem = GetItemType();	
			
			switch (ReceivedItem)
			{
				case item_short:
				// read item tag
						if      (  0x01  ==  data_received_buff[0] ) 
							{
								//  flags / speed / version
								if ( 0x05  ==  data_received_buff[1] )
									{													
									//data_received_buff[3]   - speed   data_received_buff[4] - flags 
									//read version and flags							
										set_speed(data_received_buff[3]);
										set_flags(data_received_buff[4]); 
										IN_permition = in_set_and_send_short_item;
									break;									
									}
								else
								{
									IN_permition = in_read_and_send_short_item;
									break;	
								}				
							}		 
						else
						{
						// error here
							flags |= (1 << 1);					
						}
					break;
					
				case item_long:
							if      ( 0x1  ==  data_received_buff[0] )		
							{
								spi_conf( ENABLE  );
								spi_conf( DISABLE );
							
							}
							else if ( 0x2  ==  data_received_buff[0] )
							{
								
							}
							else
							{
								
							}
					
					break;
				default:
					break;
			}

//
						//set_speed(ispdata_received[1]);
						//speed = read_speed();	
		//transfer_type = ispdata_received[0];
		//
		//--------------------------------------
		//switch (transfer_type)
		//{
			////   1B 0x01  2B speed 1-4 (other fail)
			//case out_SET_SPEED : 
//
//
//
					//IN_permition = in_STATUS;
				//break;
				//
			//case out_RESET :
			//// pulls reset down to 0 or 1, according to second byte
			//// 1B reset, 2B 0 or 1
					//if ( '0' == ispdata_received[1] )
						//{
							//PORTB &= ~(1<< PORTB0);
							//_delay_ms(20);
							//reset = '0';	
						//}
					//if ( '1' == ispdata_received[1] )
						//{
							//PORTB |= (1<< PORTB0);
							//reset = '1';	
						//}
					//else
						//{
							////IN_permition = in_ERROR;
							////Usb_ack_receive_out();
							////return;
						//}							
					//IN_permition = in_STATUS;	
				//break;
				//
			//case out_STATUS :		// returns firmware version,speed, reset pin status
					////version_flag = 1;
					////speed_flag   = 1;
					////reset_flag   = 1;		
						//
					//IN_permition = in_STATUS;
				//break;
				//
			//case out_DATA_TRANSFER :
						//
				//IN_permition = in_DATA;
				//break;		
				//
					////U8	 buff_out;
					////buff_out      =     UEBCLX;
									 ////
					////for (i = 0; i < buff_out; i++)
						////{
							//////SPDR = ispdata_received[i];                                             
							//////while(!(SPSR & (1<<SPIF))){
								//////Led1_toggle();
								//////_delay_ms(150);
							//////}	/* Wait for transmission complete */
							//////ispdata_to_send[(i-1)] = SPDR;
							 ////ispdata_to_send(i) = 'D';
						////}		
			//default: 
			  		//IN_permition = in_ERROR;               
				//break;
		//}
		//
		flags |= (1 << 0); 
		Usb_ack_receive_out();
	}	 
    
	   //** Check if we received DFU mode command from host
	   if(jump_bootloader)
	   {
		  U32 volatile tempo;
		  //Leds_off();
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
   if(!Is_usb_write_enabled())// Not ready to send report
      return; 
	   
		// some variables274
		U8 buff;
		
		// clear ready flag
		flags &= ~(1 << 0); 
		
		//cnt++;				
		//if ( 750 == cnt)
		//{
			//Led0_toggle();
			//cnt = 0;	
		//}	
		switch( IN_permition )
			{
				case	 in_set_and_send_short_item:
								
								// sends prepared data
								send_short_item( 0x1, 0x5, read_speed(), flags );
								
								IN_permition = in_Completed; 
					break;
					
				case	in_read_and_send_short_item:
				
							// send actual values
								send_short_item( 0x1, 0x5, read_speed(), flags );
								IN_permition = in_Completed;
					break;
				case	in_Completed:
						Led1_toggle();
					break;
				
			}		
	// IN data handling		
	    flags |= (1 << 0); 	
		Usb_ack_in_ready();				              
}																			
																		
// Send data over the USB
//! @brief  This function increments the cpt_sof counter each times
//! the USB Start Of Frame interrupt subroutine is executed (1ms)
//! Usefull to manage time delays
//!
void sof_action()
{
   cpt_sof++;
}



inline void set_speed(U8 received_speed)
{					
	if ( 1 == received_speed )
		{
			SPCR &= ~(1<<SPR1);
			SPCR &= ~(1<<SPR0);							
		}
	//0 0 1 fosc/16
	if ( 2 == received_speed )
		{
			SPCR &= ~(1<<SPR1);
			SPCR &= ~(1<<SPR0);								
		}
	//0 1 0 fosc/64
	if ( 3 == received_speed )
		{
			SPCR &= ~(1<<SPR1);
			SPCR &= ~(1<<SPR0);								
		}
	//0 1 1 fosc/128
	if ( 4 == received_speed )
		{
			SPCR &= ~(1<<SPR1);
			SPCR &= ~(1<<SPR0);						
		}					
}

inline U8 read_speed (void)
{
	//U8		spi_speed;
	//if ( 0x3 == ( SPCR & 0b00000011 ) )		spi_speed = 4;  
	//if ( 0x2 == ( SPCR & 0b00000010 ) )		spi_speed = 3;
	//if ( 0x1 == ( SPCR & 0b00000001 ) )		spi_speed = 2;
	//if ( 0x0 == ( SPCR & 0b00000000 ) )		spi_speed = 1;
	
	return	(( SPCR & 0b00000011 ) + 1);
}				

inline void GetReceivedData(void)
{
	length = UEBCLX;
	
	for (i = 0; i < 64; i++)
	{
		data_received_buff[i] = UEDATX;
	}	
}

inline enum item_type GetItemType(void)
{
	if ( length > 5 )	return item_long;
	else				return item_short;
}	

inline U8		  get_flags(void)
{

		  if ( 1 == (flags & 0b00000010))		flags    |= ( 1<< 1);// error
		  else									flags    &= ~(1<< 2);
		  
		  if ( 1 == (flags & 0b00000100))		flags    |= ( 1<< 0); // ready
		  else									flags    &= ~(1<< 2);

						//}
	      return flags;
		  // flags    0 / 0 / 0 / 0 / 0 / reset / error / ready
}
inline void	  set_flags(U8 flags_temp)
{
		flags = flags_temp;
		// flags    0 / 0 / 0 / 0 / 0 / reset / error / ready
	    if ( 4 == (flags & 0b00000100))	
		{
			PORTB &= ~(1<< PORTB0);
			//_delay_ms(20);
		}
	    else
	    {
			PORTB |= (1<< PORTB0);
	    }
}
inline U8		  get_version(void)
{
		  return VERSION;	
}

inline void    send_short_item(U8 tag, U8 size, U8 speed, U8 flag_temp)
{
		  UEDATX = tag;
		  UEDATX = size;
		  UEDATX = speed;
		  UEDATX = flag_temp;
		  UEDATX = VERSION;
		  // add dummy bytes
		  for ( i = 5; i <= 63; i++)
		  {
			UEDATX = 0x00;  
		  }		  		  	 	  	
}

inline void    send_long_item(U8 tag, U8 size)
{
		  UEDATX = tag;
		  UEDATX = size;
		  // send data bytes
		  for ( i = 5; i <= 63; i++)
		  {
			UEDATX = 0x00; //data_to_send_buff 
		  }
		  		  		  	 	  	
}

void spi_conf(U8 status)
{
		if		( ENABLE == status )
		{
			
		}
		else if ( DISABLE == status)
		{
			
		}
		else
		{
			
		}
							//////SPDR = ispdata_received[i];                                             
							//////while(!(SPSR & (1<<SPIF))){
								//////Led1_toggle();
								//////_delay_ms(150);
							//////}	/* Wait for transmission complete */
							//////ispdata_to_send[(i-1)] = SPDR;
							 ////ispdata_to_send(i) = 'D';
						////}	


}