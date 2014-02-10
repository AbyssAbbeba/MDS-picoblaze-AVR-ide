// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsMCUSim
 * @file MCUSimTestScriptStrategy.cxx
 */
// =============================================================================

#include "MCUSimTestScriptStrategy.h"

void MCUSimTestScriptStrategy::link ( MCUSim * simulator,
                                      std::string * execMessage,
                                      bool * success,
                                      bool * anyAssertionMade )
{
    m_simulator        = simulator;
    m_execMessage      = execMessage;
    m_success          = success;
    m_anyAssertionMade = anyAssertionMade;
}
