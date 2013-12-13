/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file contains the USB low level driver definition
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


#ifndef _USB_DRV_H_
#define _USB_DRV_H_

//_____ I N C L U D E S ____________________________________________________


typedef enum endpoint_parameter{ep_num, ep_type, ep_direction, ep_size, ep_bank, nyet_status} t_endpoint_parameter;

//! @defgroup USB_low_level_drivers USB low level drivers
//! USB low level drivers Module
//! @{

//_____ M A C R O S ________________________________________________________

#define MAX_EP_NB             7

#define EP_CONTROL            0
#define EP_1                  1
#define EP_2                  2
#define EP_3                  3
#define EP_4                  4
#define EP_5                  5
#define EP_6                  6
#define EP_7                  7

#define PIPE_CONTROL          0
#define PIPE_0                0
#define PIPE_1                1
#define PIPE_2                2
#define PIPE_3                3
#define PIPE_4                4
#define PIPE_5                5
#define PIPE_6                6
#define PIPE_7                7

// USB EndPoint
#define MSK_EP_DIR            0x7F
#define MSK_UADD              0x7F
#define MSK_EPTYPE            0xC0
#define MSK_EPSIZE            0x70
#define MSK_EPBK              0x0C
#define MSK_DTSEQ             0x0C
#define MSK_NBUSYBK           0x03
#define MSK_CURRBK            0x03
#define MSK_DAT               0xFF  // UEDATX
#define MSK_BYCTH             0x07  // UEBCHX
#define MSK_BYCTL             0xFF  // UEBCLX
#define MSK_EPINT             0x7F  // UEINT
#define MSK_HADDR             0xFF  // UHADDR

// USB Pipe
#define MSK_PNUM              0x07  // UPNUM
#define MSK_PRST              0x7F  // UPRST
#define MSK_PTYPE             0xC0  // UPCFG0X
#define MSK_PTOKEN            0x30
#define MSK_PEPNUM            0x0F
#define MSK_PSIZE             0x70  // UPCFG1X
#define MSK_PBK               0x0C

#define MSK_NBUSYBK           0x03

#define MSK_ERROR             0x1F

#define MSK_PTYPE             0xC0  // UPCFG0X
#define MSK_PTOKEN            0x30
#define MSK_TOKEN_SETUP       0x30
#define MSK_TOKEN_IN          0x10
#define MSK_TOKEN_OUT         0x20
#define MSK_PEPNUM            0x0F

#define MSK_PSIZE             0x70  // UPCFG1X
#define MSK_PBK               0x0C


// Parameters for endpoint configuration
// These define are the values used to enable and configure an endpoint.
#define TYPE_CONTROL             0
#define TYPE_ISOCHRONOUS         1
#define TYPE_BULK                2
#define TYPE_INTERRUPT           3
 //typedef enum ep_type {TYPE_CONTROL, TYPE_BULK, TYPE_ISOCHRONOUS, TYPE_INTERRUPT} e_ep_type;

#define DIRECTION_OUT            0
#define DIRECTION_IN             1
 //typedef enum ep_dir {DIRECTION_OUT, DIRECTION_IN} e_ep_dir;

#define SIZE_8                   0
#define SIZE_16                  1
#define SIZE_32                  2
#define SIZE_64                  3
#define SIZE_128                 4
#define SIZE_256                 5
#define SIZE_512                 6
#define SIZE_1024                7
 //typedef enum ep_size {SIZE_8,   SIZE_16,  SIZE_32,  SIZE_64,
 //                      SIZE_128, SIZE_256, SIZE_512, SIZE_1024} e_ep_size;

#define ONE_BANK                 0
#define TWO_BANKS                1
 //typedef enum ep_bank {ONE_BANK, TWO_BANKS} e_ep_bank;

#define NYET_ENABLED             0
#define NYET_DISABLED            1
 //typedef enum ep_nyet {NYET_DISABLED, NYET_ENABLED} e_ep_nyet;

#define TOKEN_SETUP              0
#define TOKEN_IN                 1
#define TOKEN_OUT                2

#define Is_ep_addr_in(x)         (  (x&USB_ENDPOINT_DIR_MASK)?   TRUE : FALSE)


//! @defgroup Endpoints_configuration Configuration macros for endpoints
//! List of the standard macro used to configure pipes and endpoints
//! @{
#define Usb_build_ep_config0(type, dir, nyet)     ((type<<6) | (nyet<<1) | (dir))
#define Usb_build_ep_config1(size, bank     )     ((size<<4) | (bank<<2)        )
#define usb_configure_endpoint(num, type, dir, size, bank, nyet)             \
                                    ( Usb_select_endpoint(num),              \
                                      usb_config_ep(Usb_build_ep_config0(type, dir, nyet),\
                                                    Usb_build_ep_config1(size, bank)    ))

#define Host_build_pipe_config0(type, token, ep_num)     ((type<<6) | (token<<4) | (ep_num))
#define Host_build_pipe_config1(size, bank     )         ((size<<4) | (bank<<2)        )
#define host_configure_pipe(num, type, token,ep_num, size, bank, freq)             \
                                    ( Host_select_pipe(num),              \
                                      Host_set_interrupt_frequency(freq), \
                                      host_config_pipe(Host_build_pipe_config0(type, token, ep_num),\
                                                       Host_build_pipe_config1(size, bank)    ))
//! @}

//! @defgroup USB_regulator USB Pads Regulator drivers
//! Turns ON/OFF USB pads regulator
//! @{
   //! Enable internal USB pads regulator
#define Usb_enable_regulator()          (UHWCON |= (1<<UVREGE))
   //! Disable internal USB pads regulator
#define Usb_disable_regulator()         (UHWCON &= ~(1<<UVREGE))
   //! Check regulator enable bit
#define Is_usb_regulator_enabled()      ((UHWCON &  (1<<UVREGE))  ? TRUE : FALSE)
//! @}

//! @defgroup gen_usb USB common management drivers
//! These macros manage the USB controller
//! @{

   //! Enable USB interface
#define Usb_enable()                  (USBCON |= ((1<<USBE) ))
   //! Disable USB interface
#define Usb_disable()                 (USBCON &= ~((1<<USBE)))
   //! Use device full speed mode (default mode)
#define Usb_full_speed_mode()         (UDCON &= ~(1<<LSM))
   //! For device mode, force low speed mode
#define Usb_low_speed_mode()          (UDCON |= (1<<LSM))

#define Is_usb_enabled()              ((USBCON  &   (1<<USBE))   ? TRUE : FALSE)

#define Usb_enable_device()           (USBCON |= (1<<USBE))
#define Usb_disable_device()          (USBCON &= ~(1<<USBE))
#define Usb_reset_macro_only()        (UDCON &= ~(1<<RSTCPU))
#define Usb_reset_all_system()        (UDCON |= (1<<RSTCPU))

   //! Enable VBUS pad
#define Usb_enable_vbus_pad()         (USBCON |= (1<<OTGPADE))
   //! Disable VBUS pad
#define Usb_disable_vbus_pad()        (USBCON &= ~(1<<OTGPADE))

   //! Stop internal USB clock in interface (freeze the interface register)
#define Usb_freeze_clock()            (USBCON  |=  (1<<FRZCLK))
#define Usb_unfreeze_clock()          (USBCON  &= ~(1<<FRZCLK))
#define Is_usb_clock_freezed()        ((USBCON  &   (1<<FRZCLK)) ? TRUE : FALSE)


#define MSK_IDTE_VBUSTE               ((1<<IDTE)|(1<<VBUSTE))
   //! returns the USB general interrupts (interrupt enabled)
#define Usb_get_general_interrupt()      (USBINT & (USBCON & MSK_IDTE_VBUSTE))
   //! acks the general interrupts (interrupt enabled)
#define Usb_ack_all_general_interrupt()  (USBINT = ~(USBCON & MSK_IDTE_VBUSTE))
#define Usb_ack_cache_id_transition(x)   ((x)  &= ~(1<<IDTI))
#define Usb_ack_cache_vbus_transition(x) ((x)  &= ~(1<<VBUSTI))
#define Is_usb_cache_id_transition(x)    (((x) &   (1<<IDTI))  )
#define Is_usb_cache_vbus_transition(x)  (((x) &   (1<<VBUSTI)))

#define Usb_enable_vbus_interrupt()   (USBCON  |=  (1<<VBUSTE))
#define Usb_disable_vbus_interrupt()  (USBCON  &= ~(1<<VBUSTE))
#define Is_usb_vbus_interrupt_enabled() ((USBCON &  (1<<VBUSTE))     ? TRUE : FALSE)
#define Is_usb_vbus_high()            ((USBSTA &   (1<<VBUS))    ? TRUE : FALSE)
#define Is_usb_vbus_low()             ((USBSTA &   (1<<VBUS))    ? FALSE : TRUE)
#define Usb_ack_vbus_transition()     (USBINT  = ~(1<<VBUSTI))
#define Is_usb_vbus_transition()      ((USBINT &   (1<<VBUSTI))  ? TRUE : FALSE)
//! @}


//! @defgroup USB_device_driver USB device controller drivers
//! These macros manage the USB Device controller.
//! @{
   //! initiates a remote wake-up
   #define Usb_initiate_remote_wake_up()             (UDCON   |=  (1<<RMWKUP))
   //! detaches from USB bus
   #define Usb_detach()                              (UDCON   |=  (1<<DETACH))
   //! attaches to USB bus
   #define Usb_attach()                              (UDCON   &= ~(1<<DETACH))
   //! test if remote wake-up still running
   #define Is_usb_pending_remote_wake_up()           ((UDCON & (1<<RMWKUP)) ? TRUE : FALSE)
   //! test if the device is detached
   #define Is_usb_detached()                         ((UDCON & (1<<DETACH)) ? TRUE : FALSE)

   //! returns the USB device interrupts (interrupt enabled)
   #define Usb_get_device_interrupt()                (UDINT   &   (1<<UDIEN))
   //! acks the USB device interrupts (interrupt enabled)
   #define Usb_ack_all_device_interrupt()            (UDINT   =  ~(1<<UDIEN))

   //! enables remote wake-up interrupt
   #define Usb_enable_remote_wake_up_interrupt()     (UDIEN   |=  (1<<UPRSME))
   //! disables remote wake-up interrupt
   #define Usb_disable_remote_wake_up_interrupt()    (UDIEN   &= ~(1<<UPRSME))
#define Is_remote_wake_up_interrupt_enabled()     ((UDIEN &  (1<<UPRSME))   ? TRUE : FALSE)
   //! acks remote wake-up
#define Usb_ack_remote_wake_up_start()            (UDINT   = ~(1<<UPRSMI))
   //! tests if remote wake-up still running
#define Is_usb_remote_wake_up_start()             ((UDINT &   (1<<UPRSMI))  ? TRUE : FALSE)

   //! enables resume interrupt
#define Usb_enable_resume_interrupt()             (UDIEN   |=  (1<<EORSME))
   //! disables resume interrupt
#define Usb_disable_resume_interrupt()            (UDIEN   &= ~(1<<EORSME))
#define Is_resume_interrupt_enabled()             ((UDIEN &  (1<<EORSME))   ? TRUE : FALSE)
   //! acks resume
#define Usb_ack_resume()                          (UDINT   = ~(1<<EORSMI))
   //! tests if resume occurs
#define Is_usb_resume()                           ((UDINT &   (1<<EORSMI))  ? TRUE : FALSE)

   //! enables wake-up interrupt
#define Usb_enable_wake_up_interrupt()            (UDIEN   |=  (1<<WAKEUPE))
   //! disables wake-up interrupt
#define Usb_disable_wake_up_interrupt()           (UDIEN   &= ~(1<<WAKEUPE))
#define Is_wake_up_interrupt_enabled()           ((UDIEN &  (1<<WAKEUPE))   ? TRUE : FALSE)
   //! acks wake-up
#define Usb_ack_wake_up()                         (UDINT   = ~(1<<WAKEUPI))
   //! tests if wake-up occurs
#define Is_usb_wake_up()                          ((UDINT &   (1<<WAKEUPI)) ? TRUE : FALSE)

   //! enables USB reset interrupt
#define Usb_enable_reset_interrupt()              (UDIEN   |=  (1<<EORSTE))
   //! disables USB reset interrupt
#define Usb_disable_reset_interrupt()             (UDIEN   &= ~(1<<EORSTE))
#define Is_reset_interrupt_enabled()              ((UDIEN &  (1<<EORSTE))   ? TRUE : FALSE)
   //! acks USB reset
#define Usb_ack_reset()                           (UDINT   = ~(1<<EORSTI))
   //! tests if USB reset occurs
#define Is_usb_reset()                            ((UDINT &   (1<<EORSTI))  ? TRUE : FALSE)

   //! enables Start Of Frame Interrupt
#define Usb_enable_sof_interrupt()                (UDIEN   |=  (1<<SOFE))
   //! disables Start Of Frame Interrupt
#define Usb_disable_sof_interrupt()               (UDIEN   &= ~(1<<SOFE))
#define Is_sof_interrupt_enabled()                ((UDIEN &  (1<<SOFE))   ? TRUE : FALSE)
   //! acks Start Of Frame
#define Usb_ack_sof()                             (UDINT   = ~(1<<SOFI))
   //! tests if Start Of Frame occurs
#define Is_usb_sof()                              ((UDINT &   (1<<SOFI))    ? TRUE : FALSE)

   //! enables suspend state interrupt
#define Usb_enable_suspend_interrupt()            (UDIEN   |=  (1<<SUSPE))
   //! disables suspend state interrupt
#define Usb_disable_suspend_interrupt()           (UDIEN   &= ~(1<<SUSPE))
   //! test if suspend interrupt is enabled
#define Is_suspend_interrupt_enabled()            ((UDIEN &  (1<<SUSPE))   ? TRUE : FALSE)
   //! acks Suspend
#define Usb_ack_suspend()                         (UDINT   = ~(1<<SUSPI))
   //! tests if Suspend state detected
#define Is_usb_suspend()                          ((UDINT &   (1<<SUSPI))   ? TRUE : FALSE)

   //! enables USB device address
#define Usb_enable_address()                      (UDADDR  |=  (1<<ADDEN))
   //! disables USB device address
#define Usb_disable_address()                     (UDADDR  &= ~(1<<ADDEN))
   //! test if device is adressed
#define Is_usb_addressed()                        ((UDADDR & (1<<ADDEN)) ? TRUE : FALSE)
   //! sets the USB device address
#define Usb_configure_address(addr)               (UDADDR  =   (UDADDR & (1<<ADDEN)) | ((U8)addr & MSK_UADD))

   //! returns the last frame number
#define Usb_frame_number()                        ((U16)((((U16)UDFNUMH) << 8) | ((U16)UDFNUML)))
   //! tests if a crc error occurs in frame number
#define Is_usb_frame_number_crc_error()           ((UDMFN & (1<<FNCERR)) ? TRUE : FALSE)
//! @}




//! @defgroup usb_gen_ep USB endpoint drivers
//! These macros manage the common features of the endpoints.
//! @{
   //! selects the endpoint number to interface with the CPU
#define Usb_select_endpoint(ep)                   (UENUM = (U8)ep )

   //! get the currently selected endpoint number
#define Usb_get_selected_endpoint()               (UENUM )

   //! resets the selected endpoint
#define Usb_reset_endpoint(ep)                    (UERST   =   1 << (U8)ep, UERST  =  0)

   //! enables the current endpoint
#define Usb_enable_endpoint()                     (UECONX  |=  (1<<EPEN))
   //! enables the STALL handshake for the next transaction
#define Usb_enable_stall_handshake()              (UECONX  |=  (1<<STALLRQ))
   //! resets the data toggle sequence
#define Usb_reset_data_toggle()                   (UECONX  |=  (1<<RSTDT))
   //! disables the current endpoint
#define Usb_disable_endpoint()                    (UECONX  &= ~(1<<EPEN))
   //! disables the STALL handshake
#define Usb_disable_stall_handshake()             (UECONX  |=  (1<<STALLRQC))
   //! selects endpoint interface on CPU
#define Usb_select_epnum_for_cpu()                (UECONX  &= ~(1<<EPNUMS))
   //! tests if the current endpoint is enabled
#define Is_usb_endpoint_enabled()                 ((UECONX & (1<<EPEN))    ? TRUE : FALSE)
   //! tests if STALL handshake request is running
#define Is_usb_endpoint_stall_requested()         ((UECONX & (1<<STALLRQ)) ? TRUE : FALSE)

   //! configures the current endpoint
#define Usb_configure_endpoint_type(type)         (UECFG0X =   (UECFG0X & ~(MSK_EPTYPE)) | ((U8)type << 6))
   //! configures the current endpoint direction
#define Usb_configure_endpoint_direction(dir)     (UECFG0X =   (UECFG0X & ~(1<<EPDIR))  | ((U8)dir))

   //! configures the current endpoint size
#define Usb_configure_endpoint_size(size)         (UECFG1X =   (UECFG1X & ~MSK_EPSIZE) | ((U8)size << 4))
   //! configures the current endpoint number of banks
#define Usb_configure_endpoint_bank(bank)         (UECFG1X =   (UECFG1X & ~MSK_EPBK)   | ((U8)bank << 2))
   //! allocates the current configuration in DPRAM memory
#define Usb_allocate_memory()                     (UECFG1X |=  (1<<ALLOC))
   //! un-allocates the current configuration in DPRAM memory
#define Usb_unallocate_memory()                   (UECFG1X &= ~(1<<ALLOC))

   //! acks endpoint overflow interrupt
#define Usb_ack_overflow_interrupt()              (UESTA0X &= ~(1<<OVERFI))
   //! acks endpoint underflow memory
#define Usb_ack_underflow_interrupt()             (UESTA0X &= ~(1<<UNDERFI))
   //! acks Zero Length Packet received
#define Usb_ack_zlp()                             (UESTA0X &= ~(1<<ZLPSEEN))
   //! returns data toggle
#define Usb_data_toggle()                         ((UESTA0X&MSK_DTSEQ) >> 2)
   //! returns the number of busy banks
#define Usb_nb_busy_bank()                        (UESTA0X &   MSK_NBUSYBK)
   //! tests if at least one bank is busy
#define Is_usb_one_bank_busy()                    ((UESTA0X &  MSK_NBUSYBK) == 0 ? FALSE : TRUE)
   //! tests if current endpoint is configured
#define Is_endpoint_configured()                  ((UESTA0X &  (1<<CFGOK))   ? TRUE : FALSE)
   //! tests if an overflows occurs
#define Is_usb_overflow()                         ((UESTA0X &  (1<<OVERFI))  ? TRUE : FALSE)
   //! tests if an underflow occurs
#define Is_usb_underflow()                        ((UESTA0X &  (1<<UNDERFI)) ? TRUE : FALSE)
   //! tests if a ZLP has been detected
#define Is_usb_zlp()                              ((UESTA0X &  (1<<ZLPSEEN)) ? TRUE : FALSE)

   //! returns the control direction
#define Usb_control_direction()                   ((UESTA1X &  (1<<CTRLDIR)) >> 2)
   //! returns the number of the current bank
#define Usb_current_bank()                        ( UESTA1X & MSK_CURRBK)

   //! clears FIFOCON bit
#define Usb_ack_fifocon()                         (UEINTX &= ~(1<<FIFOCON))
   //! acks NAK IN received
#define Usb_ack_nak_in()                          (UEINTX &= ~(1<<NAKINI))
   //! acks NAK OUT received
#define Usb_ack_nak_out()                         (UEINTX &= ~(1<<NAKOUTI))
   //! acks receive SETUP
#define Usb_ack_receive_setup()                   (UEINTX &= ~(1<<RXSTPI))
   //! acks reveive OUT
#define Usb_ack_receive_out()                     (UEINTX &= ~(1<<RXOUTI), Usb_ack_fifocon())
   //! acks STALL sent
#define Usb_ack_stalled()                         (MSK_STALLEDI=   0)
   //! acks IN ready
#define Usb_ack_in_ready()                        (UEINTX &= ~(1<<TXINI), Usb_ack_fifocon())
   //! Kills last bank
#define Usb_kill_last_in_bank()                   (UENTTX |= (1<<RXOUTI))
   //! tests if endpoint read allowed
#define Is_usb_read_enabled()                     (UEINTX&(1<<RWAL))
   //! tests if endpoint write allowed
#define Is_usb_write_enabled()                    (UEINTX&(1<<RWAL))
   //! tests if read allowed on control endpoint
#define Is_usb_read_control_enabled()             (UEINTX&(1<<TXINI))
   //! tests if a NAK has been sent on IN endpoint
#define Is_usb_nak_in_sent()                      (UEINTX&(1<<NAKINI))
   //! tests if a NAK has been sent on OUT endpoint
#define Is_usb_nak_out_sent()                     (UEINTX&(1<<NAKOUTI))
   //! tests if SETUP received
#define Is_usb_receive_setup()                    (UEINTX&(1<<RXSTPI))
   //! tests if OUT received
#define Is_usb_receive_out()                      (UEINTX&(1<<RXOUTI))
   //! tests if IN ready
#define Is_usb_in_ready()                         (UEINTX&(1<<TXINI))
   //! sends IN
#define Usb_send_in()                             (UEINTX &= ~(1<<FIFOCON))
   //! sends IN on control endpoint
#define Usb_send_control_in()                     (UEINTX &= ~(1<<TXINI))
   //! frees OUT bank
#define Usb_free_out_bank()                       (UEINTX &= ~(1<<FIFOCON))
   //! acks OUT on control endpoint
#define Usb_ack_control_out()                     (UEINTX &= ~(1<<RXOUTI))

   //! enables flow error interrupt
#define Usb_enable_flow_error_interrupt()         (UEIENX  |=  (1<<FLERRE))
   //! enables NAK IN interrupt
#define Usb_enable_nak_in_interrupt()             (UEIENX  |=  (1<<NAKINE))
   //! enables NAK OUT interrupt
#define Usb_enable_nak_out_interrupt()            (UEIENX  |=  (1<<NAKOUTE))
   //! enables receive SETUP interrupt
#define Usb_enable_receive_setup_interrupt()      (UEIENX  |=  (1<<RXSTPE))
   //! enables receive OUT interrupt
#define Usb_enable_receive_out_interrupt()        (UEIENX  |=  (1<<RXOUTE))
   //! enables STALL sent interrupt
#define Usb_enable_stalled_interrupt()            (UEIENX  |=  (1<<STALLEDE))
   //! enables IN ready interrupt
#define Usb_enable_in_ready_interrupt()           (UEIENX  |=  (1<<TXIN))
   //! disables flow error interrupt
#define Usb_disable_flow_error_interrupt()        (UEIENX  &= ~(1<<FLERRE))
   //! disables NAK IN interrupt
#define Usb_disable_nak_in_interrupt()            (UEIENX  &= ~(1<<NAKINE))
   //! disables NAK OUT interrupt
#define Usb_disable_nak_out_interrupt()           (UEIENX  &= ~(1<<NAKOUTE))
   //! disables receive SETUP interrupt
#define Usb_disable_receive_setup_interrupt()     (UEIENX  &= ~(1<<RXSTPE))
   //! disables receive OUT interrupt
#define Usb_disable_receive_out_interrupt()       (UEIENX  &= ~(1<<RXOUTE))
   //! disables STALL sent interrupt
#define Usb_disable_stalled_interrupt()           (UEIENX  &= ~(1<<STALLEDE))
   //! disables IN ready interrupt
#define Usb_disable_in_ready_interrupt()          (UEIENX  &= ~(1<<TXIN))

   //! returns FIFO byte for current endpoint
#define Usb_read_byte()                           (UEDATX)
   //! writes byte in FIFO for current endpoint
#define Usb_write_byte(byte)                      (UEDATX  =   (U8)byte)

   //! returns number of bytes in FIFO current endpoint (16 bits)
#define Usb_byte_counter()                        ((U8)(UEBCLX))
   //! returns number of bytes in FIFO current endpoint (8 bits)
#define Usb_byte_counter_8()                      ((U8)UEBCLX)

   //! tests the general endpoint interrupt flags
#define Usb_interrupt_flags()                     (UEINT)
   //! tests the general endpoint interrupt flags
#define Is_usb_endpoint_event()                   (Usb_interrupt_flags() != 0x00)
//! @}



//! wSWAP
//! This macro swaps the U8 order in words.
//!
//! @param x        (U16) the 16 bit word to swap
//!
//! @return         (U16) the 16 bit word x with the 2 bytes swaped

#define wSWAP(x)        \
   (   (((x)>>8)&0x00FF) \
   |   (((x)<<8)&0xFF00) \
   )


//! Usb_write_word_enum_struc
//! This macro help to fill the U16 fill in USB enumeration struct.
//! Depending on the CPU architecture, the macro swap or not the nibbles
//!
//! @param x        (U16) the 16 bit word to be written
//!
//! @return         (U16) the 16 bit word written
#if !defined(BIG_ENDIAN) && !defined(LITTLE_ENDIAN)
   #error YOU MUST Define the Endian Type of target: LITTLE_ENDIAN or BIG_ENDIAN
#endif
#ifdef LITTLE_ENDIAN
   #define Usb_write_word_enum_struc(x)   (x)
#else //BIG_ENDIAN
   #define Usb_write_word_enum_struc(x)   (wSWAP(x))
#endif


//! @}

//_____ D E C L A R A T I O N ______________________________________________

U8      usb_config_ep                (U8, U8);
U8      usb_select_enpoint_interrupt (void);
U16     usb_get_nb_byte_epw          (void);
U8      usb_send_packet              (U8 , U8*, U8);
U8      usb_read_packet              (U8 , U8*, U8);
void    usb_halt_endpoint            (U8);
void    usb_reset_endpoint           (U8);
U8      usb_init_device              (void);


#endif  // _USB_DRV_H_

