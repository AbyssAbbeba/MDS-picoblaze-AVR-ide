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
 * @file MScriptIdManager.cxx
 */
// =============================================================================

#include "MScriptIdManager.h"

MScriptIdManager::MScriptIdManager()
{
    m_ids.push_back ( Id ( 0, -1 ) );
}

inline void MScriptIdManager::autoReserve()
{
}

inline void MScriptIdManager::cleanUp()
{
}

int MScriptIdManager::acquire()
{
}

void MScriptIdManager::release ( int id )
{
}
