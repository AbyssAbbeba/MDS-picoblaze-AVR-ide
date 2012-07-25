
/*
 * Standard_request.h
 *
 * Created: 7.7.2012 13:05:00
 *  Author: abyss
 */ 

#ifndef STANDARD_REQUEST_H_
#define STANDARD_REQUEST_H_
#endif /* STANDARD_REQUEST_H_ */

#define U8		unsigned char
#define U16		unsigned int


//          	UHWCON REGISTR
#define USB_regulator_enable()				(UHWCON |= (1<<UVREGE))		// turn regulator ON


// 				USBCON REGISTR
#define USB_enable()						(USBCON |= (1<<USBE))		// Set to enable the USB controller. Clear to disable and reset the USB controller, to disable the
																		//	USB transceiver and to disable the USB controller clock inputs.
#define USB_VBUS_pad_enable()				(USBCON |= (1<<OTGPADE))	// Set to enable the VBUS pad. Clear to disable the VBUS pad.		

																		
#define USB_disable()						(USBCON &= ~(1<<USBE))		// USB disable (USB controler reset)

#define USB_clock_freeze_enable()			(USBCON |= (1<<FRZCLK))		// freezes USB clock, Set to disable the clock inputs (the ”Resume Detection” is still active). This reduces the power
																		// consumption. Clear to enable the clock inputs.
#define	USB_clock_freeze_disable()			(USBCON &= ~(1<<FRZCLK))	

//	USB device general registers
// 
//				UDCON REGISTR
#define USB_full_speed_mode()				(UDCON &= ~(1<<LSM))		// Clear to select full speed mode (D+ internal pull-up
																		// will be activate with the ATTACH bit will be set).	
																		
#define USB_detach_enabled()				(UDCON |= (1<<DETACH))		// Set to physically detach de device (disconnect internal pull-up on D+ or D-).
																		//	Clear to reconnect the device.
#define USB_detach_disabled()				(UDCON &= ~(1<<DETACH))		// !!!!!!!!!!!!!!!!!! after reset, detach is in 1 !!!!!!!!!!
																		//	Clear to reconnect the device.																		
//             UDADDR REGISTR
																			
#define USB_adress_enable()					(UDADDR |= (1<<ADDEN))		// Set to activate the UADD (USB address).
																		//	Cleared by hardware. Clearing by software has no effect.
	
//				UECONX REGISTR																																																																			
#define STALL_Request_aktivation()			(UECONX |= (1<<STALLRQ))	//Set to request a STALL answer to the host for the next handshake.
																		//	Cleared by hardware when a new SETUP is received. 
																		//	Clearing by software has no effect.
#define STALL_Request_Clear()				(UECONX |= (1<<STALLRQC))			//	Set to disable the STALL handshake mechanism.
																		//	Cleared by hardware immediately after the set. Clearing by software has no effect.
//      endpoint enable UECONX
#define endpoint_enable()					(UECONX |= (1<<EPEN))		// Set to enable the endpoint according to the device configuration. Endpoint 0 shall always be
																		//	enabled after a hardware or USB reset and participate in the device configuration.
																		// The incoming packets will be discarded (RXOUTI and RWAL will not be set).
																		//	Clear this bit to disable the endpoint.			                            
#define endpoint_disable()					(UECONX &= ~(1<<EPEN))		// Disable
																		// 
					
void endpoint_selection(unsigned char endp)								// selects endpoint to manage
{
	if (endp == 0)						// endp 0   - never
	{
		( UENUM &= ~(1<<UENUM_0)); 
		( UENUM &= ~(1<<UENUM_1)); 
		( UENUM &= ~(1<<UENUM_2)); 
	}
	
	if (endp == 1)						// endp 1
	{
		(UENUM |= (1<<UENUM_0)); 
		(UENUM &= ~(1<<UENUM_1)); 
		(UENUM &= ~(1<<UENUM_2)); 
	}
	if (endp == 2)						// endp 2
	{
		(UENUM &= ~(1<<UENUM_0)); 
		(UENUM  |= (1<<UENUM_1));
		(UENUM &= ~(1<<UENUM_2)); 
	}
		if (endp == 3)						// endp 3
	{
		(UENUM  |= (1<<UENUM_0)); 
		(UENUM  |= (1<<UENUM_1));
		(UENUM &= ~(1<<UENUM_2)); 
	}
		if (endp == 4)						// endp 4
	{
		(UENUM &= ~(1<<UENUM_0)); 
		(UENUM &= ~(1<<UENUM_1)); 
		(UENUM  |= (1<<UENUM_2));
	}
	
}					
					
																		
//				 UECFG0X REGISTR																		
void set_interrupt_endpoint_type(void)									// Selected endpoint type will be INTERRUPT
{
	(UECFG0X |= (1<<EPTYPE0));
	(UECFG0X |= (1<<EPTYPE1));

}

#define endpoint_type_OUT()					(UECFG0X &= ~(1<<EPDIR))		// Clear to configure an OUT direction for bulk, interrupt, 
																		//	isochronous or control endpoints.

#define endpoint_type_IN()					(UECFG0X |= (1<<EPDIR))		// Set to configure an IN direction for bulk, 
																		//	interrupt or isochronous endpoints.
																		
//               UECFG1X  REGISTR	
																	
void set_endpoint_size_64(void)											// Set this bit according to the endpoint size:
{																		// EPSIZE2 : EPSIZE1 : EPSIZE0
	(UECFG1X &= ~(1<<EPSIZE2));											//	000b: 8 bytes		100b: 128 bytes
	(UECFG1X |= (1<<EPSIZE0));											//	001b: 16 bytes		101b: 256 bytes
	(UECFG1X |= (1<<EPSIZE1));											//	010b: 32 bytes		110b: 512 bytes
}																		//	011b: 64 bytes		111b: Reserved. Do not use this configuration.


void set_one_bank(void)													// Set this field according to the endpoint size:
{
	(UECFG1X &= ~(1<<EPBK0));											// 00b: One bank
	(UECFG1X &= ~(1<<EPBK1));											// 01b: Double bank
}																	
																	
#define allocate_endpoint_memory()			(UECFG1X |= (1<<ALLOC));	// Set this bit to allocate the endpoint memory.
#define free_allocated_endpoint_memory()	(UECFG1X &= ~(1<<ALLOC));	//	Clear to free the endpoint memory.	



																