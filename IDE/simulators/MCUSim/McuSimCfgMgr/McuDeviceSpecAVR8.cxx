// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup McuSimCfgMgr
 * @file McuDeviceSpecAVR8.cxx
 */
// =============================================================================

#include "McuDeviceSpecAVR8.h"

#include <QDebug>
#include <cmath>

McuDeviceSpecAVR8::McuDeviceSpecAVR8() : McuDeviceSpec(MCUSim::Arch::ARCH_AVR8)
{
    for ( int i = 0; i < AVR8PinNames::SPF__MAX__; i++ )
    {
        m_io.m_specFuncMap[i] = AVR8PinNames::PIN_NC;
    }
}

McuDeviceSpecAVR8::DataMemory::DataMemory()
{
    m_ioRegInitValues = NULL;
    m_ioRegRandomInit = NULL;
    m_mem2sizes = NULL;
    m_ioMem2InitValues = NULL;
    m_mem2size = 0;
    m_ioRegDesc = NULL;
}

McuDeviceSpecAVR8::DataMemory::~DataMemory()
{
    if ( NULL != m_ioRegInitValues )
    {
        delete[] m_ioRegInitValues;
    }

    if ( NULL != m_ioRegRandomInit )
    {
        delete[] m_ioRegRandomInit;
    }

    if ( NULL != m_mem2sizes )
    {
        delete[] m_mem2sizes;
    }

    if ( NULL != m_ioMem2InitValues )
    {
        for ( unsigned int i = 0; i < m_mem2size; i++ )
        {
            if ( NULL != m_ioMem2InitValues[i] )
            {
                delete[] m_ioMem2InitValues[i];
            }
        }
        delete[] m_ioMem2InitValues;
    }

    if ( NULL != m_ioRegDesc )
    {
        delete[] m_ioRegDesc;
    }
}

bool McuDeviceSpecAVR8::setupSimulator ( AVR8Config & mcuConfig ) const
{
    mcuConfig.m_family = m_family;
    mcuConfig.m_name = m_name.toStdString();

    // A/D converter
    mcuConfig.m_configAdc->m_enabled = m_adc.m_enabled;
    // Analog comparator
    mcuConfig.m_configAcomp->m_enabled = m_analogComparator.m_enabled;
    // Boot loader
    mcuConfig.m_configBootLoader->m_enabled = m_bootLoader.m_enabled;
    mcuConfig.m_configBootLoader->m_minProgTime = m_bootLoader.m_minProgTime;
    mcuConfig.m_configBootLoader->m_maxProgTime = m_bootLoader.m_maxProgTime;
    mcuConfig.m_configBootLoader->m_pageSize = m_bootLoader.m_pageSize;
    for ( int i = 0; i < 4; i++ )
    {
        mcuConfig.m_configBootLoader->m_bootResetAddress[i] = m_bootLoader.m_bootResetAddress[i];
    }
    mcuConfig.m_configBootLoader->m_rwwSectionSize = m_bootLoader.m_rwwSectionSize;
    // Clock control (empty)
    // mcuConfig.m_configClockControl->SOMETHING = m_clockControl.SOMETHING;
    // Data EEPROM
    mcuConfig.m_configDataEEPROM->m_enabled = m_dataEEPROM.m_enabled;
    mcuConfig.m_configDataEEPROM->m_addrRegWidth = m_dataEEPROM.m_addrRegWidth;
    mcuConfig.m_configDataEEPROM->m_size = m_dataEEPROM.m_size;
    mcuConfig.m_configDataEEPROM->m_writeTime = m_dataEEPROM.m_writeTime;
    // Data memory
    if ( NULL != mcuConfig.m_configDataMemory->m_ioRegInitValues )
    {
        delete [] mcuConfig.m_configDataMemory->m_ioRegInitValues;
    }
    if ( NULL != mcuConfig.m_configDataMemory->m_ioRegRandomInit )
    {
        delete [] mcuConfig.m_configDataMemory->m_ioRegRandomInit;
    }
    if ( NULL != mcuConfig.m_configDataMemory->m_mem2sizes )
    {
        delete [] mcuConfig.m_configDataMemory->m_mem2sizes;
    }
    if ( NULL != mcuConfig.m_configDataMemory->m_ioMem2InitValues )
    {
        for ( unsigned int i = 0; i < mcuConfig.m_configDataMemory->m_mem2size; i++ )
        {
            if ( NULL != mcuConfig.m_configDataMemory->m_ioMem2InitValues[i] )
            {
                delete [] mcuConfig.m_configDataMemory->m_ioMem2InitValues[i];
            }
        }
        delete [] mcuConfig.m_configDataMemory->m_ioMem2InitValues;
    }
    mcuConfig.m_configDataMemory->m_regFileSize = m_dataMemory.m_regFileSize;
    mcuConfig.m_configDataMemory->m_sramSize = m_dataMemory.m_sramSize;
    mcuConfig.m_configDataMemory->m_ioRegSize = m_dataMemory.m_ioRegSize;
    mcuConfig.m_configDataMemory->m_ioRegInitValues = new uint32_t[m_dataMemory.m_ioRegSize];
    mcuConfig.m_configDataMemory->m_ioRegRandomInit = new uint8_t[m_dataMemory.m_ioRegSize];
    for ( unsigned int i = 0; i < m_dataMemory.m_ioRegSize; i++ )
    {
        mcuConfig.m_configDataMemory->m_ioRegInitValues[i] = m_dataMemory.m_ioRegInitValues[i];
        mcuConfig.m_configDataMemory->m_ioRegRandomInit[i] = m_dataMemory.m_ioRegRandomInit[i];
    }
    mcuConfig.m_configDataMemory->m_mem2size = m_dataMemory.m_mem2size;
    mcuConfig.m_configDataMemory->m_mem2sizes = new unsigned int[m_dataMemory.m_mem2size];
    for ( unsigned int i = 0; i < m_dataMemory.m_mem2size; i++ )
    {
        mcuConfig.m_configDataMemory->m_mem2sizes[i] = m_dataMemory.m_mem2sizes[i];
    }
    mcuConfig.m_configDataMemory->m_ioMem2InitValues = new uint32_t * [m_dataMemory.m_mem2size];
    for ( unsigned int i = 0; i < m_dataMemory.m_mem2size; i++ )
    {
        mcuConfig.m_configDataMemory->m_ioMem2InitValues[i] = new uint32_t[m_dataMemory.m_mem2sizes[i]];
        for ( unsigned int j = 0; j < m_dataMemory.m_mem2sizes[i]; j++ )
        {
            mcuConfig.m_configDataMemory->m_ioMem2InitValues[i][j] = m_dataMemory.m_ioMem2InitValues[i][j];
        }
    }
    mcuConfig.m_configDataMemory->m_spWidth = m_dataMemory.m_spWidth;
    mcuConfig.m_configDataMemory->m_spMax = pow(2, m_dataMemory.m_spWidth);
    // External interrupts
    mcuConfig.m_configExternalInterrupts->m_enabled = m_externalInterrupts.m_enabled;
    // Fuses and lock bits
    mcuConfig.m_configFusesAndLocks->m_defaultFuses = m_fusesAndLocks.m_defaultFuses;
    mcuConfig.m_configFusesAndLocks->m_defaultLockBits = m_fusesAndLocks.m_defaultLockBits;
    // Instruction set
    for ( int i = 0; i < AVR8InsNames::SPECI__MAX__; i++ )
    {
        mcuConfig.m_configInstructionSet->m_availableInstructions[i] = m_instructionSet.m_availableInstructions[i];
    }
    mcuConfig.m_configInstructionSet->m_family = m_family;
    mcuConfig.m_configInstructionSet->m_pcWidth = m_instructionSet.m_pcWidth;
    switch ( m_instructionSet.m_pcWidth )
    {
        case AVR8InstructionSet::PCWIDTH_16:
            mcuConfig.m_configInstructionSet->m_pcMax = 0x10000; // 2¹⁶
            break;
        case AVR8InstructionSet::PCWIDTH_22:
            mcuConfig.m_configInstructionSet->m_pcMax = 0x400000; // 2²²
            break;
        default:
            qDebug() << "m_instructionSet.m_pcWidth is invalid";
            return false;
    }
    // Interrupt controller
    for ( int i = 0; i < AVR8InterruptController::INTVEC__MAX__; i++ )
    {
        mcuConfig.m_configInterruptController->m_possibleInterrupts[i] = m_interruptController.m_possibleInterrupts[i];

        if ( ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_INT0] )
                 ||
             ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_INT1] ) )
        {
            mcuConfig.m_configInterruptController->m_interruptGroups[AVR8InterruptController::INTGRP_EXTERNAL] = true;
        }
        else
        {
            mcuConfig.m_configInterruptController->m_interruptGroups[AVR8InterruptController::INTGRP_EXTERNAL] = false;
        }

        if ( ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_TIMER2_COMP] )
                 ||
             ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_TIMER2_OVF] )
                 ||
             ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_TIMER1_CAPT] )
                 ||
             ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_TIMER1_COMPA] )
                 ||
             ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_TIMER1_COMPB] )
                 ||
             ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_TIMER1_OVF] )
                 ||
             ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_TIMER0_OVF] ) )
        {
            mcuConfig.m_configInterruptController->m_interruptGroups[AVR8InterruptController::INTGRP_TIMER012] = true;
        }
        else
        {
            mcuConfig.m_configInterruptController->m_interruptGroups[AVR8InterruptController::INTGRP_TIMER012] = false;
        }

        if ( ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_USART_RXC] )
                 ||
             ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_USART_UDRE] )
                 ||
             ( true == m_interruptController.m_possibleInterrupts[AVR8InterruptController::INTVEC_USART_TXC] ) )
        {
            mcuConfig.m_configInterruptController->m_interruptGroups[AVR8InterruptController::INTGRP_UART] = true;
        }
        else
        {
            mcuConfig.m_configInterruptController->m_interruptGroups[AVR8InterruptController::INTGRP_UART] = false;
        }
    }

    // Input and output
    mcuConfig.m_configIO->m_random = m_io.m_random;
    mcuConfig.m_configIO->m_pullUpresistance = m_io.m_pullUpresistance;
    for ( int i = 0; i < AVR8PinNames::SPF__MAX__; i++ )
    {
        mcuConfig.m_configIO->m_specFuncMap[i] = m_io.m_specFuncMap[i];
    }
    for ( unsigned int i = 0; i < AVR8IO::NUMBER_OF_PORTS; i++ )
    {
        mcuConfig.m_configIO->m_hasPort[i] = m_io.m_hasPort[i];
    }
    for ( unsigned int i = 0; i < AVR8IO::NUMBER_OF_PINS; i++ )
    {
        mcuConfig.m_configIO->m_availablePins[i] = m_io.m_availablePins[i];
    }
    // ISP - In-System Programming interface
    mcuConfig.m_configIsp->m_enabled = m_isp.m_enabled;
    // Parallel programming interface
    mcuConfig.m_configPprog->m_enabled = m_parallelProg.m_enabled;
    // Program memory
    mcuConfig.m_configProgramMemory->m_size = m_programMemory.m_size;
    // SPI - Serial Peripheral Interface
    mcuConfig.m_configSpi->m_enabled = m_spi.m_enabled;
    // System control
    mcuConfig.m_configSystemControl->m_resetTresholdLow = m_systemControl.m_resetTresholdLow;
    mcuConfig.m_configSystemControl->m_resetTresholdHigh = m_systemControl.m_resetTresholdHigh;
    mcuConfig.m_configSystemControl->m_minResetPulseWidth = m_systemControl.m_minResetPulseWidth;
    mcuConfig.m_configSystemControl->m_powerOnResetTresholdFalling = m_systemControl.m_powerOnResetTresholdFalling;
    mcuConfig.m_configSystemControl->m_powerOnResetTresholdRaising = m_systemControl.m_powerOnResetTresholdRaising;
    mcuConfig.m_configSystemControl->m_tBOD = m_systemControl.m_tBOD;
    mcuConfig.m_configSystemControl->m_brownOutTreshold[0] = m_systemControl.m_brownOutTreshold[0];
    mcuConfig.m_configSystemControl->m_brownOutTreshold[1] = m_systemControl.m_brownOutTreshold[1];
    mcuConfig.m_configSystemControl->m_hasPowerOnReset = m_systemControl.m_hasPowerOnReset;
    mcuConfig.m_configSystemControl->m_hasBrownOutReset = m_systemControl.m_hasBrownOutReset;
    mcuConfig.m_configSystemControl->m_hasExternalReset = m_systemControl.m_hasExternalReset;
    mcuConfig.m_configSystemControl->m_hasWatchDog = m_systemControl.m_hasWatchDog;
    // Timer/Counter 0
    mcuConfig.m_configTimerCounter0->m_enabled = m_timerCounter0.m_enabled;
    // Timer/Counter 1
    mcuConfig.m_configTimerCounter1->m_enabled = m_timerCounter1.m_enabled;
    // Timer/Counter 2
    mcuConfig.m_configTimerCounter2->m_enabled = m_timerCounter2.m_enabled;
    // TWI - Two Wire Interface
    mcuConfig.m_configTwi->m_enabled = m_twi.m_enabled;
    // USART - Universal Synchronous/Asynchronous Receiver/Transmitter
    mcuConfig.m_configUsart->m_enabled = m_usart.m_enabled;
    // Watchdog timer (empty)
    // mcuConfig.m_configWatchdogTimer->SOMETHING = m_watchdogTimer.SOMETHING;

    return true;
}
