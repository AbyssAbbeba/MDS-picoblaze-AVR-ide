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
 * @file MCUSimSubsys.h
 */
// =============================================================================

#ifndef MCUSIMSUBSYS_H
#define MCUSIMSUBSYS_H

#include "MCUSimBase.h"
#include "MCUSimEventLogger.h"

/**
 * @brief
 * @ingroup MCUSim
 * @class MCUSimSubsys
 */
class MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum SubsysId
        {
            ID_INVALID = 0,  ///<

            ID_MEM_CODE,     ///<
            ID_MEM_DATA,     ///<
            ID_MEM_EEPROM,   ///<
            ID_MEM_REGISTERS,///<
            ID_STACK,        ///<

            ID_CPU,          ///<
            ID_FUSES,        ///<
            ID_INTERRUPTS,   ///<
            ID_EXT_INT,      ///<
            ID_WATCHDOG,     ///<
            ID_PRESCALLER,   ///<
            ID_IO,           ///<
            ID_PLIO,         ///< Pure Logic Input/Output
            ID_BOOT_LOADER,  ///<
            ID_SYS_CONTROL,  ///<
            ID_CLK_CONTROL,  ///<
            ID_COUNTER_0,    ///<
            ID_COUNTER_1,    ///<
            ID_COUNTER_2,    ///<
            ID_SPI,          ///<
            ID_USART,        ///<
            ID_TWI,          ///<
            ID_ADC,          ///<
            ID_ACOMP,        ///<
            ID_ISP,          ///<
            ID_PPROG,        ///<

            ID__MAX__        ///<
        };

    ////    Constructors and Destructors    ////
    protected:
        /**
         * @brief
         */
        MCUSimSubsys() : m_id(ID_INVALID) {};

        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in] id
         */
        MCUSimSubsys ( MCUSimEventLogger * eventLogger,
                       SubsysId id )
        {
            link(eventLogger, id);
        };

    public:
        /**
         * @brief
         */
        virtual ~MCUSimSubsys() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] mode
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) = 0;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        SubsysId getId() const
        {
            return m_id;
        }

    ////    Inline Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in] id
         */
        void link ( MCUSimEventLogger * eventLogger,
                    SubsysId id )
        {
            m_eventLogger = eventLogger;
            m_id = id;
        }

        /**
         * @brief
         * @param[in] eventId
         * @param[in] eventLocation
         * @param[in] eventDetail
         */
        void logEvent ( int eventId,
                        int eventLocation = 0,
                        int eventDetail = 0 )
        {
            m_eventLogger->logEvent(m_id, eventId, eventLocation, eventDetail);
        }

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        MCUSimEventLogger * m_eventLogger;

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        SubsysId m_id;
};

#endif // MCUSIMSUBSYS_H
