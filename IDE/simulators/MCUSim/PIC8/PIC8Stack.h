/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup PIC8
 * @file PIC8Stack.h
 */

#ifndef PIC8STACK_H
#define PIC8STACK_H

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8Stack
 */
class PIC8Stack : public MCUSim::Subsys {
public:
    PIC8Stack();
    ~PIC8Stack();

    enum Event {
        EVENT_STACK_OVERFLOW,
        EVENT_STACK_UNDERFLOW
    };

    struct Config {
        Config() {
            m_size = 0;
        }

        unsigned int m_size;
    };

    Config m_config;

    PIC8Stack * link(MCUSim::EventLogger * eventLogger);
    void reset(MCUSim::ResetMode mode);
    inline void resetToInitialValues();
    inline void loadConfig();
    inline void mcuReset();

    /**
     * @brief
     * @param value
     */
    inline void pushOnStack(unsigned int value);

    /**
     * @brief
     * @return
     */
    inline unsigned int popFromStack();

private:
    unsigned int * m_data;
    int m_position;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline void PIC8Stack::pushOnStack(unsigned int value) {
    if ( m_config.m_size == m_position ) {
        logEvent(EVENT_STACK_OVERFLOW, m_position, value);
        m_position = 0;
    }
    m_data[m_position++] = value;
}

inline unsigned int PIC8Stack::popFromStack() {
    if ( 0 == m_position ) {
        logEvent(EVENT_STACK_UNDERFLOW, m_position, value);
        m_position = m_config.m_size;
    }
    return m_data[--m_position];
}

#endif // PIC8STACK_H
