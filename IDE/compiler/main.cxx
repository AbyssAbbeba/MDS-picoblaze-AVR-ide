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

// Common compiler header files.
#include "Compiler.h"
#include "CompilerMsgInftStdout.h"

// getopt_long() function
#include <getopt.h>

// Standard headers
#include <iostream>

// Used for i18n only.
#include <QObject>

int main ( int argc, char ** argv )
{
    static const struct option longopts[] =
    {
        {"help",    no_argument,       0, 'h' },
        {"arch",    required_argument, 0, 'a' },
        {"lang",    required_argument, 0, 'l' },
        {0,         0,                 0, 0   }
    };

    CompilerOptions opts;
    CompilerMsgInftStdout msgInterface;
    Compiler compiler(&msgInterface);

    if ( 1 == argc )
    {
        std::cout << "opt required\n";
        return 0;
    }

    // Disable error messages from getopt_long().
    opterr = 0;

    int opt;
    int idx;
    while ( -1 != ( opt = getopt_long(argc, argv, ":ha:l:f:", longopts, &idx) ) )
    {
        switch ( opt )
        {
            case 'h':
                std::cout << "help\n";
                return 0;
            case 'a':
                std::cout << "arch '"<<optarg<<"'\n";
                return 0;
            case 'l':
                std::cout << "lang '"<<optarg<<"'\n";
                return 0;
            case 'f':
                opts.m_sourceFile = optarg;
                break;
            case ':':
                std::cout << "arg missing\n";
                return 0;
            case '?':
                std::cout << "arg not understood: '"<<(char)optopt<<"'\n";
                return 0;
            default:
                return 0;
        }
    }

    opts.m_lstFile = std::string(argv[1]) + ".lst";
    opts.m_codeTree = std::string(argv[1]) + ".ctr";
    opts.m_symbolTable = std::string(argv[1]) + ".sym";
    opts.m_macroTable = std::string(argv[1]) + ".mac";

    opts.m_hexFile = std::string(argv[1]) + ".hex";

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
