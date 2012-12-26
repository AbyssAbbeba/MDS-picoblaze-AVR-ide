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
 * @ingroup PIC8
 * @file PIC8Config.h
 */
// =============================================================================

#ifndef PIC8CONFIG_H
#define PIC8CONFIG_H

#include "../MCUSim.h"
#include "PIC8Sim.h"
#include "PIC8InstructionSet.h"
#include "PIC8ProgramMemory.h"
#include "PIC8DataMemory.h"
#include "PIC8ConfigWord.h"
#include "PIC8IO.h"
#include "PIC8Stack.h"
#include "PIC8ClockControl.h"
#include "PIC8InterruptCtrl.h"
#include "PIC8DataEEPROM.h"
#include "PIC8WatchDogTimer.h"
#include "PIC8Timer0.h"

#include <string>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8Config
 */
class PIC8Config : public MCUSim::Config
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8Config();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] system
         */
        void link ( PIC8Sim * system );

    ////    Inline Public Operations    ////
    public:
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
        /**
         * @brief
         */
        MCUSim::Family m_family;

        /**
         * @brief
         */
        std::string m_name;

        /// @name Configuration containers for PIC8 simulator subsystems.
        //@{
            ///
            PIC8InstructionSet::Config      * m_configInstructionSet;

            ///
            PIC8ProgramMemory::Config       * m_configProgramMemory;

            ///
            PIC8DataMemory::Config          * m_configDataMemory;

            ///
            PIC8ConfigWord::Config          * m_configConfigWord;

            ///
            PIC8IO::Config                  * m_configIO;

            ///
            PIC8ClockControl::Config        * m_configClockControl;

            ///
            PIC8Stack::Config               * m_configStack;

            ///
            PIC8ConfigWord::Config          * m_configWord;

            ///
            PIC8InterruptCtrl::Config       * m_configInterruptCtrl;

            ///
            PIC8DataEEPROM::Config          * m_configDataEEPROM;

            ///
            PIC8WatchDogTimer::Config       * m_configWatchDogTimer;

            ///
            PIC8Timer0::Config              * m_configTimer0;
        //@}

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        bool isLinked;
};

#endif // PIC8CONFIG_H
