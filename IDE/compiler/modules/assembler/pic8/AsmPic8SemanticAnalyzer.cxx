// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Pic8Asm
 * @file AsmPic8SemanticAnalyzer.cxx
 */
// =============================================================================

#include "AsmPic8SemanticAnalyzer.h"

#include <iostream>

void AsmPic8SemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    std::cout << "AsmPic8SemanticAnalyzer::process:\n" << codeTree;
}

void AsmPic8SemanticAnalyzer::setDevice ( const std::string & deviceName )
{
    std::cout << "AsmPic8SemanticAnalyzer::setDevice ( " << deviceName << ")\n";
}
