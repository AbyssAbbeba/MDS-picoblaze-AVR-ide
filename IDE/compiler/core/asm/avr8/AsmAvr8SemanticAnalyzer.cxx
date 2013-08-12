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
 * @ingroup Avr8Asm
 * @file AsmAvr8SemanticAnalyzer.cxx
 */
// =============================================================================

#include "AsmAvr8SemanticAnalyzer.h"

void AsmAvr8SemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    std::cout << "AsmAvr8SemanticAnalyzer::process:\n" << codeTree;
}

void AsmAvr8SemanticAnalyzer::setDevice ( const std::string & deviceName )
{
    std::cout << "AsmAvr8SemanticAnalyzer::setDevice ( " << deviceName << ")\n";
}
