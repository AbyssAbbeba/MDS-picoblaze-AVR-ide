// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MoraviaScript
 * @file MScriptExecContext.cxx
 */
// =============================================================================

#include "MScriptExecContext.h"

void MScriptExecContext::init ( const MScriptStatement * rootNode )
{
    clear();
    m_programPointer.push_back(ProgPtr(rootNode, FLAG_NORMAL));
}

void MScriptExecContext::clear()
{
    m_programPointer.clear();
}
