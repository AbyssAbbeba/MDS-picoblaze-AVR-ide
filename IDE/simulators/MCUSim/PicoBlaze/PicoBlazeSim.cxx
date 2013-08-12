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
 * @ingroup PicoBlaze
 * @file PicoBlazeSim.cxx
 */
// =============================================================================

#include "PicoBlazeSim.h"

#include "PicoBlazeConfig.h"
#include "PicoBlazeInstructionSet.h"
#include "PicoBlazeInstructionSet2.h"
#include "PicoBlazeInstructionSet3.h"
#include "PicoBlazeInstructionSet6.h"
#include "PicoBlazeProgramMemory.h"
#include "PicoBlazeDataMemory.h"
#include "PicoBlazeRegisters.h"
#include "PicoBlazeInterruptController.h"
#include "PicoBlazeIO.h"
#include "PicoBlazeStack.h"

#include <cassert>

PicoBlazeSim::PicoBlazeSim()
{
    m_eventLogger           = new MCUSimEventLogger();
    m_config                = new PicoBlazeConfig();

    m_programMemory         = new PicoBlazeProgramMemory();
    m_dataMemory            = new PicoBlazeDataMemory();
    m_registers             = new PicoBlazeRegisters();
    m_instructionSet        = new PicoBlazeInstructionSet3();
    m_io                    = new PicoBlazeIO();
    m_clockControl          = new PicoBlazeClockControl();
    m_stack                 = new PicoBlazeStack();
    m_interruptController   = new PicoBlazeInterruptController();

    regSubSys ( m_programMemory -> link ( m_eventLogger ) );
    regSubSys ( m_dataMemory -> link ( m_eventLogger ) );
    regSubSys ( m_registers -> link ( m_eventLogger ) );
    regSubSys ( m_io -> link ( m_eventLogger ) );
    regSubSys ( m_instructionSet -> link ( m_eventLogger,
                                           m_io,
                                           m_stack,
                                           m_registers,
                                           m_dataMemory,
                                           m_programMemory,
                                           m_interruptController ) );
    regSubSys ( m_clockControl -> link ( m_eventLogger ) );
    regSubSys ( m_stack -> link ( m_eventLogger ) );
    regSubSys ( m_interruptController -> link ( m_eventLogger,
                                                m_instructionSet ) );

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
    for ( std::vector<MCUSimSubsys*>::iterator i = m_subSystems.begin();
          i != m_subSystems.end();
          i++ )
    {
        delete *i;
    }
}

inline void PicoBlazeSim::checkSubSystems() const
{
    for ( std::vector<MCUSimSubsys*>::const_iterator i = m_subSystems.begin();
          i != m_subSystems.end();
          i++ )
    {
        assert ( MCUSimSubsys::ID_INVALID != (*i)->getId() );
    }
}

inline void PicoBlazeSim::regSubSys ( MCUSimSubsys * subSystem )
{
    m_subSystems.push_back(subSystem);
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

        default:                              return NULL;
    }
}

MCUSimClock::ClockSource & PicoBlazeSim::getClockSource()
{
    return m_clockControl->m_clockSource;
}

void PicoBlazeSim::reset ( ResetMode mode )
{
    for ( std::vector<MCUSimSubsys*>::iterator i = m_subSystems.begin();
          i != m_subSystems.end();
          i++ )
    {
        (*i)->reset(mode);
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
        delete origInstructionSet;
    }
}

inline void PicoBlazeSim::mcuReset()
{
    m_time = 0;
}

int PicoBlazeSim::executeInstruction()
{
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
