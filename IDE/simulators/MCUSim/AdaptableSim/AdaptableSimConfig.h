// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2013
 * @ingroup AdaptableSim
 * @file AdaptableSimConfig.h
 */
// =============================================================================

#ifndef AdaptableSimCONFIG_H
#define AdaptableSimCONFIG_H

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
            AdaptableSimClockControl::Config               * m_configClockControl;

            ///
            AdaptableSimDataMemory::Config                 * m_configDataMemory;

            ///
            AdaptableSimRegisters::Config                  * m_configRegisters;

            ///
            AdaptableSimInterruptController::Config        * m_configInterruptController;

            ///
            AdaptableSimProgramMemory::Config              * m_configProgramMemory;

            ///
            AdaptableSimStack::Config                      * m_configStack;

            ///
            AdaptableSimInstructionSet::Config             * m_configInstructionSet;

            ///
            AdaptableSimIO::Config                         * m_configIO;

            ///
            AdaptableSimStatusFlags::Config                * m_configStatusFlags;
        //@}

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        bool m_isLinked;
};

#endif // AdaptableSimCONFIG_H
