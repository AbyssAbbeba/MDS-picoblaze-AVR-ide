// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @authors Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup PIC8
 * @file PIC8IO.cxx
 */
// =============================================================================

//          ╭────────────╮
// PIN █──┬─┤ RESISTANCE ├─────▎← VOLTAGE_INT
//        │ ╰────────────╯
//        ╰────────────────────▎→ REAL_VOLTAGE
//

#include "PIC8IO.h"
#include "PIC8DataMemory.h"

#include <cmath>

#ifndef NAN
  #error 'NAN' macro is not defined
#endif

// PIC8IO::PIC8IO() {
//      m_lowLevelInterface = new SimFloatType * [II__MAX__];
//      for ( int i = 0; i < II__MAX__; i++ ) {
//              m_lowLevelInterface[i] = new SimFloatType[NUMBER_OF_PINS];
//
//              for ( unsigned int j = 0; j < NUMBER_OF_PINS; j++ ) {
//                      m_lowLevelInterface[i][j] = 0;
//              }
//      }
//
//      setSourceVoltage(5); // default voltage is 5V
//
//      m_enabled = false;
// }
//
// PIC8IO * PIC8IO::link(
//              MCUSim::EventLogger     * eventLogger,
//              PIC8DataMemory          * dataMemory
// ) {
//      IO::link(eventLogger);
//
//      m_dataMemory = dataMemory;
//
//      return this;
// }
//
// PIC8IO::~PIC8IO() {
//      if ( NULL != m_lowLevelInterface ) {
//              for ( int i = 0; i < II__MAX__; i++ ) {
//                      delete[] m_lowLevelInterface[i];
//              }
//              delete[] m_lowLevelInterface;
//      }
// }
//
// void PIC8IO::clockCycles() {
//      if ( false == m_enabled ) {
//              return;
//      }
//
//      /*
//       * - Read port states and set new ones.
//       * - Adjust port configurations to the I/O flags DDRx registers
//       */
//      m_PullUpDisabled = m_dataMemory->readBitFast(PIC8RegNames::SFIOR, PIC8RegNames::SFIOR_PUD);
//      if ( true == m_config.m_hasPort[0] ) {
//              portSetDirection(PIC8PinNames::PIN_PA0, 0, PIC8RegNames::DDRA);
//              portReadWrite(PIC8PinNames::PIN_PA0, PIC8RegNames::PINA, PIC8RegNames::PORTA);
//      }
//      if ( true == m_config.m_hasPort[1] ) {
//              portSetDirection(PIC8PinNames::PIN_PB0, 1, PIC8RegNames::DDRB);
//              portReadWrite(PIC8PinNames::PIN_PB0, PIC8RegNames::PINB, PIC8RegNames::PORTB);
//      }
//      if ( true == m_config.m_hasPort[2] ) {
//              portSetDirection(PIC8PinNames::PIN_PC0, 2, PIC8RegNames::DDRC);
//              portReadWrite(PIC8PinNames::PIN_PC0, PIC8RegNames::PINC, PIC8RegNames::PORTC);
//      }
//      if ( true == m_config.m_hasPort[3] ) {
//              portSetDirection(PIC8PinNames::PIN_PD0, 3, PIC8RegNames::DDRD);
//              portReadWrite(PIC8PinNames::PIN_PD0, PIC8RegNames::PIND, PIC8RegNames::PORTD);
//      }
//
// }
//
// inline void PIC8IO::portSetDirection(unsigned int pinIdx, int portIndex, const unsigned int ddrAddr) {
//      const unsigned int ddr = m_dataMemory->readFast(ddrAddr);
//      if ( ddr == m_lastDdr[portIndex]) {
//              return;
//      }
//      m_lastDdr[portIndex] = ddr;
//
//      for ( int i = 0x1; i < 0x100; i <<= 1, pinIdx++ ) {
//              if ( ( PINMD_NOT_IMPLEMENTED | PINMD_OUTPUT_OVERRIDE ) & m_pinMode[pinIdx] ) {
//                      continue;
//              }
//
//              SimFloatType resistance;
//
//              if ( ddr & i ) {
//                      // Output
//                      resistance = 0;
//              } else {
//                      // Input
//                      if ( true == m_PullUpDisabled ) {
//                              resistance = INFINITY;
//                      } else {
//                              resistance = m_config.m_pullUpresistance;
//                      }
//              }
//              setResistance((PIC8PinNames::PIN)pinIdx, resistance);
//      }
// }
//
// // This takes into account input and output override, and the input synchronizer.
// inline void PIC8IO::portReadWrite(unsigned int pinIdx, const unsigned int inRegAddr, const unsigned int outRegAddr) {
//      unsigned int in = 0;
//      unsigned int out = m_dataMemory->readFast(outRegAddr);
//
//      for ( int i = 0x1; i < 0x100; i <<= 1, pinIdx++ ) {
//              if ( PINMD_NOT_IMPLEMENTED & m_pinMode[pinIdx] ) {
//                      continue;
//              }
//
//              // Read pin state
//              if ( !(PINMD_INPUT_OVERRIDE & m_pinMode[pinIdx]) ) {
//                      if ( true == getLog ( (PIC8PinNames::PIN)pinIdx ) ) {
//                              in |= i;
//                      }
//              }
//
//              // Set new pin state
//              if ( !(PINMD_OUTPUT_OVERRIDE & m_pinMode[pinIdx]) ) {
//                      setLog ( (PIC8PinNames::PIN)pinIdx, bool(out & i) );
//              }
//      }
//
//      m_dataMemory->writeFast(inRegAddr, in);
// }
//
// void PIC8IO::reset(MCUSim::ResetMode mode) {
//      switch ( mode ) {
//              case MCUSim::RSTMD_NEW_CONFIG:
//                      loadConfig();
//                      break;
//              case MCUSim::RSTMD_INITIAL_VALUES:
//                      resetToInitialValues();
//                      break;
//              case MCUSim::RSTMD_MCU_RESET:
//                      mcuReset();
//                      break;
//              default:
//                      // Irrelevant requests are silently ignored
//                      break;
//      }
// }
//
// inline void PIC8IO::loadConfig() {
//      for ( unsigned int i = 0; i < NUMBER_OF_PINS; i++ ) {
//              if ( true == m_config.m_availablePins[i] ) {
//                      m_pinMode[i] = PINMD_NOT_IMPLEMENTED;
//              } else {
//                      m_pinMode[i] = PINMD_GENERAL_IO;
//              }
//      }
// }
//
// inline void PIC8IO::resetToInitialValues() {
//      // TODO: Not implemented yet!
// }
//
// inline void PIC8IO::mcuReset() {
//      for ( unsigned int i = 0; i < NUMBER_OF_PINS; i++ ) {
//              if ( PINMD_NOT_IMPLEMENTED & m_pinMode[i] ) {
//                      m_lowLevelInterface[II_VOLTAGE_INT][i]  = 0;
//                      m_lowLevelInterface[II_REAL_VOLTAGE][i] = 0;
//                      m_lowLevelInterface[II_CURRENT][i]      = 0;
//                      m_lowLevelInterface[II_RESISTANCE][i]   = NAN; // pin is not implemented
//              } else {
//                      for ( int j = 0; j < II__MAX__; j++ ) {
//                              m_lowLevelInterface[j][i] = 0;
//                      }
//                      m_pinMode[i] = PINMD_GENERAL_IO;
//              }
//      }
//      for ( unsigned int i = 0; i < NUMBER_OF_PORTS; i++ ) {
//              m_lastDdr[i] = (unsigned int) -1;
//      }
// }
