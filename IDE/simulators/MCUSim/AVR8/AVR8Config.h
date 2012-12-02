// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @authors Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup AVR8
 * @file AVR8Config.h
 */
// =============================================================================

#ifndef AVR8CONFIG_H
#define AVR8CONFIG_H

#include "../MCUSim.h"
#include "AVR8Sim.h"
#include "AVR8BootLoader.h"
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

#include <string>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8Config
 */
class AVR8Config : public MCUSim::Config
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AVR8Config();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] system
         */
        void link ( AVR8Sim * system );

        /**
         * @brief
         * @return
         */
        bool linked() const
        {
            return isLinked;
        }

    ////    Public Attributes    ////
    public:
        AVR8BootLoader::Config          * m_configBootLoader;
        AVR8InstructionSet::Config      * m_configInstructionSet;
        AVR8ProgramMemory::Config       * m_configProgramMemory;
        AVR8DataMemory::Config          * m_configDataMemory;
        AVR8FusesAndLocks::Config       * m_configFusesAndLocks;
        AVR8InterruptController::Config * m_configInterruptController;
        AVR8TimerCounter0::Config       * m_configTimerCounter0;
        AVR8TimerCounter1::Config       * m_configTimerCounter1;
        AVR8TimerCounter2::Config       * m_configTimerCounter2;
        AVR8IO::Config                  * m_configIO;
        AVR8ExternalInterrupts::Config  * m_configExternalInterrupts;
        AVR8DataEEPROM::Config          * m_configDataEEPROM;
        AVR8WatchdogTimer::Config       * m_configWatchdogTimer;
        AVR8SystemControl::Config       * m_configSystemControl;
        AVR8ClockControl::Config        * m_configClockControl;
        AVR8Spi::Config                 * m_configSpi;
        AVR8Usart::Config               * m_configUsart;
        AVR8Twi::Config                 * m_configTwi;
        AVR8Adc::Config                 * m_configAdc;
        AVR8AnalogComparator::Config    * m_configAcomp;
        AVR8Isp::Config                 * m_configIsp;
        AVR8ParallelProg::Config        * m_configPprog;

        /**
         * @brief
         */
        MCUSim::Family m_family;

        /**
         * @brief
         */
        std::string m_name;

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        bool isLinked;
};

#endif // AVR8CONFIG_H
