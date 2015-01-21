// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup SimCmdLine
 * @file SimCmdLineObserver.h
 */
// =============================================================================

#ifndef SIMCMDLINEOBSERVER_H
#define SIMCMDLINEOBSERVER_H

// Simulator observer base class.
#include "../SimControl/MCUSimObserver.h"

/**
 * @brief
 * @ingroup SimCmdLine
 * @class SimCmdLineObserver
 */
class SimCmdLineObserver : public MCUSimObserver
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        SimCmdLineObserver() : m_trackTime ( false ) {}

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] subsysId
         * @param[in] eventId
         * @param[in] locationOrReason
         * @param[in] detail
         */
        virtual void handleEvent ( int subsysId,
                                   int eventId,
                                   int locationOrReason,
                                   int detail ) override;

        /**
         * @brief
         */
        virtual void deviceChanged() override {};

        /**
         * @brief
         */
        virtual void deviceReset() override {};

        /**
         * @brief
         */
        virtual void setReadOnly ( bool ) override {};

        /**
         * @brief
         */
        void setTrackTime ( bool trackTime )
        {
            m_trackTime = trackTime;
        }

    ////    Private Attributes    ////
    private:
        bool m_trackTime;
};

#endif // SIMCMDLINEOBSERVER_H
