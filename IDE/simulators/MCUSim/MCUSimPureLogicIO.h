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
 * @ingroup MCUSim
 * @file MCUSimPureLogicIO.h
 */
// =============================================================================

#ifndef MCUSIMPURELOGICIO_H
#define MCUSIMPURELOGICIO_H

#include "MCUSimSubsys.h"

/**
 * @brief
 * @ingroup MCUSim
 * @class MCUSimPureLogicIO
 */
class MCUSimPureLogicIO : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_PLIO_READ,      ///<
            EVENT_PLIO_READ_END,  ///<

            EVENT_PLIO_WRITE,     ///<
            EVENT_PLIO_WRITE_END, ///<

            EVENT_PLIO__MAX__     ///<
        };

    ////    Constructors and Destructors    ////
    protected:
        /**
         * @brief
         */
        MCUSimPureLogicIO() {};

        /**
         * @brief
         * @param[in,out] eventLogger
         */
        MCUSimPureLogicIO ( MCUSimEventLogger * eventLogger ) : MCUSimSubsys ( eventLogger, ID_PLIO ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        virtual unsigned int getNumberOfPorts() = 0;

        /**
         * @brief
         * @return
         */
        virtual unsigned int getNumberOfBitsPerPort() = 0;

        /**
         * @brief
         * @return
         */
        virtual unsigned int getNumberOfBitsTotal() = 0;

        /**
         * @brief
         * @param[in] port
         * @param[in] bit
         * @return
         */
        virtual bool read ( unsigned int port,
                            unsigned int bit ) = 0;

        /**
         * @brief
         * @param[in] port
         * @return
         */
        virtual unsigned int read ( unsigned int port ) = 0;

        /**
         * @brief
         * @param[in] port
         * @param[in] bit
         * @param[in] value
         */
        virtual void write ( unsigned int port,
                             unsigned int bit,
                             bool value ) = 0;

        /**
         * @brief
         * @param[in] port
         * @param[in] value
         */
        virtual void write ( unsigned int port,
                             unsigned int value ) = 0;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        char * getInputArray()
        {
            return m_inputBitArray;
        }

        /**
         * @brief
         * @return
         */
        char * getOutputArray()
        {
            return m_outputBitArray;
        }

    ////    Inline Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in,out] eventLogger
         */
        void link ( MCUSimEventLogger * eventLogger )
        {
            MCUSimSubsys::link ( eventLogger, ID_PLIO );
        }

    ////    Protected Attributes    ////
    protected:
        ///
        unsigned int m_numberOfBits;

        /// @brief When port is read by the MCU, this array is used as source of the read value.
        char * m_inputBitArray;

        /// @brief When port value is changed by the MCU, this array is used to store the new port value.
        char * m_outputBitArray;
};

#endif // MCUSIMPURELOGICIO_H
