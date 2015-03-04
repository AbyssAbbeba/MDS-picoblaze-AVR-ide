// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCParserExtension.cxx
 */
// =============================================================================

#include "CompilerCParserExtension.h"

CompilerCParserExtension::CompilerCParserExtension()
{
    m_typedef = false;
    m_scopes.resize(1);
}

void CompilerCParserExtension::enterScopeBlock()
{
    m_scopes.resize(m_scopes.size() + 1);
}

void CompilerCParserExtension::leaveScopeBlock()
{
    m_scopes.pop_back();
}

void CompilerCParserExtension::newIdentifier ( const std::string & id,
                                               Type type )
{
    const auto it = m_scopes.back().find(id);
    if ( m_scopes.back().cend() == it )
    {
        m_scopes.back()[id] = type;
    }
}

CompilerCParserExtension::Type CompilerCParserExtension::findIdentifier ( const std::string & id )
{
    for ( const auto i : m_scopes )
    {
        const auto it = i.find(id);
        if ( i.cend() != it )
        {
            return it->second;
        }
    }

    return TYPE_UNSPECIFIED;
}
