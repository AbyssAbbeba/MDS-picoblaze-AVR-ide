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
    std::cout << QObject::tr("Options:").toStdString() << std::endl;
    std::cout << QObject::tr("    --type, -t <AsmVariant>").toStdString() << std::endl;
    std::cout << QObject::tr("        Specify variant of assembly language used in the input file, possible options "
                             "are:").toStdString() << std::endl;
    std::cout << QObject::tr("            1 : Xilinx KCPSMx.").toStdString() << std::endl;
    std::cout << QObject::tr("    --input, -i <file.asm>").toStdString() << std::endl;
    std::cout << QObject::tr("        Specify input file.").toStdString() << std::endl;
    std::cout << QObject::tr("    --output, -o <file.asm>").toStdString() << std::endl;
    std::cout << QObject::tr("        Specify output file.").toStdString() << std::endl;
    std::cout << QObject::tr("    --backup, -b").toStdString() << std::endl;
    std::cout << QObject::tr("        Enable generation of backup files.").toStdString() << std::endl;
    std::cout << QObject::tr("    --version, -V").toStdString() << std::endl;
    std::cout << QObject::tr("        Print version and exit.").toStdString() << std::endl;
    std::cout << QObject::tr("    --help, -h").toStdString() << std::endl;
    std::cout << QObject::tr("        Print this message.").toStdString() << std::endl;
    std::cout << std::endl;
    std::cout << QObject::tr("Examples of usage:").toStdString() << std::endl;
    std::cout << "    " << executable << "--type=1 --input=my_file.psm --output=final_file.asm" << std::endl;
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

    bool makeBackup = false;
    AsmTranslator::Variant type = AsmTranslator::V_INVALID;
    std::string input;
    std::string output;

    // Disable error messages from getopt_long().
    opterr = 0;

    const char * shortopts = ":hVbi:o:t:";
    static const struct option longopts[] =
    {
        { "help",        no_argument,       0, 'h' },
        { "version",     no_argument,       0, 'V' },
        { "backup",      no_argument,       0, 'b' },
        { "input",       required_argument, 0, 'i' },
        { "output",      required_argument, 0, 'o' },
        { "type",        required_argument, 0, 't' },
        { 0,             0,                 0, 0   }
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

            case 'V':
                return 0;

            case 'b':
                makeBackup = true;
                break;

            case 'i':
                input = optarg;
                break;

            case 'o':
                output = optarg;
                break;

            case 't':
            {
                std::string arg = optarg;
                if ( "1" == arg )
                {
                    type = AsmTranslator::V_KCPSM_XILINX;
                }
            }
            break;

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

    bool ok = true;
    if ( true == input.empty() )
    {
        std::cerr << QObject::tr("Error: input file not specified.").toStdString()
                  << std::endl;
        ok = false;
    }
    if ( true == output.empty() )
    {
        std::cerr << QObject::tr("Error: output file not specified.").toStdString()
                  << std::endl;
        ok = false;
    }
    if ( AsmTranslator::V_INVALID == type )
    {
        std::cerr << QObject::tr("Error: assembly language variant not specified.").toStdString()
                  << std::endl;
        ok = false;
    }
    if ( false == ok )
    {
        return 1;
    }

    AsmTranslator translator;
    bool result = translator.translate ( type, output, input, makeBackup );
    for ( auto i : translator.getMessages() )
    {
        std::cout << i << std::endl;
    }
    return ( ( true == result ) ? 0 : 2 );
}
