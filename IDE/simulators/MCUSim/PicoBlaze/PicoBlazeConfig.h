// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2013
 * @ingroup PicoBlaze
 * @file PicoBlazeConfig.h
 */
// =============================================================================

#ifndef PICOBLAZECONFIG_H
#define PICOBLAZECONFIG_H

// Forward declarations
class PicoBlazeSim;

#include "../MCUSim.h"
#include "PicoBlazeClockControl.h"
#include "PicoBlazeDataMemory.h"
#include "PicoBlazeRegisters.h"
#include "PicoBlazeInterruptController.h"
#include "PicoBlazeProgramMemory.h"
#include "PicoBlazeStack.h"
#include "PicoBlazeInstructionSet.h"
#include "PicoBlazeIO.h"

#include <string>

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeConfig
 */
class PicoBlazeConfig : public MCUSimConfig
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PicoBlazeConfig();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] system
         */
        void link ( PicoBlazeSim * system );

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

        /// @name Configuration containers for PicoBlaze simulator subsystems.
        //@{
            ///
            PicoBlazeClockControl::Config               * m_configClockControl;

            ///
            PicoBlazeDataMemory::Config                 * m_configDataMemory;

            ///
            PicoBlazeRegisters::Config                  * m_configRegisters;

            ///
            PicoBlazeInterruptController::Config        * m_configInterruptController;

            ///
            PicoBlazeProgramMemory::Config              * m_configProgramMemory;

            ///
            PicoBlazeStack::Config                      * m_configStack;

            ///
            PicoBlazeInstructionSet::Config             * m_configInstructionSet;

            ///
            PicoBlazeIO::Config                         * m_configIO;
        //@}

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        bool m_isLinked;
};

#endif // PICOBLAZECONFIG_H
