
/*
 * IncFile2.h
 *
 * Created: 12.7.2012 20:00:28
 *  Author: abyss
 */ 


#ifndef INCFILE2_H_
#define INCFILE2_H_

#endif /* INCFILE2_H_ */

unsigned int interrupt_status;

void PLL_inicialization(void)
{																
		(PLLFRQ |= (1<<PINMUX));									// 8 MHz osc. input to PLL
		(PLLFRQ &= ~(1<<PLLUSB));		
		(PLLCSR &= ~(1<<PINDIV));									// select 8 MHz input clock for PLL				
		(PLLCSR |= (1<<PLLE));										// enables PLL				
			while ( !( PLLCSR & (1<<PLOCK)) );						//	=== PLL locked?	PLOCK = 1		!!!!!!!!!!!!	
}

void usb_inicialization (void)
{
	USB_regulator_enable();										//Power On the USB interface
	USB_VBUS_pad_enable();										//• Power-On USB pads regulator
	USB_full_speed_mode();										//• Configure PLL interface
																//• Enable PLL
	PLL_inicialization();										//• Check PLL lock
																//• Enable USB interface
	USB_enable();												//• Configure USB interface (USB speed, Endpoints configuration...)
	USB_clock_freeze_disable();									//• Wait for USB VBUS information connection
																//• Attach USB device	
}



void endpoint_activation(void)
{
		endpoint_selection(1);
	endpoint_enable();
	set_interrupt_endpoint_type();
	endpoint_type_IN();
	set_endpoint_size_64();
	set_one_bank();
	allocate_endpoint_memory();	
	
		endpoint_selection(2);
	endpoint_enable();
	set_interrupt_endpoint_type();
	endpoint_type_OUT();
	set_endpoint_size_64();
	set_one_bank();
	allocate_endpoint_memory();	
	
		endpoint_selection(3);
	endpoint_enable();
	set_interrupt_endpoint_type();
	endpoint_type_IN();
	set_endpoint_size_64();
	set_one_bank();
	allocate_endpoint_memory();	
	
		endpoint_selection(4);
	endpoint_enable();
	set_interrupt_endpoint_type();
	endpoint_type_OUT();
	set_endpoint_size_64();
	set_one_bank();
	allocate_endpoint_memory();		
}

void interrupt_configuration(void)
{
		(UEIENX |= (1<<NAKOUTE));									// enable an endpoint interrupt (EPINTx) when NAKOUTI is set.
		(UEIENX |= (1<<NAKINE));									//Set to enable an endpoint interrupt (EPINTx) when NAKINI is set.
		(UEIENX |= (1<<RXSTPE));									//Set to enable an endpoint interrupt (EPINTx) when RXSTPI is sent.
		(UEIENX |= (1<<RXOUTE));									//Set to enable an endpoint interrupt (EPINTx) when RXOUTI is sent.
		(UEIENX |= (1<<TXINE));										//Set to enable an endpoint interrupt (EPINTx) when TXINI is sent.
}

void which_int_source(void)
{
//	• 6 - NAKINI - NAK IN Received Interrupt Flag
//Set by hardware when a NAK handshake has been sent in response of a IN request from the
//host. This triggers an USB interrupt if NAKINE is sent.
//Shall be cleared by software. Setting by software has no effect.

 if (UEINTX &= (1<<NAKINI))				//  interrupt source nakini? 
{
	interrupt_status = 1;
	(UEINTX &= ~(1<<NAKINI));			// ack interrupt source	
}

//• 4 - NAKOUTI - NAK OUT Received Interrupt Flag
//Set by hardware when a NAK handshake has been sent in response of a OUT/PING request
//from the host. This triggers an USB interrupt if NAKOUTE is sent.
//Shall be cleared by software. Setting by software has no effect.

 if (UEINTX &= (1<<NAKOUTI))				//  interrupt source NAKOUTI? 
{
	interrupt_status = 2;
	(UEINTX &= ~(1<<NAKOUTI));			// ack interrupt source	
}

//• 3 - RXSTPI - Received SETUP Interrupt Flag
//Set by hardware to signal that the current bank contains a new valid SETUP packet. An interrupt
//(EPINTx) is triggered (if enabled).
//Shall be cleared by software to handshake the interrupt. Setting by software has no effect.
//This bit is inactive (cleared) if the endpoint is an IN endpoint.

 if (UEINTX &= (1<<RXSTPI))				//  interrupt source RXSTPI? 
{
	interrupt_status = 3;
	
}

//• 2 - RXOUTI / KILLBK - Received OUT Data Interrupt Flag
//Set by hardware to signal that the current bank contains a new packet. An interrupt (EPINTx) is
//triggered (if enabled).
//Shall be cleared by software to handshake the interrupt. Setting by software has no effect.

 if (UEINTX &= (1<<RXOUTI))				//  interrupt source RXOUTI)? 
{
	interrupt_status = 4;
	(UEINTX &= ~(1<<RXOUTI));			// ack interrupt source	,  send handshake
}


//• 0 - TXINI - Transmitter Ready Interrupt Flag
//Set by hardware to signal that the current bank is free and can be filled. An interrupt (EPINTx) is
//triggered (if enabled).
//Shall be cleared by software to handshake the interrupt. Setting by software has no effect.
//This bit is inactive (cleared) if the endpoint is an OUT endpoint.

 if (UEINTX &= (1<<TXINI))				//  interrupt source TXINI? 
{
	interrupt_status = 5;
	(UEINTX &= ~(1<<TXINI));			// ack interrupt source	,  send handshake
}


}


void send_zlp(void)
{
	while ( !( UEINTX & (1<<TXINI)) );
	
	(UEINTX &= ~(1<<TXINI));
	(UEINTX &= ~(1<<FIFOCON));							// send packet
}

void get_descriptor(void)								// this function handles descriptor request-----------
{
			//wValue_msb					// descriptor type !!!!!!
			//	
												////	Descriptor Types Value
												////DEVICE 1
												//CONFIGURATION 2
												//STRING 3
												//INTERFACE 4
												//ENDPOINT 5
												//DEVICE_QUALIFIER 6
												//OTHER_SPEED_CONFIGURATION 7
												//INTERFACE_POWER1 8
						switch (wValue_msb)
						{
								case 1:						//   DEVICE DESCRIPTOR
							
			////////////////			D A T A   S T A G E
				while (RXOUTI != 1 || TXINI != 1  )							// checking if IN or OUT data
				{
					
					// flash led for example
				}
						 if (UEINTX &= (1<<RXOUTI))				//  is it OUT?
							{
									
	
							} 
							
						 if (UEINTX &= (1<<TXINI))				//  is it IN? 
							{
									
	
							} 	
								case 2:						// CONFIGURATION
													
								case 3:						// string
													
								case 4:						// interface
													
								case 5:						// endpoint
													;
												}
			// wValue_lsb										descriptor index in the low byte
		//	The wIndex field specifies the Language ID for string descriptors or is reset to zero for other descriptors.
		
}