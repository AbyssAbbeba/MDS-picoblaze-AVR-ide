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
 * @ingroup PIC8
 * @file PIC8ExternalInterrupts.h
 */
// =============================================================================

#ifndef PIC8EXTERNALINTERRUPTS_H
#define PIC8EXTERNALINTERRUPTS_H

// Forward declarations
class PIC8DataMemory;
class PIC8IO;
class PIC8InterruptController;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8ExternalInterrupts
 */
class PIC8ExternalInterrupts : public MCUSim::Subsys
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        //static const unsigned int NOMINAL_T_CY = ???;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Config
        {
            ///
            bool m_enabled;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8ExternalInterrupts() {};

        /**
         * @brief
         */
        ~PIC8ExternalInterrupts() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @param[in,out] io
         * @return
         */
        PIC8ExternalInterrupts * link ( MCUSim::EventLogger     * eventLogger,
                                        PIC8DataMemory          * dataMemory,
                                        PIC8IO                  * io,
                                        PIC8InterruptController * interruptController );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

        /**
         * @brief
         * @param[in] portB
         */
        void portBRead ( unsigned int portB );

        /**
         * @brief
         */
        void clockCycles();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @return
         */
        inline void mcuReset();

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Private Attributes    ////
    private:
        /// @name PIC8 simulator subsystems
        //@{
            ///
            PIC8DataMemory * m_dataMemory;

            ///
            PIC8IO * m_io;

            ///
            PIC8InterruptController * m_interruptController;
        //@}

        /**
         * @brief
         */
        bool m_int0;

        /**
         * @brief
         */
        unsigned int m_portB;
};

#endif // PIC8EXTERNALINTERRUPTS_H
