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
#include "PIC8InterruptController.h"
#include "PIC8IO.h"
#include "PIC8Stack.h"
#include "PIC8WatchDogTimer.h"
#include "PIC8TimerCounter0.h"
#include "PIC8Timer0WdtPrescaller.h"
#include "PIC8ISP.h"
#include "PIC8ExternalInterrupts.h"

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
    m_interruptController   = new PIC8InterruptController();
    m_dataEEPROM            = new PIC8DataEEPROM();
    m_watchDogTimer         = new PIC8WatchDogTimer();
    m_timerCounter0         = new PIC8TimerCounter0();
    m_timer0WdtPrescaller   = new PIC8Timer0WdtPrescaller();
    m_isp                   = new PIC8ISP();
    m_externalInterrupts    = new PIC8ExternalInterrupts();

    regSubSys(m_configWord->link(m_eventLogger));

    regSubSys(m_programMemory->link(m_eventLogger, m_configWord));

    regSubSys(m_dataMemory->link ( m_eventLogger,
                                   m_externalInterrupts,
                                   m_instructionSet ) );

    regSubSys(m_io->link ( m_eventLogger,
                           m_dataMemory ) );

    regSubSys(m_instructionSet->link ( m_eventLogger,
                                       &m_processorMode,
                                       m_programMemory,
                                       m_dataMemory,
                                       m_configWord,
                                       m_stack,
                                       m_interruptController,
                                       m_watchDogTimer ));

    regSubSys(m_clockControl->link ( m_eventLogger,
                                     m_configWord ) );

    regSubSys(m_stack->link(m_eventLogger));

    regSubSys(m_interruptController->link ( m_eventLogger,
                                            &m_processorMode,
                                            m_dataMemory,
                                            m_instructionSet,
                                            m_stack ));

    regSubSys(m_dataEEPROM->link ( m_eventLogger,
                                   m_dataMemory ) );

    regSubSys(m_watchDogTimer->link ( m_eventLogger,
                                      m_timer0WdtPrescaller,
                                      m_dataMemory,
                                      m_configWord,
                                      m_interruptController ));

    regSubSys(m_timerCounter0->link ( m_eventLogger,
                                      m_dataMemory,
                                      m_io,
                                      m_timer0WdtPrescaller ) );

    regSubSys(m_timer0WdtPrescaller->link ( m_eventLogger,
                                            m_dataMemory ) );

    regSubSys(m_isp->link ( m_eventLogger,
                            m_programMemory,
                            m_dataEEPROM,
                            m_io,
                            m_configWord ) );

    regSubSys(m_externalInterrupts->link ( m_eventLogger,
                                           m_dataMemory,
                                           m_io,
                                           m_interruptController ) );

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
        case Subsys::ID_IO:             return m_io;
        case Subsys::ID_MEM_EEPROM:     return m_dataEEPROM;
        case Subsys::ID_STACK:          return m_stack;
        case Subsys::ID_COUNTER_0:      return m_timerCounter0;
        case Subsys::ID_CLK_CONTROL:    return m_clockControl;
        case Subsys::ID_WATCHDOG:       return m_watchDogTimer;
        case Subsys::ID_INTERRUPTS:     return m_interruptController;
        case Subsys::ID_PRESCALLER:     return m_timer0WdtPrescaller;
        case Subsys::ID_ISP:            return m_isp;
        case Subsys::ID_EXT_INT:        return m_externalInterrupts;

        default:                        return NULL;
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
    if ( true == m_io->m_config.m_enabled )
    {
        m_io->clockCycles();
        if ( false == m_io->getLog(PIC8PinNames::SPF_MCLR) )
        {
            if ( true == m_isp->m_config.m_enabled )
            {
                m_isp->timeStep(cycles2time(1));
            }
            return 1;
        }
    }

    float timeStep = cycles2time(m_clockCycles);

    if ( MD_SLEEP == m_processorMode )
    {
        if ( true == m_externalInterrupts->m_config.m_enabled )
        {
            m_externalInterrupts->clockCycles();
        }

        m_clockCycles = m_interruptController->autoInterrupt();

        if ( -1 == m_clockCycles )
        {
            reset(MCUSim::RSTMD_MCU_RESET);
        }
        else if ( 1 == m_clockCycles )
        {
            m_instructionSet->wakeFromSleep();
        }
        else
        {
            return 1;
        }

        if ( m_clockControl->m_clockSource.getType() != Clock::ClockSource::TYPE_RC )
        {
            return ( 1024 / 2 + 1 );
        }
        else
        {
            return 1;
        }
    }
    else
    {
        if ( true == m_timerCounter0->m_config.m_enabled )
        {
            m_timerCounter0->clockCycles(m_clockCycles);
        }
        if ( true == m_watchDogTimer->m_config.m_enabled )
        {
            m_watchDogTimer->timeStep(timeStep, m_clockCycles);
        }
        if ( true == m_timer0WdtPrescaller->m_config.m_enabled )
        {
            m_timer0WdtPrescaller->clockCycles();
        }
        if ( true == m_externalInterrupts->m_config.m_enabled )
        {
            m_externalInterrupts->clockCycles();
        }

        m_clockCycles = m_interruptController->autoInterrupt();
        if ( -1 == m_clockCycles )
        {
            reset(MCUSim::RSTMD_MCU_RESET);
            return 1;
        }
        else if ( 0 == m_clockCycles )
        {
            m_clockCycles += m_instructionSet->execInstruction();
        }
    }

    return m_clockCycles;
}

int PIC8Sim::timeStep ( float timeStep )
{
    m_time += timeStep;

    if ( true == m_io->m_config.m_enabled )
    {
        if ( false == m_io->getLog(PIC8PinNames::SPF_MCLR) )
        {
            if ( true == m_isp->m_config.m_enabled )
            {
                m_isp->timeStep(m_time);
            }
            return 1;
        }
    }

    if ( m_time < m_clockPeriod )
    {
        if ( true == m_watchDogTimer->m_config.m_enabled )
        {
            m_watchDogTimer->timeStep(timeStep, m_clockCycles);
        }
        return 0;
    }

    int allocatedCycles = int(m_time / m_clockPeriod);
    while ( m_time >= m_clockPeriod )
    {
        m_time -= m_clockPeriod;
    }

    if ( MD_NORMAL == m_processorMode )
    {
        if ( true == m_timerCounter0->m_config.m_enabled )
        {
            m_timerCounter0->clockCycles(allocatedCycles);
        }
        if ( true == m_watchDogTimer->m_config.m_enabled )
        {
            m_watchDogTimer->timeStep(timeStep, allocatedCycles);
        }
        if ( true == m_timer0WdtPrescaller->m_config.m_enabled )
        {
            m_timer0WdtPrescaller->clockCycles();
        }
        if ( true == m_externalInterrupts->m_config.m_enabled )
        {
            m_externalInterrupts->clockCycles();
        }
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

    int cycles = m_interruptController->autoInterrupt();
    if ( 0 != cycles )
    {
        if ( -1 == cycles )
        {
            reset(MCUSim::RSTMD_MCU_RESET);
            return 0;
        }
        else if ( m_processorMode == MD_SLEEP )
        {
            // Wake from sleep
            m_processorMode = MD_NORMAL;
        }
 
        if ( cycles > allocatedCycles )
        {
            m_clockCycles += ( cycles - allocatedCycles );
        }

        if ( m_clockControl->m_clockSource.getType() != Clock::ClockSource::TYPE_RC )
        {
            m_clockCycles += ( 1024 / 2 + 1 );
        }
        else
        {
            m_clockCycles++;
        }

        if ( m_clockCycles > 0 )
        {
            return allocatedCycles;
        }
    }

    if ( MD_NORMAL == m_processorMode )
    {
        cycles = m_instructionSet->execInstruction();

        if ( cycles > allocatedCycles )
        {
            m_clockCycles += ( cycles - allocatedCycles );
        }
    }

    return allocatedCycles;
}
