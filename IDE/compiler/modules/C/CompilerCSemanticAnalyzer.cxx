// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014, 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup Compiler
 * @file CompilerCSemanticAnalyzer.cxx
 */
// =============================================================================

#include "CompilerCSemanticAnalyzer.h"

#include<iostream>//DEBUG

// C compiler header files.
#include "CompilerCSymbolTable.h"
#include "CompilerCTreeDecoder.h"

CompilerCSemanticAnalyzer::CompilerCSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                                       CompilerOptions * opts )
                                                     :
                                                       CompilerSemanticAnalyzer ( compilerCore, opts )
{
    m_symbolTable = new CompilerCSymbolTable();
    m_treeDecoder = new CompilerCTreeDecoder(compilerCore, opts, m_symbolTable);
}

CompilerCSemanticAnalyzer::~CompilerCSemanticAnalyzer()
{
    delete m_symbolTable;
    delete m_treeDecoder;
}

void CompilerCSemanticAnalyzer::setDevice ( const std::string & deviceName )
{
    std::cout << "CompilerCSemanticAnalyzer::setDevice ( " << deviceName << ")\n";
}


void CompilerCSemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    std::cout << "CompilerCSemanticAnalyzer::process:\n" << codeTree << "\n\n";
    m_treeDecoder->processDeclarations(codeTree);
}
