// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @ingroup Compiler
 * @file CompilerCSemanticAnalyzer.cxx
 */
// =============================================================================

#include "CompilerCSemanticAnalyzer.h"

#include <iostream>

CompilerCSemanticAnalyzer::CompilerCSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                                       CompilerOptions * opts )
                                                     : CompilerSemanticAnalyzer ( compilerCore, opts )
{
}

CompilerCSemanticAnalyzer::~CompilerCSemanticAnalyzer()
{
}

void CompilerCSemanticAnalyzer::setDevice ( const std::string & deviceName )
{
    std::cout << "CompilerCSemanticAnalyzer::setDevice ( " << deviceName << ")\n";
}

void CompilerCSemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    std::cout << "CompilerCSemanticAnalyzer::process:\n" << codeTree;
}
