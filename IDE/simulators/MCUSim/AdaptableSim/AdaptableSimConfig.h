// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdaptableSim
 * @file AdaptableSimConfig.h
 */
// =============================================================================

#ifndef ADAPTABLESIMCONFIG_H
#define ADAPTABLESIMCONFIG_H

// Forward declarations.
class AdaptableSimSim;

#include "../MCUSim.h"

#include "AdaptableSimIO.h"
#include "AdaptableSimStack.h"
#include "AdaptableSimRegisters.h"
#include "AdaptableSimDataMemory.h"
#include "AdaptableSimStatusFlags.h"
#include "AdaptableSimClockControl.h"
#include "AdaptableSimProgramMemory.h"
#include "AdaptableSimInstructionSet.h"
#include "AdaptableSimInterruptController.h"

#include <string>

/**
 * @brief
 * @ingroup AdaptableSim
 * @class AdaptableSimConfig
 */
class AdaptableSimConfig : public MCUSimConfig
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AdaptableSimConfig();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] system
         */
        void link ( AdaptableSimSim * system );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        bool linked() const
        {
            return m_isLinked;
        }

        /**
         * @brief
         * @param[in] dev
         */
        void configure ( MCUSim::Family dev );

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

        /// @name Configuration containers for AdaptableSim simulator subsystems.
        //@{
            ///
            AdaptableSimIO::Config                         * m_configIO;

            ///
            AdaptableSimStack::Config                      * m_configStack;

            ///
            AdaptableSimRegisters::Config                  * m_configRegisters;

            ///
            AdaptableSimDataMemory::Config                 * m_configDataMemory;

            ///
            AdaptableSimStatusFlags::Config                * m_configStatusFlags;

            ///
            AdaptableSimClockControl::Config               * m_configClockControl;

            ///
            AdaptableSimProgramMemory::Config              * m_configProgramMemory;

            ///
            AdaptableSimInstructionSet::Config             * m_configInstructionSet;

            ///
            AdaptableSimInterruptController::Config        * m_configInterruptController;
        //@}

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        bool m_isLinked;
};

#endif // ADAPTABLESIMCONFIG_H
