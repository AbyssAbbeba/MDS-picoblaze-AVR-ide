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
    for ( auto scope : m_members )
    {
        delete scope;
    }

    for ( const auto & i : m_symbols )
    {
        delete i;
    }

    for ( const auto & i : m_datatypes )
    {
        delete i;
    }
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerCScope * scope )
{

}
