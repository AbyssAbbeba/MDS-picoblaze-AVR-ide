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
 * @file PIC8ConfigWord.h
 */
// =============================================================================

#ifndef PIC8FUSESANDLOCKS_H
#define PIC8FUSESANDLOCKS_H

#include "../MCUSim.h"

#include <cstdint>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8ConfigWord
 */
class PIC8ConfigWord : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief Bits of the configuration word
         */
        enum CfgWordBit
        {
            // Bit          Bit #
            CFGW_CP,        ///< Code Protection bit
            CFGW_PWRTE,     ///< Power-up Timer Enable bit
            CFGW_WDTE,      ///< Watchdog Timer Enable bit
            CFGW_F0SC1,     ///< Oscillator Selection bit (1: RC|HS, 0: XT|LP)
            CFGW_F0SC0,     ///< Oscillator Selection bit (1: RC|XT, 0: HS|LP)

            CFGW__MAX__     ///<
        };

        /**
         * @brief
         */
        struct Config
        {
            ///
            uint8_t m_defaultCfgWord;
        };

    ////    Constructors and Destructors    ////
    public:
        PIC8ConfigWord() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PIC8ConfigWord * link ( MCUSimEventLogger * eventLogger );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

        /**
         * @brief
         * @param[in] configWord
         */
        void setWord ( unsigned int configWord );

        /**
         * @brief
         * @return
         */
        unsigned int getWord();

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] bit
         * @return
         */
        bool operator[] ( CfgWordBit bit ) const
        {
            return m_cfgWord[bit];
        }

        /**
         * @brief
         * @param[in] bit
         * @param[in] value
         */
        void setFuse ( CfgWordBit bit,
                       bool value )
        {
            m_cfgWord[bit] = value;
        }

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void loadConfig();

        /**
         * @brief
         */
        inline void resetToInitialValues();

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        bool m_cfgWord [ CFGW__MAX__ ];
};

#endif // PIC8FUSESANDLOCKS_H
