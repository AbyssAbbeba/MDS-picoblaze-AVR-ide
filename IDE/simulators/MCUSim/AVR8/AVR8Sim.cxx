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
 * @ingroup AVR8
 * @file AVR8Sim.cxx
 */
// =============================================================================

#include "AVR8Sim.h"

#include "AVR8Config.h"
#include "AVR8InstructionSet.h"
#include "AVR8ProgramMemory.h"
#include "AVR8DataMemory.h"
#include "AVR8FusesAndLocks.h"
#include "AVR8InterruptController.h"
#include "AVR8TimerCounter0.h"
#include "AVR8TimerCounter1.h"
#include "AVR8TimerCounter2.h"
#include "AVR8IO.h"
#include "AVR8ExternalInterrupts.h"
#include "AVR8DataEEPROM.h"
#include "AVR8BootLoader.h"
#include "AVR8WatchdogTimer.h"
#include "AVR8SystemControl.h"
#include "AVR8ClockControl.h"
#include "AVR8Spi.h"
#include "AVR8Usart.h"
#include "AVR8Twi.h"
#include "AVR8Adc.h"
#include "AVR8AnalogComparator.h"
#include "AVR8Isp.h"
#include "AVR8ParallelProg.h"

#include <cassert>

AVR8Sim::AVR8Sim()
{
    m_sleepMode = SLEEPMD_NONE;
    m_processorMode = MD_NORMAL;
    m_haltMode = HALTM_NONE;

    m_eventLogger           = new MCUSimEventLogger();
    m_config                = new AVR8Config();

    m_interrupts            = new AVR8InterruptController();
    m_fusesAndLocks         = new AVR8FusesAndLocks();
    m_programMemory         = new AVR8ProgramMemory();
    m_dataMemory            = new AVR8DataMemory();
    m_dataEEPROM            = new AVR8DataEEPROM();
    m_bootLoader            = new AVR8BootLoader();
    m_instructionSet        = new AVR8InstructionSet();
    m_timerCounter0         = new AVR8TimerCounter0();
    m_timerCounter1         = new AVR8TimerCounter1();
    m_timerCounter2         = new AVR8TimerCounter2();
    m_io                    = new AVR8IO();
    m_externalInterrupts    = new AVR8ExternalInterrupts();
    m_watchdogTimer         = new AVR8WatchdogTimer();
    m_systemControl         = new AVR8SystemControl();
    m_clockControl          = new AVR8ClockControl();
    m_spi                   = new AVR8Spi();
    m_usart                 = new AVR8Usart();
    m_twi                   = new AVR8Twi();
    m_adc                   = new AVR8Adc();
    m_acomp                 = new AVR8AnalogComparator();
    m_isp                   = new AVR8Isp();
    m_pprog                 = new AVR8ParallelProg();

    regSubSys ( m_interrupts->link ( m_eventLogger,
                                     m_instructionSet,
                                     m_programMemory,
                                     m_dataMemory,
                                     m_fusesAndLocks,
                                     &m_sleepMode,
                                     m_bootLoader ) );

    regSubSys ( m_fusesAndLocks->link(m_eventLogger) );

    regSubSys ( m_programMemory->link(m_eventLogger, m_bootLoader) );

    regSubSys ( m_dataMemory->link(m_eventLogger) );

    regSubSys ( m_dataEEPROM->link(m_eventLogger, m_dataMemory, m_interrupts) );

    regSubSys ( m_bootLoader->link ( m_eventLogger,
                                     m_programMemory,
                                     m_dataMemory,
                                     m_fusesAndLocks,
                                     m_dataEEPROM,
                                     m_instructionSet,
                                     &m_haltMode ) );

    regSubSys ( m_instructionSet->link ( m_eventLogger,
                                         m_programMemory,
                                         m_dataMemory,
                                         &m_processorMode,
                                         &m_sleepMode,
                                         m_fusesAndLocks,
                                         m_interrupts,
                                         m_systemControl,
                                         &m_haltMode,
                                         m_bootLoader ) );

    regSubSys ( m_timerCounter0->link ( m_eventLogger,
                                        MCUSimSubsys::ID_COUNTER_0,
                                        m_dataMemory,
                                        m_io ) );

    regSubSys ( m_timerCounter1->link ( m_eventLogger,
                                        MCUSimSubsys::ID_COUNTER_1,
                                        m_dataMemory ) );

    regSubSys ( m_timerCounter2->link ( m_eventLogger,
                                        MCUSimSubsys::ID_COUNTER_2,
                                        m_dataMemory ) );

    regSubSys ( m_io->link ( m_eventLogger,
                             m_dataMemory ) );

    regSubSys ( m_externalInterrupts->link ( m_eventLogger,
                                             m_dataMemory,
                                             m_io ) );

    regSubSys ( m_watchdogTimer->link ( m_eventLogger,
                                        m_dataMemory,
                                        m_interrupts,
                                        m_fusesAndLocks ) );

    regSubSys ( m_systemControl->link ( m_eventLogger,
                                        m_dataMemory,
                                        m_interrupts,
                                        m_watchdogTimer,
                                        m_io,
                                        m_fusesAndLocks ) );

    regSubSys ( m_clockControl->link ( m_eventLogger,
                                       m_fusesAndLocks ) );

    regSubSys ( m_spi->link ( m_eventLogger,
                              m_dataMemory ) );

    regSubSys ( m_usart->link ( m_eventLogger,
                                m_dataMemory ) );

    regSubSys ( m_twi->link ( m_eventLogger,
                              m_dataMemory ) );

    regSubSys ( m_adc->link ( m_eventLogger,
                              m_dataMemory ) );

    regSubSys ( m_acomp->link ( m_eventLogger,
                                m_dataMemory ) );

    regSubSys ( m_isp->link ( m_eventLogger,
                              m_programMemory ) );

    regSubSys ( m_pprog->link ( m_eventLogger,
                                m_programMemory ) );

    checkSubSystems();
    m_config->link(this);
}

AVR8Sim::~AVR8Sim()
{
    deleteSubSystems();
    delete m_config;
    delete m_eventLogger;
}

inline void AVR8Sim::deleteSubSystems()
{
    for ( std::vector<MCUSimSubsys*>::iterator i = m_subSystems.begin();
          i != m_subSystems.end();
          i++ )
    {
        delete *i;
    }
}

inline void AVR8Sim::checkSubSystems() const
{
    for ( std::vector<MCUSimSubsys*>::const_iterator i = m_subSystems.begin();
          i != m_subSystems.end();
          i++ )
    {
        assert ( MCUSimSubsys::ID_INVALID != (*i)->getId() );
    }
}

inline void AVR8Sim::regSubSys ( MCUSimSubsys * subSystem )
{
    m_subSystems.push_back(subSystem);
}

MCUSimSubsys * AVR8Sim::getSubsys ( MCUSimSubsys::SubsysId id )
{
    switch ( id )
    {
        case MCUSimSubsys::ID_MEM_CODE:       return m_programMemory;
        case MCUSimSubsys::ID_MEM_DATA:       return m_dataMemory;
        case MCUSimSubsys::ID_MEM_EEPROM:     return m_dataEEPROM;

        case MCUSimSubsys::ID_CPU:            return m_instructionSet;
        case MCUSimSubsys::ID_FUSES:          return m_fusesAndLocks;
        case MCUSimSubsys::ID_INTERRUPTS:     return m_interrupts;
        case MCUSimSubsys::ID_EXT_INT:        return m_externalInterrupts;
        case MCUSimSubsys::ID_WATCHDOG:       return m_watchdogTimer;
        case MCUSimSubsys::ID_IO:             return m_io;
        case MCUSimSubsys::ID_BOOT_LOADER:    return m_bootLoader;
        case MCUSimSubsys::ID_SYS_CONTROL:    return m_systemControl;
        case MCUSimSubsys::ID_CLK_CONTROL:    return m_clockControl;
        case MCUSimSubsys::ID_COUNTER_0:      return m_timerCounter0;
        case MCUSimSubsys::ID_SPI:            return m_spi;
        case MCUSimSubsys::ID_USART:          return m_usart;
        case MCUSimSubsys::ID_TWI:            return m_twi;
        case MCUSimSubsys::ID_ADC:            return m_adc;
        case MCUSimSubsys::ID_ACOMP:          return m_acomp;
        case MCUSimSubsys::ID_ISP:            return m_isp;
        case MCUSimSubsys::ID_PPROG:          return m_pprog;

        default:
            return NULL;
    }
}

MCUSimClock::ClockSource & AVR8Sim::getClockSource()
{
    return m_clockControl->m_clockSource;
}

void AVR8Sim::reset ( ResetMode mode )
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

MCUSimConfig & AVR8Sim::getConfig()
{
    return * m_config;
}

MCUSim::Family AVR8Sim::family() const
{
    return m_config->m_family;
}

const char * AVR8Sim::name() const
{
    return m_config->m_name.c_str();
}

MCUSim::Arch AVR8Sim::arch() const
{
    return m_config->getArch();
}

inline void AVR8Sim::resetToInitialValues()
{
    m_clockPeriod = ( 1.0 / m_clockControl->getFrequency() );
}

inline void AVR8Sim::loadConfig()
{
}

inline void AVR8Sim::mcuReset()
{
    m_clockCycles = 0;
    m_time = 0;
}

int AVR8Sim::executeInstruction()
{
    float timeStep = cycles2time(m_clockCycles);

    m_io->clockCycles();
    if ( true == m_externalInterrupts->enabled() )
    {
        m_externalInterrupts->clockCycle();
    }
    if ( true == m_timerCounter0->enabled() )
    {
        m_timerCounter0->clockCycles(m_clockCycles);
    }
    if ( true == m_bootLoader->enabled() )
    {
        m_bootLoader->timeStep(timeStep, m_clockCycles);
    }
    if ( true == m_bootLoader->enabled() )
    {
        m_dataEEPROM->timeStep(timeStep, m_clockCycles);
    }
    m_systemControl->timeStep(timeStep, m_clockCycles);

// TODO:
// - m_timerCounter1
// - m_timerCounter2
// - m_spi
// - m_usart
// - m_twi
// - m_adc
// - m_acomp
// - m_isp
// - m_pprog

    m_clockCycles = m_interrupts->autoInterrupt();

    if ( 0 != m_clockCycles )
    {
        if ( -1 == m_clockCycles )
        {
            m_clockCycles = 0;
            reset(MCUSim::RSTMD_MCU_RESET);
            return 0;
        }
        else if ( m_sleepMode != SLEEPMD_NONE )
        {
            // Wake from sleep
            m_sleepMode = SLEEPMD_NONE;
            m_processorMode = MD_NORMAL;
        }
    }

    if ( MD_NORMAL == m_processorMode )
    {
        m_clockCycles += m_instructionSet->execInstruction();
    }

    return m_clockCycles;
}

int AVR8Sim::timeStep(float timeStep)
{
    m_time += timeStep;

    if ( m_time < m_clockPeriod )
    {
        m_bootLoader->timeStep(timeStep);
        m_dataEEPROM->timeStep(timeStep);
        m_watchdogTimer->timeStep(timeStep);
        m_systemControl->timeStep(timeStep);

        return 0;
    }

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
    m_externalInterrupts->clockCycle();
    m_timerCounter0->clockCycles(allocatedCycles);
    m_bootLoader->timeStep(timeStep, allocatedCycles);
    m_dataEEPROM->timeStep(timeStep, allocatedCycles);
    m_watchdogTimer->timeStep(timeStep, allocatedCycles);
    m_systemControl->timeStep(timeStep, allocatedCycles);

    int cycles = m_interrupts->autoInterrupt();
    if ( 0 != cycles )
    {
        if ( -1 == cycles )
        {
            reset(MCUSim::RSTMD_MCU_RESET);
            return 0;
        }
        else if ( m_sleepMode != SLEEPMD_NONE )
        {
            // Wake from sleep
            m_sleepMode = SLEEPMD_NONE;
            m_processorMode = MD_NORMAL;
        }

        if ( cycles > allocatedCycles )
        {
            m_clockCycles += ( cycles - allocatedCycles );
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
