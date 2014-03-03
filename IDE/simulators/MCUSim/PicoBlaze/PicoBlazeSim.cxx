// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PicoBlaze
 * @file PicoBlazeSim.cxx
 */
// =============================================================================

#include "PicoBlazeSim.h"

#include "PicoBlazeIO.h"
#include "PicoBlazeStack.h"
#include "PicoBlazeConfig.h"
#include "PicoBlazeRegisters.h"
#include "PicoBlazeDataMemory.h"
#include "PicoBlazeProgramMemory.h"
#include "PicoBlazeInstructionSet.h"
#include "PicoBlazeInstructionSet1.h"
#include "PicoBlazeInstructionSet2.h"
#include "PicoBlazeInstructionSet3.h"
#include "PicoBlazeInstructionSet6.h"
#include "PicoBlazeInstructionSet1CPLD.h"
#include "PicoBlazeInterruptController.h"

#include <cassert>

PicoBlazeSim::PicoBlazeSim()
{
    m_config                = new PicoBlazeConfig();
    m_eventLogger           = new MCUSimEventLogger();

    m_io                    = new PicoBlazeIO();
    m_stack                 = new PicoBlazeStack();
    m_registers             = new PicoBlazeRegisters();
    m_dataMemory            = new PicoBlazeDataMemory();
    m_statusFlags           = new PicoBlazeStatusFlags();
    m_clockControl          = new PicoBlazeClockControl();
    m_programMemory         = new PicoBlazeProgramMemory();
    m_instructionSet        = new PicoBlazeInstructionSet3();
    m_interruptController   = new PicoBlazeInterruptController();

    regSubSys ( m_io             -> link ( m_eventLogger ) );
    regSubSys ( m_stack          -> link ( m_eventLogger ) );
    regSubSys ( m_registers      -> link ( m_eventLogger ) );
    regSubSys ( m_dataMemory     -> link ( m_eventLogger ) );
    regSubSys ( m_statusFlags    -> link ( m_eventLogger ) );
    regSubSys ( m_clockControl   -> link ( m_eventLogger ) );
    regSubSys ( m_programMemory  -> link ( m_eventLogger ) );

    regSubSys ( m_instructionSet -> link ( m_eventLogger,
                                           m_io,
                                           m_stack,
                                           m_registers,
                                           m_dataMemory,
                                           m_statusFlags,
                                           m_programMemory,
                                           m_interruptController ) );

    regSubSys ( m_interruptController -> link ( m_eventLogger, m_instructionSet, m_statusFlags ) );

    checkSubSystems();

    m_config->link(this);
}

PicoBlazeSim::~PicoBlazeSim()
{
    deleteSubSystems();

    delete m_config;
    delete m_eventLogger;
}

inline void PicoBlazeSim::deleteSubSystems()
{
    for ( auto subsys : m_subSystems )
    {
        delete subsys;
    }
}

inline void PicoBlazeSim::checkSubSystems() const
{
    for ( const auto subsys : m_subSystems )
    {
        assert ( MCUSimSubsys::ID_INVALID != subsys->getId() );
    }
}

inline void PicoBlazeSim::regSubSys ( MCUSimSubsys * subSystem )
{
    m_subSystems.push_back(subSystem);
}


inline void PicoBlazeSim::unregSubSys ( const MCUSimSubsys * subSystem )
{
    for ( auto it = m_subSystems.begin();
          it != m_subSystems.end();
          ++it )
    {
        if ( subSystem == *it )
        {
            m_subSystems.erase(it);
        }
    }
}

MCUSimSubsys * PicoBlazeSim::getSubsys ( MCUSimSubsys::SubsysId id )
{
    switch ( id )
    {
        case MCUSimSubsys::ID_MEM_CODE:       return m_programMemory;
        case MCUSimSubsys::ID_MEM_DATA:       return m_dataMemory;
        case MCUSimSubsys::ID_MEM_REGISTERS:  return m_registers;
        case MCUSimSubsys::ID_CPU:            return m_instructionSet;
        case MCUSimSubsys::ID_PLIO:           return m_io;
        case MCUSimSubsys::ID_STACK:          return m_stack;
        case MCUSimSubsys::ID_CLK_CONTROL:    return m_clockControl;
        case MCUSimSubsys::ID_INTERRUPTS:     return m_interruptController;
        case MCUSimSubsys::ID_FLAGS:          return m_statusFlags;

        default:                              return nullptr;
    }
}

MCUSimClock::ClockSource & PicoBlazeSim::getClockSource()
{
    return m_clockControl->m_clockSource;
}

void PicoBlazeSim::reset ( ResetMode mode )
{
    for ( auto subsys : m_subSystems )
    {
        subsys->reset(mode);
    }

    switch ( mode )
    {
        case MCUSim::RSTMD_NEW_CONFIG:
            loadConfig();
            break;
        case MCUSim::RSTMD_INITIAL_VALUES:
            resetToInitialValues();
            break;
        case MCUSim::RSTMD_MCU_RESET:
            mcuReset();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

MCUSimConfig & PicoBlazeSim::getConfig()
{
    return * m_config;
}

MCUSim::Family PicoBlazeSim::family() const
{
    return m_config->m_family;
}

const char * PicoBlazeSim::name() const
{
    return m_config->m_name.c_str();
}

MCUSim::Arch PicoBlazeSim::arch() const
{
    return m_config->getArch();
}

inline void PicoBlazeSim::resetToInitialValues()
{
    m_clockPeriod = ( 1.0 / m_clockControl->getFrequency() );
}

inline void PicoBlazeSim::loadConfig()
{
    // Change instruction set, if the current one is no longer usable for the chosen device.
    if ( false == m_instructionSet->isValid() )
    {
        PicoBlazeInstructionSet * origInstructionSet = m_instructionSet;
        switch ( m_instructionSet->m_config.m_dev )
        {
            case MCUSim::FAMILY_KCPSM1CPLD:
                m_instructionSet = new PicoBlazeInstructionSet1CPLD();
                break;
            case MCUSim::FAMILY_KCPSM1:
                m_instructionSet = new PicoBlazeInstructionSet1();
                break;
            case MCUSim::FAMILY_KCPSM2:
                m_instructionSet = new PicoBlazeInstructionSet2();
                break;
            case MCUSim::FAMILY_KCPSM3:
                m_instructionSet = new PicoBlazeInstructionSet3();
                break;
            case MCUSim::FAMILY_KCPSM6:
                m_instructionSet = new PicoBlazeInstructionSet6();
                break;
            default:
                break;
        }

        m_instructionSet->adapt(origInstructionSet);
        m_interruptController->link(m_eventLogger, m_instructionSet, m_statusFlags);
        delete origInstructionSet;

        unregSubSys(origInstructionSet);
        regSubSys(m_instructionSet);
    }
}

inline void PicoBlazeSim::mcuReset()
{
    m_time = 0;
}

int PicoBlazeSim::executeInstruction()
{
    m_io->clockCycle();

    int intCode = m_interruptController->autoInterrupt();
    switch ( intCode )
    {
        case -1:
            reset ( MCUSim::RSTMD_MCU_RESET );
            break;
        case 0:
            m_instructionSet->execInstruction();
            break;
        default:
            break;
    }

    return 1;
}

int PicoBlazeSim::timeStep ( float timeStep )
{
    m_time += timeStep;

    int allocatedCycles = int(m_time / m_clockPeriod);
    while ( m_time >= m_clockPeriod )
    {
        m_time -= m_clockPeriod;
    }

    for ( int i = 0; i < allocatedCycles; i++ )
    {
        executeInstruction();
    }

    return allocatedCycles;
}
