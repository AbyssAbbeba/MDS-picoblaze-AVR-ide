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
#include "PicoBlazeProgramMemory.h"
#include "PicoBlazeDataMemory.h"
#include "PicoBlazeRegisters.h"
#include "PicoBlazeInterruptController.h"
#include "PicoBlazeIO.h"
#include "PicoBlazeStack.h"

#include <cassert>

PicoBlazeSim::PicoBlazeSim()
{
    m_eventLogger           = new EventLogger();
    m_config                = new PicoBlazeConfig();

    m_programMemory         = new PicoBlazeProgramMemory();
    m_dataMemory            = new PicoBlazeDataMemory();
    m_registers             = new PicoBlazeRegisters();
    m_instructionSet        = new PicoBlazeInstructionSet();
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
    for ( std::vector<Subsys*>::iterator i = m_subSystems.begin();
          i != m_subSystems.end();
          i++ )
    {
        delete *i;
    }
}

inline void PicoBlazeSim::checkSubSystems() const
{
    for ( std::vector<Subsys*>::const_iterator i = m_subSystems.begin();
          i != m_subSystems.end();
          i++ )
    {
        assert ( Subsys::ID_INVALID != (*i)->getId() );
    }
}

inline void PicoBlazeSim::regSubSys ( Subsys * subSystem )
{
    m_subSystems.push_back(subSystem);
}

MCUSim::Subsys * PicoBlazeSim::getSubsys ( Subsys::SubsysId id )
{
    switch ( id )
    {
        case Subsys::ID_MEM_CODE:       return m_programMemory;
        case Subsys::ID_MEM_DATA:       return m_dataMemory;
        case Subsys::ID_MEM_REGISTERS:  return m_registers;
        case Subsys::ID_CPU:            return m_instructionSet;
        case Subsys::ID_IO:             return m_io;
        case Subsys::ID_STACK:          return m_stack;
        case Subsys::ID_CLK_CONTROL:    return m_clockControl;
        case Subsys::ID_INTERRUPTS:     return m_interruptController;

        default:                        return NULL;
    }
}

MCUSim::Clock::ClockSource & PicoBlazeSim::getClockSource()
{
    return m_clockControl->m_clockSource;
}

void PicoBlazeSim::reset ( ResetMode mode )
{
    for ( std::vector<Subsys*>::iterator i = m_subSystems.begin();
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

MCUSim::Config & PicoBlazeSim::getConfig()
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
