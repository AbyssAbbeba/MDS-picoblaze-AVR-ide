// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MCUSim
 * @file MCUSim.h
 */
// =============================================================================

#ifndef MCUSIM_H
#define MCUSIM_H

// Forward declarations.
class DataFile;

// Standard headers.
#include <cstddef>

// MCUSim header files.
#include "MCUSimBase.h"
#include "MCUSimSubsys.h"
#include "MCUSimEventLogger.h"
#include "MCUSimPureLogicIO.h"
#include "MCUSimMemory.h"
#include "MCUSimConfig.h"
#include "MCUSimClock.h"
#include "MCUSimCPU.h"
#include "MCUSimIO.h"

/**
 * @brief
 * @ingroup MCUSim
 * @class MCUSim
 */
class MCUSim : public MCUSimBase
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        virtual ~MCUSim() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] mode
         */
        virtual void reset ( ResetMode mode ) = 0;

        /**
         * @brief
         * @param[in] mode
         * @return
         */
        virtual float cycles2time ( int numOfCycles ) = 0;

        /**
         * @brief
         * @return
         */
        virtual int executeInstruction() = 0;

        /**
         * @brief
         * @param[in] time
         * @return
         */
        virtual int timeStep ( float time ) = 0;

        /**
         * @brief
         * @param[in] id
         * @return
         */
        virtual MCUSimSubsys * getSubsys ( MCUSimSubsys::SubsysId id ) = 0;

        /**
         * @brief
         * @return
         */
        virtual MCUSimClock::ClockSource & getClockSource() = 0;

        /**
         * @brief
         * @return
         */
        virtual MCUSimConfig & getConfig() = 0;

        /**
         * @brief
         * @return
         */
        virtual MCUSimEventLogger * getLog() = 0;

        /**
         * @brief
         * @return
         */
        virtual Arch arch() const = 0;

        /**
         * @brief
         * @return
         */
        virtual Family family() const = 0;

        /**
         * @brief
         * @return
         */
        virtual const char * name() const = 0;

        /**
         * @brief
         * @return
         */
        virtual Mode mode() const = 0;
};

#endif // MCUSIM_H
