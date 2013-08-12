// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Mcs51Asm
 * @file AsmMcs51SemanticAnalyzer.cxx
 */
// =============================================================================

#include "AsmMcs51SemanticAnalyzer.h"

void AsmMcs51SemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    std::cout << "AsmMcs51SemanticAnalyzer::process:\n" << codeTree;
}

void AsmMcs51SemanticAnalyzer::setDevice ( const std::string & deviceName )
{
    std::cout << "AsmMcs51SemanticAnalyzer::setDevice ( " << deviceName << ")\n";
}
