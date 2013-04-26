// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2013
 * @ingroup PicoBlaze
 * @file PicoBlazeIO.h
 */
// =============================================================================

#ifndef PICOBLAZEIO_H
#define PICOBLAZEIO_H

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeIO
 */
class PicoBlazeIO : public MCUSim::PureLogicIO
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int NUMBER_OF_PORTS = 256;

        /// @brief
        static const unsigned int NUMBER_OF_BITS_PER_PORT = 8;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Config
        {
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PicoBlazeIO();

        /**
         * @brief
         */
        virtual ~PicoBlazeIO();

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] portID
         * @return
         */
        inline unsigned int input ( unsigned int portID );

        /**
         * @brief
         * @param[in] portID
         * @param[in] value
         */
        inline void output ( unsigned int portID,
                             unsigned int value );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PicoBlazeIO * link ( MCUSim::EventLogger * eventLogger );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

        /**
         * @brief
         * @return
         */
        virtual unsigned int getNumberOfPorts();

        /**
         * @brief
         * @return
         */
        virtual unsigned int getNumberOfBitsPerPort();

        /**
         * @brief
         * @return
         */
        virtual unsigned int getNumberOfBitsTotal();

        /**
         * @brief
         * @param[in] port
         * @param[in] bit
         * @return
         */
        virtual bool read ( unsigned int port,
                            unsigned int bit );

        /**
         * @brief
         * @param[in] port
         * @return
         */
        virtual unsigned int read ( unsigned int port );

        /**
         * @brief
         * @param[in] port
         * @param[in] bit
         * @param[in] value
         */
        virtual void write ( unsigned int port,
                             unsigned int bit,
                             bool value );

        /**
         * @brief
         * @param[in] port
         * @param[in] value
         */
        virtual void write ( unsigned int port,
                             unsigned int value );

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline unsigned int PicoBlazeIO::input ( unsigned int portID )
{
    logEvent ( EVENT_PLIO_READ, portID );
    return ( unsigned int ) m_inputBitArray [ portID ];
}

inline void PicoBlazeIO::output ( unsigned int portID,
                                  unsigned int value )
{
    logEvent ( EVENT_PLIO_WRITE, portID, value );
    m_outputBitArray [ portID ] = ( char ) value;
}

#endif // PICOBLAZEIO_H
