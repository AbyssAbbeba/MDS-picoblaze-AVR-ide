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
                               : m_parent(parent),
                                 m_type(type)
{
}

CompilerCScope::~CompilerCScope()
{
}
