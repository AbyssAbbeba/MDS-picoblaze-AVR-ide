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
 * @ingroup MoraviaScript
 * @file MScriptIdManager.cxx
 */
// =============================================================================

#include "MScriptIdManager.h"

MScriptIdManager::MScriptIdManager()
{
    m_ids.push_back(-1);
}

inline void MScriptIdManager::autoGrow()
{
    if ( -1 == m_ids[0] )
    {
        size_t origSize = m_ids.size();
        m_ids.resize ( size_t ( GROW_FACTOR * m_ids.size() ) );

        m_ids[0] = int(origSize);
        for ( size_t i = origSize; i < m_ids.size(); i++ )
        {
            m_ids[i] = int ( i + 1 );
        }

        m_ids[ m_ids.size() - 1 ] = -1;
    }
}

int MScriptIdManager::acquire()
{
    autoGrow();

    int result = m_ids[0];
    m_ids[0] = m_ids[m_ids[0]];

    return result;
}

void MScriptIdManager::release ( int id )
{
    m_ids[id] = m_ids[0];
    m_ids[0] = id;
}
