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
 * @ingroup AdaptableSim
 * @file AdaptableSimSim.cxx
 */
// =============================================================================

#include "AdaptableSimSim.h"

#include "AdaptableSimIO.h"
#include "AdaptableSimStack.h"
#include "AdaptableSimConfig.h"
#include "AdaptableSimRegisters.h"
#include "AdaptableSimDataMemory.h"
#include "AdaptableSimProgramMemory.h"
#include "AdaptableSimInstructionSet.h"
#include "AdaptableSimInterruptController.h"

#include <cassert>

AdaptableSimSim::AdaptableSimSim()
{
    m_config                = new AdaptableSimConfig();
    m_eventLogger           = new MCUSimEventLogger();

    m_io                    = new AdaptableSimIO();
    m_stack                 = new AdaptableSimStack();
    m_registers             = new AdaptableSimRegisters();
    m_dataMemory            = new AdaptableSimDataMemory();
    m_statusFlags           = new AdaptableSimStatusFlags();
    m_clockControl          = new AdaptableSimClockControl();
    m_programMemory         = new AdaptableSimProgramMemory();
    m_instructionSet        = new AdaptableSimInstructionSet();
    m_interruptController   = new AdaptableSimInterruptController();

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

AdaptableSimSim::~AdaptableSimSim()
{
    deleteSubSystems();

    delete m_config;
    delete m_eventLogger;
}

inline void AdaptableSimSim::deleteSubSystems()
{
    for ( auto subsys : m_subSystems )
    {
        delete subsys;
    }
}

inline void AdaptableSimSim::checkSubSystems() const
{
    #ifndef NDEBUG
        for ( const auto subsys : m_subSystems )
        {
            assert ( MCUSimSubsys::ID_INVALID != subsys->getId() );
        }
    #endif // NDEBUG
}

inline void AdaptableSimSim::regSubSys ( MCUSimSubsys * subSystem )
{
    m_subSystems.push_back(subSystem);
}

inline void AdaptableSimSim::unregSubSys ( const MCUSimSubsys * subSystem )
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

MCUSimSubsys * AdaptableSimSim::getSubsys ( MCUSimSubsys::SubsysId id )
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

MCUSimClock::ClockSource & AdaptableSimSim::getClockSource()
{
    return m_clockControl->m_clockSource;
}

void AdaptableSimSim::reset ( ResetMode mode )
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

MCUSimConfig & AdaptableSimSim::getConfig()
{
    return * m_config;
}

MCUSim::Family AdaptableSimSim::family() const
{
    return m_config->m_family;
}

const char * AdaptableSimSim::name() const
{
    return m_config->m_name.c_str();
}

MCUSim::Arch AdaptableSimSim::arch() const
{
    return m_config->getArch();
}

inline void AdaptableSimSim::resetToInitialValues()
{
    m_clockPeriod = ( 1.0 / m_clockControl->getFrequency() );
}

inline void AdaptableSimSim::loadConfig()
{
}

inline void AdaptableSimSim::mcuReset()
{
    m_time = 0;
}

int AdaptableSimSim::executeInstruction()
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

int AdaptableSimSim::timeStep ( float timeStep )
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
