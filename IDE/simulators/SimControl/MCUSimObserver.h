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
 * @ingroup SimControl
 * @file MCUSimObserver.h
 */
// =============================================================================

#ifndef MCUSIMOBSERVER_H
#define MCUSIMOBSERVER_H

// Forward declarations
class MCUSimControl;

/**
 * @class MCUSimObserver
 * @ingroup SimControl
 * @brief
 */
class MCUSimObserver
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        MCUSimObserver();

        /**
         * @brief
         */
        virtual ~MCUSimObserver();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] managedBy
         */
        void setControlUnit ( MCUSimControl * managedBy );

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
                                   int detail ) = 0;

        /**
         * @brief
         */
        virtual void deviceChanged() = 0;

        /**
         * @brief
         */
        virtual void deviceReset() = 0;

        /**
         * @brief
         * @param[in] readOnly
         */
        virtual void setReadOnly ( bool readOnly ) = 0;

    ////    Protected Attributes    ////
    protected:
        ///
        MCUSimControl * m_simControlUnit;
};

#endif // MCUSIMOBSERVER_H
