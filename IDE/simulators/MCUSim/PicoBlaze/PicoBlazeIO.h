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
class PicoBlazeIO : public MCUSimPureLogicIO
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
        PicoBlazeIO * link ( MCUSimEventLogger * eventLogger );

        /**
         * @brief
         * @param[in] mode
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) override;

        /**
         * @brief
         * @return
         */
        virtual unsigned int getNumberOfPorts() override;

        /**
         * @brief
         * @return
         */
        virtual unsigned int getNumberOfBitsPerPort() override;

        /**
         * @brief
         * @return
         */
        virtual unsigned int getNumberOfBitsTotal() override;

        /**
         * @brief
         * @param[in] port
         * @param[in] bit
         * @return
         */
        virtual bool read ( unsigned int port,
                            unsigned int bit ) override;

        /**
         * @brief
         * @param[in] port
         * @return
         */
        virtual unsigned int read ( unsigned int port ) override;

        /**
         * @brief
         * @param[in] port
         * @param[in] bit
         * @param[in] value
         */
        virtual void write ( unsigned int port,
                             unsigned int bit,
                             bool value ) override;

        /**
         * @brief
         * @param[in] port
         * @param[in] value
         */
        virtual void write ( unsigned int port,
                             unsigned int value ) override;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         */
        inline void clockCycle();

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Private Attributes    ////
    private:
        /// @brief
        bool m_readStrobe;

        /// @brief
        bool m_writeStrobe;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline unsigned int PicoBlazeIO::input ( unsigned int portID )
{
    logEvent ( EVENT_PLIO_READ, portID );
    m_readStrobe = true;
    return ( unsigned int ) m_inputBitArray [ portID ];
}

inline void PicoBlazeIO::output ( unsigned int portID,
                                  unsigned int value )
{
    logEvent ( EVENT_PLIO_WRITE, portID, value );
    m_writeStrobe = true;
    m_outputBitArray [ portID ] = ( char ) value;
}

inline void PicoBlazeIO::clockCycle()
{
    if ( true == m_readStrobe )
    {
        m_readStrobe = false;
        logEvent ( EVENT_PLIO_READ_END );
    }

    if ( true == m_writeStrobe )
    {
        m_writeStrobe = false;
        logEvent ( EVENT_PLIO_WRITE_END );
    }
}

#endif // PICOBLAZEIO_H
