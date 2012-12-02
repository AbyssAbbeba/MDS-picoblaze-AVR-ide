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
 * @file PIC8ConfigWord.h
 */
// =============================================================================

#ifndef PIC8FUSESANDLOCKS_H
#define PIC8FUSESANDLOCKS_H

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8ConfigWord
 */
class PIC8ConfigWord : public MCUSim::Subsys {
public:
    PIC8ConfigWord() {};

    // Bits of the configuration word
    // -----------------------
    enum CfgWordBit {
        // Bit          Bit #
        CFGW_CP,        ///< Code Protection bit
        CFGW_PWRTE,     ///< Power-up Timer Enable bit
        CFGW_WDTE,      ///< Watchdog Timer Enable bit
        CFGW_F0SC1,     ///< Oscillator Selection bit (1: RC|HS, 0: XT|LP)
        CFGW_F0SC0,     ///< Oscillator Selection bit (1: RC|XT, 0: HS|LP)

        CFGW__MAX__
    };

    struct Config {
        unsigned char m_defaultCfgWord;
    };

    bool operator[] (CfgWordBit bit) const {
        return m_cfgWord[bit];
    }
    void setFuse(CfgWordBit bit, bool value) {
        m_cfgWord[bit] = value;
    }

    Config m_config;

    PIC8ConfigWord * link(MCUSim::EventLogger * eventLogger);
    void reset(MCUSim::ResetMode mode);

protected:
    bool m_cfgWord[CFGW__MAX__];

private:
    inline void loadConfig();
    inline void resetToInitialValues();
};

#endif // PIC8FUSESANDLOCKS_H
