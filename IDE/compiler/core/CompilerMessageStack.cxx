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
 * @ingroup Compiler
 * @file CompilerMessageStack.cxx
 */
// =============================================================================

#include "CompilerMessageStack.h"

bool CompilerMessageStack::isUnique ( const CompilerSourceLocation & location,
                                      CompilerBase::MessageType type,
                                      const std::string & text,
                                      bool ignoreLocation )
{
    if ( true == ignoreLocation )
    {
        if ( m_data.cend() != m_data.find(text) )
        {
            return false;
        }
    }
    else
    {
        auto range = m_data.equal_range(text);
        for ( MsgMap::iterator i = range.first;
              i != range.second;
              i++ )
        {
            if ( true == location.equal(i->second.first) )
            {
                return false;
            }
        }
    }

    m_data.insert ( std::pair<std::string,MsgMetaData>(text, MsgMetaData(location, type)) );
    return true;
}

void CompilerMessageStack::clear()
{
    m_data.clear();
}
