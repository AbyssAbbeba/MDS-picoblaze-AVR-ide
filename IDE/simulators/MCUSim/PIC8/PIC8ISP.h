// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @authors Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup PIC8
 * @file PIC8ISP.h
 */
// =============================================================================

#ifndef PIC8ISP_H
#define PIC8ISP_H

// Forward declarations
class PIC8ProgramMemory;
class PIC8IO;
class PIC8ConfigWord;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8ISP
 */
class PIC8ISP : public MCUSim::Subsys
{
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
        PIC8ISP();

        /**
         * @brief
         */
        ~PIC8ISP();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] programMemory
         * @param[in,out] io
         * @param[in,out] configWord
         * @return
         */
        PIC8ISP * link ( MCUSim::EventLogger * eventLogger,
                         PIC8ProgramMemory   * programMemory,
                         PIC8IO              * io,
                         PIC8ConfigWord      * configWord );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

    ////    Inline Private Operations    ////
    private:

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
            PIC8ProgramMemory * m_programMemory;

            ///
            PIC8IO * m_io;

            ///
            PIC8ConfigWord * m_configWord;
        //@}
};

#endif // PIC8ISP_H
