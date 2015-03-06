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
 * @file CompilerCScope.cxx
 */
// =============================================================================

#include "CompilerCScope.h"

// C compiler header files.
#include "CompilerCDeclaration.h"

CompilerCScope::CompilerCScope ( CompilerCScope * parent,
                                 Type type )
                               :
                                 m_type ( type ),
                                 m_parent ( parent )
{
}

CompilerCScope::~CompilerCScope()
{
    for ( auto scope : m_chidren )
    {
        delete scope;
    }

    for ( const auto & i : m_symbols )
    {
        delete i.second;
    }

    for ( const auto & i : m_datatypes )
    {
        delete i.second;
    }
}
