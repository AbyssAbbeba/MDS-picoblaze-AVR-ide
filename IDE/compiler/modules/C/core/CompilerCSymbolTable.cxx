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
 * @file CompilerCSymbolTable.cxx
 */
// =============================================================================

#include "CompilerCSymbolTable.h"

// Compiler header files.
#include "CompilerStatement.h"

// C compiler header files.
#include "CompilerCScope.h"

CompilerCSymbolTable::CompilerCSymbolTable()
{
    m_rootScope = new CompilerCScope(nullptr, CompilerCScope::TYPE_FILE);
}

CompilerCSymbolTable::~CompilerCSymbolTable()
{
    delete m_rootScope;
}

void CompilerCSymbolTable::enterScopeDeclaration ( const CompilerStatement * scope )
{
}

void CompilerCSymbolTable::leaveScopeDeclaration()
{
}

void CompilerCSymbolTable::newSymbol ( CompilerCDeclaration * symbol )
{
}

void CompilerCSymbolTable::newDataType ( CompilerCDeclaration * dt )
{
}

void CompilerCSymbolTable::enterScope ( const CompilerStatement * scope )
{
}

void CompilerCSymbolTable::leaveScope()
{
}

const CompilerCDeclaration * CompilerCSymbolTable::findSymbol ( const std::string & id )
{
    return nullptr;
}

const CompilerCDeclaration * CompilerCSymbolTable::findDataType ( const std::string & typeName,
                                                                  CompilerCDeclaration::Type type )
{
    return nullptr;
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerCSymbolTable * table )
{

}
