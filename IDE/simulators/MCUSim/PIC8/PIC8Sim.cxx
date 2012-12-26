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
 * @file PIC8Sim.cxx
 */
// =============================================================================

#include "PIC8Sim.h"

#include "PIC8Config.h"
#include "PIC8InstructionSet.h"
#include "PIC8ProgramMemory.h"
#include "PIC8DataMemory.h"
#include "PIC8ConfigWord.h"
#include "PIC8DataEEPROM.h"
#include "PIC8InterruptCtrl.h"
#include "PIC8IO.h"
#include "PIC8Stack.h"
#include "PIC8WatchDogTimer.h"
#include "PIC8Timer0.h"

#include <cassert>

PIC8Sim::PIC8Sim()
{
    m_processorMode = MD_NORMAL;

    m_eventLogger           = new EventLogger();
    m_config                = new PIC8Config();

    m_configWord            = new PIC8ConfigWord();
    m_programMemory         = new PIC8ProgramMemory();
    m_dataMemory            = new PIC8DataMemory();
    m_instructionSet        = new PIC8InstructionSet();
    m_io                    = new PIC8IO();
    m_clockControl          = new PIC8ClockControl();
    m_stack                 = new PIC8Stack();
    m_interruptCtrl         = new PIC8InterruptCtrl();
    m_dataEEPROM            = new PIC8DataEEPROM();
    m_watchDogTimer         = new PIC8WatchDogTimer();
    m_timer0                = new PIC8Timer0();

    regSubSys(m_configWord->link(m_eventLogger));

    regSubSys(m_programMemory->link(m_eventLogger));

    regSubSys(m_dataMemory->link(m_eventLogger));

    regSubSys(m_io->link(m_eventLogger, m_dataMemory));

    regSubSys(m_instructionSet->link ( m_eventLogger,
                                       &m_processorMode,
                                       m_programMemory,
                                       m_dataMemory,
                                       m_configWord,
                                       m_stack,
                                       m_interruptCtrl,
                                       m_watchDogTimer ));

    regSubSys(m_clockControl->link(m_eventLogger, m_configWord));

    regSubSys(m_stack->link(m_eventLogger));

    regSubSys(m_interruptCtrl->link(m_eventLogger));
    regSubSys(m_dataEEPROM->link(m_eventLogger));
    regSubSys(m_watchDogTimer->link(m_eventLogger));
    regSubSys(m_timer0->link(m_eventLogger, m_dataMemory, m_io));

    checkSubSystems();
    m_config->link(this);
}

PIC8Sim::~PIC8Sim()
{
    deleteSubSystems();
    delete m_config;
    delete m_eventLogger;
}

inline void PIC8Sim::deleteSubSystems()
{
    for ( std::vector<Subsys*>::iterator i = m_subSystems.begin();
          i != m_subSystems.end();
          i++ )
    {
        delete *i;
    }
}

inline void PIC8Sim::checkSubSystems() const
{
    for ( std::vector<Subsys*>::const_iterator i = m_subSystems.begin();
          i != m_subSystems.end();
          i++ )
    {
        assert ( Subsys::ID_INVALID != (*i)->getId() );
    }
}

inline void PIC8Sim::regSubSys ( Subsys * subSystem )
{
    m_subSystems.push_back(subSystem);
}

MCUSim::Subsys * PIC8Sim::getSubsys ( Subsys::SubsysId id )
{
    switch ( id )
    {
        case Subsys::ID_MEM_CODE:       return m_programMemory;
        case Subsys::ID_MEM_DATA:       return m_dataMemory;

        case Subsys::ID_CPU:            return m_instructionSet;
        case Subsys::ID_FUSES:          return m_configWord;

        default:
            return NULL;
    }
}

MCUSim::Clock::ClockSource & PIC8Sim::getClockSource()
{
    return m_clockControl->m_clockSource;
}

void PIC8Sim::reset ( ResetMode mode )
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

MCUSim::Config & PIC8Sim::getConfig()
{
    return * m_config;
}

MCUSim::Family PIC8Sim::family() const
{
    return m_config->m_family;
}

const char * PIC8Sim::name() const
{
    return m_config->m_name.c_str();
}

MCUSim::Arch PIC8Sim::arch() const
{
    return m_config->getArch();
}

inline void PIC8Sim::resetToInitialValues()
{
    m_clockPeriod = ( 1.0 / m_clockControl->getFrequency() );
}

inline void PIC8Sim::loadConfig()
{
}

inline void PIC8Sim::mcuReset()
{
    m_clockCycles = 0;
    m_time = 0;
}

int PIC8Sim::executeInstruction()
{
    m_io->clockCycles();
    return m_instructionSet->execInstruction();
}

int PIC8Sim::timeStep ( float timeStep )
{
    m_time += timeStep;

    int allocatedCycles = int(m_time / m_clockPeriod);
    while ( m_time >= m_clockPeriod )
    {
        m_time -= m_clockPeriod;
    }

    if ( m_clockCycles > 0 )
    {
        m_clockCycles -= allocatedCycles;

        if ( m_clockCycles < 0 )
        {
            allocatedCycles = -m_clockCycles;
        }
        else
        {
            return 0;
        }
    }

    m_io->clockCycles();
    int cycles = m_instructionSet->execInstruction();

    if ( cycles > allocatedCycles )
    {
        m_clockCycles += ( cycles - allocatedCycles );
    }

    return allocatedCycles;
}
