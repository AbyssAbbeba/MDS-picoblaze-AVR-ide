/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Driver routines to read (no write) datas stored in AVRMega flash
 * These routines can be stored and executed in all flash space.
 *
 * - Compiler:           GNU GCC for AVR
 * - Supported devices:  All AVRMega devices
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 *****************************************************************************/

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
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#ifndef __GNUC__
#error Assembler file supported only by GNU GCC
#endif

//_____ I N C L U D E S ______________________________________________________


#include <avr/io.h>     // Registers declarations
  
//_____ M A C R O S ________________________________________________________

//! Macro for I/O Location used in following instructions :
//! "IN - Load an I/O Location to Register", "OUT - Store Register to I/O Location", ...
#define _ASM_SFR_IO_( sfr ) (sfr-0x20)


//_____ D E C L A R A T I O N S ______________________________________________

   .global  flash_read_sig
   .global  flash_read_fuse
   

//! @{
//! \verbatim

//! @brief This macro function allows to read device IDs of the product.
//!
//! @param  add   Address of device ID to read.
//!
//! @return byte  Read value (R24)
//!
flash_read_sig:
   RCALL    WAIT_SPMEN                       // Wait for SPMEN flag cleared
   MOV      R31,R23
   MOV      R30,R22                          // move adress to z pointer (R31=ZH R30=ZL)
   LDI      R20, ((1<<SPMEN) | (1<<SIGRD))  
   OUT      _ASM_SFR_IO_(SPMCSR), R20        // argument 2 decides function (r18)
   LPM                                       // Store program memory
   MOV      R24, R0                          // Store return value
   RJMP     WAIT_SPMEN                       // Wait for SPMEN flag cleared


//! @brief This macro function allows to read a fuse byte of the product.
//!
//! @param  add   Address of fuse to read.
//! 
//! @return byte  Read value (R24)
//!
flash_read_fuse:
   RCALL    WAIT_SPMEN                       // Wait for SPMEN flag cleared
   MOV      R31,R23
   MOV      R30,R22                          // move adress to z pointer (R31=ZH R30=ZL)
   LDI      R20,((1<<SPMEN) | (1<<BLBSET))  
   OUT      _ASM_SFR_IO_(SPMCSR), R20        // argument 2 decides function (r18)
   LPM                                       // Store program memory
   MOV      R24, R0                          // Store return value
   RJMP     WAIT_SPMEN                       // Wait for SPMEN flag cleared

//! @brief Performs an active wait on SPME flag
//!
WAIT_SPMEN:
   MOV      R0, R18
   IN       R18,_ASM_SFR_IO_(SPMCSR)         // get SPMCR into r18
   SBRC     R18,SPMEN
   RJMP     WAIT_SPMEN                       // Wait for SPMEN flag cleared
   MOV      R18, R0
   RET

//! \endverbatim
//! @}

