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
 * @file McuDeviceSpecPIC8.cxx
 */
// =============================================================================

#include "McuDeviceSpecPIC8.h"

#include <cmath>

McuDeviceSpecPIC8::McuDeviceSpecPIC8()
{
    for ( int i = 0; i < PIC8PinNames::SPF__MAX__; i++ )
    {
        m_io.m_specFuncMap[i] = PIC8PinNames::PIN_NC;
    }
    for ( unsigned int i = 0; i < PIC8IO::NUMBER_OF_PORTS; i++ )
    {
        m_io.m_hasPort[i] = false;
    }
    for ( unsigned int i = 0; i < PIC8IO::NUMBER_OF_PINS; i++ )
    {
        m_io.m_availablePins[i] = false;
    }
}

McuDeviceSpecPIC8::DataMemory::DataMemory()
{
    m_addrTransTab = NULL;
    m_randomInit = NULL;
    m_initValues = NULL;
}

McuDeviceSpecPIC8::DataMemory::~DataMemory()
{
    if ( NULL != m_addrTransTab )
    {
        delete[] m_addrTransTab;
    }
    if ( NULL != m_randomInit )
    {
        delete[] m_randomInit;
    }
    if ( NULL != m_initValues )
    {
        delete m_initValues;
    }
}

bool McuDeviceSpecPIC8::setupSimulator ( PIC8Config & mcuConfig ) const
{
    mcuConfig.m_family = m_family;
    mcuConfig.m_name = m_name.toStdString();

    mcuConfig.m_configInstructionSet->m_family = m_family;
    mcuConfig.m_configInstructionSet->m_ignoreUndefinedOpCodes = false;
    mcuConfig.m_configInstructionSet->m_pcMax = ( m_programMemory.m_size - 1 );

    mcuConfig.m_configProgramMemory->m_size = m_programMemory.m_size;
    mcuConfig.m_configProgramMemory->m_configWordAddress = m_programMemory.m_configWordAddress;
    mcuConfig.m_configProgramMemory->m_idLocationsRange[0] = m_programMemory.m_idLocationsRange[0];
    mcuConfig.m_configProgramMemory->m_idLocationsRange[1] = m_programMemory.m_idLocationsRange[1];

    mcuConfig.m_configDataMemory->m_size = m_dataMemory.m_size;
    if ( NULL != mcuConfig.m_configDataMemory->m_addrTransTab )
    {
        delete [] mcuConfig.m_configDataMemory->m_addrTransTab;
    }
    if ( NULL != mcuConfig.m_configDataMemory->m_randomInit )
    {
        delete [] mcuConfig.m_configDataMemory->m_randomInit;
    }
    if ( NULL != mcuConfig.m_configDataMemory->m_initValues )
    {
        delete [] mcuConfig.m_configDataMemory->m_initValues;
    }
    mcuConfig.m_configDataMemory->m_addrTransTab = new int [ m_dataMemory.m_size ];
    for ( unsigned int i = 0; i < m_dataMemory.m_size; i++ )
    {
        mcuConfig.m_configDataMemory->m_addrTransTab[i] = m_dataMemory.m_addrTransTab[i];
    }
    mcuConfig.m_configDataMemory->m_randomInit = new unsigned int [ m_dataMemory.m_size ];
    for ( unsigned int i = 0; i < m_dataMemory.m_size; i++ )
    {
        mcuConfig.m_configDataMemory->m_randomInit[i] = m_dataMemory.m_randomInit[i];
    }
    mcuConfig.m_configDataMemory->m_initValues = new unsigned int [ m_dataMemory.m_size ];
    for ( unsigned int i = 0; i < m_dataMemory.m_size; i++ )
    {
        mcuConfig.m_configDataMemory->m_initValues[i] = m_dataMemory.m_initValues[i];
    }

    mcuConfig.m_configConfigWord->m_defaultCfgWord = m_configWord.m_defaultCfgWord;

    for ( int i = 0; i < PIC8PinNames::SPF__MAX__; i++ )
    {
        mcuConfig.m_configIO->m_specFuncMap[i] = m_io.m_specFuncMap[i];
    }
    for ( unsigned int i = 0; i < PIC8IO::NUMBER_OF_PORTS; i++ )
    {
        mcuConfig.m_configIO->m_hasPort[i] = m_io.m_hasPort[i];
    }
    for ( unsigned int i = 0; i < PIC8IO::NUMBER_OF_PINS; i++ )
    {
        mcuConfig.m_configIO->m_availablePins[i] = m_io.m_availablePins[i];
    }
    mcuConfig.m_configIO->m_pullUpresistance = m_io.m_pullUpresistance;
    mcuConfig.m_configIO->m_enabled = m_io.m_enabled;

    // mcuConfig.m_configClockControl-> = m_clockControl.;

    mcuConfig.m_configStack->m_size = m_stack.m_size;

    for ( int i = 0; i < PIC8InterruptController::INTVEC__MAX__; i++ )
    {
        mcuConfig.m_configInterruptController->m_possibleInterrupts[i] = m_interruptController.m_possibleInterrupts[i];
    }

    mcuConfig.m_configDataEEPROM->m_size = m_dataEEPROM.m_size;
    mcuConfig.m_configDataEEPROM->m_writeTime = m_dataEEPROM.m_writeTime;

    mcuConfig.m_configWatchDogTimer->m_enabled = m_watchDogTimer.m_enabled;

    mcuConfig.m_configTimerCounter0->m_enabled = m_timerCounter0.m_enabled;

    mcuConfig.m_configTimer0WdtPrescaller->m_enabled = m_timer0WdtPrescaller.m_enabled;

    mcuConfig.m_configExternalInterrupts->m_enabled = m_externalInterrupts.m_enabled;

    mcuConfig.m_configISP->m_enabled = m_isp.m_enabled;
    mcuConfig.m_configISP->m_delayBetweenCmdAndData = m_isp.m_delayBetweenCmdAndData;
    mcuConfig.m_configISP->m_progTime = m_isp.m_progTime;
    mcuConfig.m_configISP->m_commandSet = m_isp.m_commandSet;

    // Everything went correctly.
    return true;
}
