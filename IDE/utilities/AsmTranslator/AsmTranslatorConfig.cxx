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
 * @ingroup AsmTranslator
 * @file AsmTranslatorConfig.cxx
 */
// =============================================================================

#include "AsmTranslatorConfig.h"

AsmTranslatorConfig::AsmTranslatorConfig()
{
    for ( int i = 0; i <= (int) F_INSTRUCTION; i++ )
    {
        m_letterCase[i] = LC_LOWERCASE;
    }
    m_eol = EOF_LF;
    m_indentation = IND_KEEP;
    m_tabSize = 8;
    m_shortInstructions = false;
}
