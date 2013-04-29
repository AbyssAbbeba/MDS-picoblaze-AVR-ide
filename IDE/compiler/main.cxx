// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Compiler
 * @file main.cxx
 */
// =============================================================================

#include "Compiler.h"
#include "CompilerMsgInftStdout.h"

#include <iostream>

int main ( int argc, char ** argv )
{
    CompilerMsgInftStdout msgInterface;
    CompilerOptions opts;
    Compiler compiler(&msgInterface);

    opts.m_sourceFile = argv[1];

    if ( true == compiler.compile ( CompilerBase::LI_ASM, CompilerBase::TA_KCPSM3, &opts ) )
    {
        std::cout << "SUCCESS\n";
        return 0;
    }
    else
    {
        std::cout << "FAILURE\n";
        return 1;
    }
}
