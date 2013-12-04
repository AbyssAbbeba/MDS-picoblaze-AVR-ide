/*
 * Programmer.c
 *
 * Created: 4.7.2012 18:53:53
 *  Author: abyss 
 
 
 //PORTB |= (1<<PB6) | (1<<PB4); PORTB &= ~((1<<PB6) | (1<<PB4));

 if (PORTB &= (1<<PB6))
{
//Do something, for instance set a value in a variable
}

Check if PB6 and PB4 is set:

if ((PORTB &= ((1<<PB6) | (1<<PB4))) == ((1<<PB6) | (1<<PB4)))
{
//Do something, for instance set a value in a variable
}




 */ 
#define F_CPU 12000000UL  // 12 MHz

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h> 

#include "Standard_request.h"
#include "USB_functions.h"

unsigned int interrupt_status;
unsigned char N_in,N_out,i;
unsigned char data_in[64];
unsigned char data_out[64];
unsigned char bytes_to_send;

unsigned char bmRequestType;
unsigned char bRequest;
unsigned char wValue_lsb;
unsigned char wValue_msb;
unsigned char wIndex_lsb;
unsigned char wIndex_msb;
unsigned char wLength_lsb;
unsigned char wLength_msb;

unsigned char device_descriptor[] = 
{
	18,												// desc. length 
	1,												// desc. type DEVICE
	0x02,
	0x00,											// USB specification (2.0) 0x0200;
	0,
	0,
	0,												// class,subclass etc.
	64,												// max packet size	
	0xC2,
	0x51,											// id vendor
	0x17,
	0x02,											// id product
	0x01,
	0x00,											// 1.00 verze
	0x04,											// index of string desc
	0x20,											// iproduct
	0x42,											// iserialnumber
	0x01											// number of configurations - 1	
};
unsigned char configuration_descriptor[] = 
{
	9,												// legth
	2,												// desc tzpe CONFIGURATION
	32,												// TOTAL LENGTH OF DESCRIPTORS                                !!!!!!!!!!!!!!
	1,			
	1,												// number of configurations and configuration number
	0,												// dont know what that means o.O
	0x80,											// USB_CONFIG_BUSPOWERED
	250,											// max current = 250*2 = 500mA	
};
unsigned char interface_descriptor[] = 
{
	9,												// length (solid)
	4,												// type interface
	0,												// INTERFACE NUMBER
	0,												// not used
	2,												// NUMBER OF ENDPOINTS WITHOUT endp0
	0xff,											// CLASS VENDOR SPECIFIC
	0xff,										    // SUB CLASS
	0xff,											// protocol
	0												//  Index of string descriptor describing this interface  S T R I N G   D E F I N E                                        
};
unsigned char endpoint_descriptor1[] = 
{
	7,												// length
	5,												// type
	0b10000001,										// ENDPOINT NUMBER AND DIRECTION --- IN¨¨
	3,												// interrupt type
	0,
	64,												//(poradi bajtu OOBRATIT?)
	2																				
};
unsigned char endpoint_descriptor2[] = 
{
	7,												// length
	5,												// type
	0b00000001,										// ENDPOINT NUMBER AND DIRECTION --- OUT
	3,												// interrupt type
	0,
	64,												// packet size							(poradi bajtu OOBRATIT?)
	2												// poooooling
};

//if a device does not support string descriptors, all
//references to string descriptors within device, configuration, and interface descriptors must be reset to zero.

unsigned char string_descriptor[] = 
{
	
};




int main(void)
{
	// DODELAT MEGA INICIALIZACI
	// VYRESIT ZLP
	// prozkoumat pouziti TXINI
	// zkontrolovat data stage
	usb_inicialization();
	interrupt_configuration();
	USB_detach_disabled();
	sei();
	
	
		 while(1)
		 {
			 
    		switch (interrupt_status)
    		{
	case 0:
				interrupt_status = 0;
	case 1:
				interrupt_status = 0;
	case 2:
				
				
				interrupt_status = 0;
				
				////////////////////////////////////////////////////////////////
				
				
	case 3:		//Once the OUT status stage has been received, the USB controller waits for a SETUP request.
				//The SETUP request have priority over any other request and has to be ACK’ed. This means that
				//	any other flag should be cleared and the fifo reset when a SETUP is received.
		
		(UEIENX &= ~(1<<NAKOUTE));														// turn off interrupts !
		(UEIENX &= ~(1<<NAKINE));
		(UEIENX &= ~(1<<RXOUTE));
		(UEIENX &= ~(1<<TXINE));	
				//       S E T U P   S T A G E	

						//while (RXOUTI != 1)												// waiting for data0 to be sent
							//{
							//// flash led for example 
							//}
																						// read data from bank
								bmRequestType = UEDATX;
								
								bRequest = UEDATX;
								
								wValue_lsb = UEDATX;
								wValue_msb = UEDATX;
								
								wIndex_lsb = UEDATX;
								wIndex_msb = UEDATX;
								
								wLength_lsb = UEDATX;
								wLength_msb = UEDATX;
					(UEINTX &= ~(1<<RXSTPI));												// ack interrupt source	, send handshake																			
					// CHOOSE REQUEST TYPE
								switch (bRequest)
								{
											case 0:												// GET_STATUS
											
											
											case 5:												// SET_ADDRESS

											UDADDR = wValue_lsb;								// copy adress from host to uC register UDADDR
											send_zlp();											// send zero length packet
											(UDADDR |= (1<<ADDEN));								// enable adress 

											// NO DATA STAGE !!
															
											case 6:												 // GET_DESCRIPTOR
													get_descriptor();
											
											case 7:												// SET_DESCRIPTOR
											;											
											//The device driver will then select a configuration for the device, by 
											//sending a Set Configuration request to the device.
								}
								
								
								
								
					
						(UEINTX &= ~(1<<RXOUTI));			// ack ack data stage, ready for IN or OUT data
				

				//				 S T A T U S    S T A G E
				
				
				interrupt_status = 0;
				interrupt_configuration();									// TURN interrupts back ON	
				/////////////////////////////////////////////////////////////////
	case 4:
				N_out = UEBCLX;												// find out number of bytes
				
					for (i = 0; i <= N_out; i++ )							// read data from bank
					{
						data_out[i] = UEDATX;
					}
					
				(UEINTX &= ~(1<<FIFOCON));							// stop sending NAK, ready for another OUT !
				interrupt_status = 0;
				/////////////////////////////////////////////////////////////////
	case 5:
					N_in = bytes_to_send;
					
				
					for (i = 0; i <= N_in; i++ )							// push data into bank
					{
						UEDATX = data_in[i];
					}
				
				
				(UEINTX &= ~(1<<FIFOCON));							// stop sending NAK, send data and be ready for another IN !
				interrupt_status = 0;
    		}
					
		 }
}	
	

	
ISR (USB_COM_vect)														// endpoint interrupt routine
{
 if (UEINT &= (1<<EPINT0))				// endpoint 0 interrupt
{
	endpoint_selection(0);
	which_int_source();			
}	

/////////////////////////////////////////////

 if (UEINT &= (1<<EPINT1))				// endpoint 1 interrupt (IN)
{
	endpoint_selection(1);	
	which_int_source();			
}	

////////////////////////////////////////////

 if (UEINT &= (1<<EPINT2))				// endpoint 2 interrupt (OUT)
{
	endpoint_selection(2);	
	which_int_source();			
}
	
}


//ISR (USB_GEN_vect){}