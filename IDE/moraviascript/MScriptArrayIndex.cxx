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
        for ( std::vector<unsigned int>::const_iterator i = m_index.cbegin();
              i != m_index.cend();
              i++ )
        {
            result += "[";
            result += *i;
            result += "]";
        }
    }
    else
    {
        for ( std::vector<std::string>::const_iterator i = m_key.cbegin();
              i != m_key.cend();
              i++ )
        {
            result += "[\"";
            result += *i;
            result += "\"]";
        }
    }

    return result;
}

int MScriptArrayIndex::dimensions() const
{
    if ( NULL == this )
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
