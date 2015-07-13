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
#include "CompilerCBackend.h"
#include "CompilerCSymbolTable.h"
#include "CompilerCTreeDecoder.h"
#include "CompilerCExprProcessor.h"

CompilerCSemanticAnalyzer::CompilerCSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                                       CompilerOptions * opts,
                                                       CompilerCBackend * backend )
                                                     :
                                                       CompilerSemanticAnalyzer ( compilerCore, opts ),
                                                       m_backend ( backend )
{
    m_symbolTable   = new CompilerCSymbolTable();
    m_exprProcessor = new CompilerCExprProcessor(compilerCore, opts);
    m_treeDecoder   = new CompilerCTreeDecoder(compilerCore, opts, m_backend, m_symbolTable, m_exprProcessor);
}

CompilerCSemanticAnalyzer::~CompilerCSemanticAnalyzer()
{
    delete m_symbolTable;
    delete m_exprProcessor;
    delete m_treeDecoder;
}

void CompilerCSemanticAnalyzer::setDevice ( const std::string & deviceName )
{
    std::cout << "CompilerCSemanticAnalyzer::setDevice ( " << deviceName << ")\n";
}


void CompilerCSemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    std::cout << "CompilerCSemanticAnalyzer::process:\n" << codeTree << "\n\n";
    m_treeDecoder->processCodeTree(codeTree);
}
