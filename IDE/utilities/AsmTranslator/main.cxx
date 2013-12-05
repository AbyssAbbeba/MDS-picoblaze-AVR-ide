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
 * @ingroup AsmTranslator
 * @file main.cxx
 */
// =============================================================================

// AsmTranslator header files.
#include "AsmTranslator.h"

// Standard header files
#include <fstream>
#include <iostream>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// getopt_long() function
#include <getopt.h>

// Used for i18n only.
#include <QObject>

/**
 * @brief Program version string.
 * @note Be careful with this value, it might be expected in "[0-9]+\.[0-9]+(\.[0-9]+)?" format.
 */
static const char * VERSION = "1.0";

/**
 * @brief Print help message, a short introduction how to use this program.
 * @param[in] executable Name of executable binary from which this program was launched.
 */
void printHelp ( const char * executable )
{
    std::cout << "Options:" << std::endl;
    std::cout << "    --all, -a" << std::endl;
    std::cout << "        Run all test cases in all suites." << std::endl;
    std::cout << "    --list, -l" << std::endl;
    std::cout << "        List all test cases in all suites." << std::endl;
    std::cout << "    --suite, -s <test_suite_number>" << std::endl;
    std::cout << "        Run specific test suite." << std::endl;
    std::cout << "    --test, -t <test_suite_number>:<test_case_number>" << std::endl;
    std::cout << "        Run specific test in specific suite." << std::endl;
    std::cout << "    --help, -h" << std::endl;
    std::cout << "        Print this message." << std::endl;
    std::cout << std::endl;
}

int main ( int argc, char ** argv )
{
    // -----------------------------------------------------------------------------------------------------------------
    // Process the CLI arguments and act accordingly.
    // -----------------------------------------------------------------------------------------------------------------

    std::cout << QObject::tr("MDS assembler translator v%1").arg(VERSION).toStdString() << std::endl
              << QObject::tr("(C) copyright 2013 Moravia Microsystems, s.r.o., Brno, CZ, European Union.")
                            .toStdString() << std::endl
              << "All rights reserved." << std::endl
              << std::endl;

    if ( argc < 2 )
    {
        std::cerr << QObject::tr ( "Error: program requires argument, type `%1 -h' for help." )
                                . arg ( argv[optind-1] )
                                . toStdString();
        std::cerr << std::endl << std::endl;
        return 1;
    }

    // Disable error messages from getopt_long().
    opterr = 0;

    const char * shortopts = ":h";
    static const struct option longopts[] =
    {
        { "help",        no_argument,       0,   'h' },
        { 0,             0,                 0, 0     }
    };

    int opt;
    int idx = 0;
    while ( -1 != ( opt = getopt_long(argc, argv, shortopts, longopts, &idx) ) )
    {
        switch ( opt )
        {
            case 'h':
                printHelp(argv[0]);
                return 0;

            /* Error states */
            case ':':
                std::cerr << QObject::tr("Error: option `%1' requires argument.").arg(argv[optind-1]).toStdString()
                          << std::endl;
                return 1;
            case '?':
                std::cerr << QObject::tr("Error: option `%1' not understood.").arg(argv[optind-1]).toStdString()
                          << std::endl;
                return 1;
            default:
                return 1;
        }
    }

    return 1;
}
/*
{
    std::ifstream input(argv[1]);
    std::ofstream output("output.log");

    AsmTranslator translator;

    if ( true == translator.translate ( AsmTranslator::V_KCPSM_XILINX, output, input ) )
    {
        for ( auto i : translator.getMessages() ) std::cout << "MSG: " << i << "\n";
        return 0;
    }
    else
    {
        for ( auto i : translator.getMessages() ) std::cout << "MSG: " << i << "\n";
        return 1;
    }
}
*/
