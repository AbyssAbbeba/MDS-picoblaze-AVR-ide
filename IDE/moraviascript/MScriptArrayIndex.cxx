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
 * @file MScriptArrayIndex.cxx
 */
// =============================================================================

#include "MScriptArrayIndex.h"

std::string MScriptArrayIndex::toString() const
{
    std::string result;

    if ( false == m_index.empty() )
    {
        for ( const auto idx : m_index )
        {
            result += "[";
            result += idx;
            result += "]";
        }
    }
    else
    {
        for ( const auto & key : m_key )
        {
            result += "[\"";
            result += key;
            result += "\"]";
        }
    }

    return result;
}

int MScriptArrayIndex::dimensions() const
{
    if ( nullptr == this )
    {
        return 0;
    }

    if ( false == m_index.empty() )
    {
        return (int) m_index.size();
    }
    else if ( false == m_key.empty() )
    {
        return (int) -(m_key.size());
    }
    else
    {
        return 0;
    }
}

void MScriptArrayIndex::clear()
{
    m_index.clear();
    m_key.clear();
}
