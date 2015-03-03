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
 * @file CompilerCScopes.cxx
 */
// =============================================================================

#include "CompilerCScopes.h"

CompilerCScopes::CompilerCScopes()
{
}

CompilerCScopes::~CompilerCScopes()
{
}

void CompilerCScopes::enterScopeDeclaration ( const CompilerStatement * scope )
{
}

void CompilerCScopes::leaveScopeDeclaration()
{
}

void CompilerCScopes::newSymbol ( const CompilerCDeclaration * symbol )
{
}

void CompilerCScopes::newDataType ( const CompilerCDeclaration * dt )
{
}

void CompilerCScopes::enterScope ( const CompilerStatement * scope )
{
}

void CompilerCScopes::leaveScope()
{
}

const CompilerCDeclaration * CompilerCScopes::findSymbol ( const std::string & id )
{
}

const CompilerCDeclaration * CompilerCScopes::findDataType ( const std::string & typeName )
{
}
