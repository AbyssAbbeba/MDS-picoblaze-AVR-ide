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
 * @ingroup AsmTranslator
 * @file main.cxx
 */
// =============================================================================

// AsmTranslator header files.
#include "AsmTranslator.h"

// Standard header files
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

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
    std::cout << QObject::tr("Usage:").toStdString() << std::endl
              << QObject::tr("    %1 <OPTIONS> [ -- ] <input_file>").arg(executable)
                            .toStdString() << std::endl
              << std::endl;

    std::cout << QObject::tr("Options:").toStdString() << std::endl
              << QObject::tr("    --type, -t <AsmVariant>").toStdString() << std::endl
              << QObject::tr("        Specify variant of assembly language used in the input file, possible options "
                             "are:").toStdString() << std::endl
              << QObject::tr("            1 : Xilinx KCPSMx.").toStdString() << std::endl
              << QObject::tr("            2 : Mediatronix KCPSMx.").toStdString() << std::endl
              << QObject::tr("            3 : openPICIDE KCPSMx.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --output, -o <file.asm>").toStdString() << std::endl
              << QObject::tr("        Specify output file.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --cfg-ind <indentation>").toStdString() << std::endl
              << QObject::tr("        Indent with:").toStdString() << std::endl
              << QObject::tr("            - keep   : do not alter original indentation,").toStdString()
              << std::endl
              << QObject::tr("            - spaces : indent with spaces (default),").toStdString() << std::endl
              << QObject::tr("            - tabs   : indent with tabs.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --cfg-tabsz <n>").toStdString() << std::endl
              << QObject::tr("        Consider tab to be displayed at most n spaces wide, here it is by default 8.")
                            .toStdString() << std::endl
              << std::endl
              << QObject::tr("    --cfg-eof <end of line character>").toStdString() << std::endl
              << QObject::tr("        Specify line separator, available options are:").toStdString() << std::endl
              << QObject::tr("            - crlf : [WINDOWS] use sequence of carriage return and line feed characters")
                            .toStdString() << std::endl
              << QObject::tr("            - lf   : [UNIX] use a single line feed character (0x0a = '\\n') (default),")
                            .toStdString() << std::endl
              << QObject::tr("            - cr   : [APPLE] use single carriage return (0x0d = '\\r').")
                            .toStdString() << std::endl
              << std::endl
              << QObject::tr("    --short-inst <true|false>").toStdString() << std::endl
              << QObject::tr("        Use instruction shortcuts, e.g. `in' instead of `input', etc. (default: false).")
                            .toStdString() << std::endl
              << std::endl
              << QObject::tr("    --cfg-lc-sym <character>").toStdString() << std::endl
              << QObject::tr("        Use uppercase, or lowercase characters for symbols:").toStdString() << std::endl
              << QObject::tr("            - l : lowercase (default),").toStdString() << std::endl
              << QObject::tr("            - u : uppercase.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --cfg-lc-dir <character>").toStdString() << std::endl
              << QObject::tr("        Use uppercase, or lowercase characters for directives:").toStdString()
              << std::endl
              << QObject::tr("            - l : lowercase (default),").toStdString() << std::endl
              << QObject::tr("            - u : uppercase.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --cfg-lc-inst <character>").toStdString() << std::endl
              << QObject::tr("        Use uppercase, or lowercase characters for instructions:").toStdString()
              << std::endl
              << QObject::tr("            - l : lowercase (default),").toStdString() << std::endl
              << QObject::tr("            - u : uppercase.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --backup, -b").toStdString() << std::endl
              << QObject::tr("        Enable generation of backup files.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --version, -V").toStdString() << std::endl
              << QObject::tr("        Print version and exit.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --help, -h").toStdString() << std::endl
              << QObject::tr("        Print this message.").toStdString() << std::endl
              << std::endl
              << std::endl;

    std::cout << QObject::tr("Notes:").toStdString() << std::endl
              << QObject::tr("    * `--' marks the end of options, it becomes useful when you want to translate file")
              .toStdString() << std::endl
              << QObject::tr("      which name could be mistaken for a command line option.").toStdString() << std::endl
              << std::endl
              << std::endl;

    std::cout << QObject::tr("Examples of usage:").toStdString() << std::endl
              << "    " << executable << " --type=1 --input=my_file.psm --output=final_file.asm" << std::endl
              << std::endl;
}

/**
 * @brief
 * @param[in] opt
 */
inline void invalidCfgOption ( const char * opt )
{
    std::cout << QObject::tr("Error: invalid configuration option value: ").toStdString() << "`" << opt << "'."
              << std::endl;
}

int main ( int argc, char ** argv )
{
    // -----------------------------------------------------------------------------------------------------------------
    // Process the CLI arguments and act accordingly.
    // -----------------------------------------------------------------------------------------------------------------

    std::cout << QObject::tr("MDS assembler translator v%1").arg(VERSION).toStdString() << std::endl
              << QObject::tr("(C) copyright 2013, 2014 Moravia Microsystems, s.r.o., Brno, CZ, European Union.")
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

    std::string input;
    std::string output;
    AsmTranslator translator;
    AsmTranslator::Variant type = AsmTranslator::V_INVALID;
    bool makeBackup = false;

    // Disable error messages from getopt_long().
    opterr = 0;

    const char * shortopts = ":hVbo:t:";
    static const struct option longopts[] =
    {
        { "help",        no_argument,       0, 'h' },
        { "version",     no_argument,       0, 'V' },
        { "backup",      no_argument,       0, 'b' },

        { "output",      required_argument, 0, 'o' },
        { "type",        required_argument, 0, 't' },

        { "cfg-ind",     required_argument, 0, 0x100 },
        { "cfg-tabsz",   required_argument, 0, 0x101 },
        { "cfg-eof",     required_argument, 0, 0x102 },
        { "short-inst",  required_argument, 0, 0x103 },
        { "cfg-lc-sym",  required_argument, 0, 0x104 },
        { "cfg-lc-dir",  required_argument, 0, 0x105 },
        { "cfg-lc-inst", required_argument, 0, 0x106 },

        { 0,             0,                 0, 0   }
    };

    int opt;
    int idx = 0;
    while ( -1 != ( opt = getopt_long(argc, argv, shortopts, longopts, &idx) ) )
    {
        switch ( opt )
        {
            case 'h': // --help
                printHelp(argv[0]);
                return 0;

            case 'V': // --version
                std::cout << QObject::tr("VERSION: %1").arg(VERSION).toStdString() << std::endl;
                return 0;

            case 'b': // --backup
                makeBackup = true;
                break;

            case 'o': // --output
                output = optarg;
                break;

            case 't': // --type
                if ( 0 == strcmp ( optarg, "1" ) )
                {
                    type = AsmTranslator::V_KCPSM_XILINX;
                }
                else if ( 0 == strcmp ( optarg, "2" ) )
                {
                    type = AsmTranslator::V_KCPSM_MEDIATRONIX;
                }
                else if ( 0 == strcmp ( optarg, "3" ) )
                {
                    type = AsmTranslator::V_KCPSM_OPENPICIDE;
                }
                break;

            case 0x100: // --cfg-ind
                if ( 0 == strcmp(optarg, "tabs") )
                {
                    translator.m_config.m_indentation = AsmTranslatorConfig::IND_TABS;
                }
                else if ( 0 == strcmp(optarg, "spaces") )
                {
                    translator.m_config.m_indentation = AsmTranslatorConfig::IND_SPACES;
                }
                else if ( 0 == strcmp(optarg, "keep") )
                {
                    translator.m_config.m_indentation = AsmTranslatorConfig::IND_KEEP;
                }
                else
                {
                    invalidCfgOption(argv[optind-1]);
                    return 1;
                }
                break;

            case 0x101: // --cfg-tabsz
            {
                int len = strlen(optarg);
                bool failed = false;

                for ( int i = 0; i < len; i++ )
                {
                    if ( ( i > 2 ) || ( 0 == isdigit(optarg[i]) ) )
                    {
                        failed = true;
                        break;
                    }
                }

                if ( false == failed )
                {
                    translator.m_config.m_tabSize = (unsigned int) atoi(optarg);
                }
                else
                {
                    invalidCfgOption(argv[optind-1]);
                    return 1;
                }
                break;
            }

            case 0x102: // --cfg-eof
                if ( 0 == strcmp(optarg, "lf") )
                {
                    translator.m_config.m_eol = AsmTranslatorConfig::EOF_LF;
                }
                else if ( 0 == strcmp(optarg, "cr") )
                {
                    translator.m_config.m_eol = AsmTranslatorConfig::EOF_CR;
                }
                else if ( 0 == strcmp(optarg, "crlf") )
                {
                    translator.m_config.m_eol = AsmTranslatorConfig::EOF_CRLF;
                }
                else
                {
                    invalidCfgOption(argv[optind-1]);
                    return 1;
                }
                break;

            case 0x103: // --short-inst
                if ( 0 == strcmp(optarg, "true") )
                {
                    translator.m_config.m_shortInstructions = true;
                }
                else if ( 0 == strcmp(optarg, "false") )
                {
                    translator.m_config.m_shortInstructions = false;
                }
                else
                {
                    invalidCfgOption(argv[optind-1]);
                    return 1;
                }
                break;

            case 0x104: // --cfg-lc-sym
            case 0x105: // --cfg-lc-dir
            case 0x106: // --cfg-lc-inst
            {
                AsmTranslatorConfig::Field field;
                if ( opt == 0x104 )
                {
                    field = AsmTranslatorConfig::F_SYMBOL;
                }
                else if ( opt == 0x105 )
                {
                    field = AsmTranslatorConfig::F_DIRECTIVE;
                }
                else
                {
                    field = AsmTranslatorConfig::F_INSTRUCTION;
                }

                if ( 0 == strcmp(optarg, "l") )
                {
                    translator.m_config.m_letterCase[field] = AsmTranslatorConfig::LC_LOWERCASE;
                }
                else if ( 0 == strcmp(optarg, "u") )
                {
                    translator.m_config.m_letterCase[field] = AsmTranslatorConfig::LC_UPPERCASE;
                }
                else
                {
                    invalidCfgOption(argv[optind-1]);
                    return 1;
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
    for ( int i = optind; i < argc; i++ )
    {
        if ( false == input.empty() )
        {
            std::cerr << QObject::tr("Error: only one input file may be specified.").toStdString() << std::endl;
            ok = false;
            break;
        }
        input = argv[i];
    }
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

    bool result = translator.translate ( type, output, input, makeBackup );
    for ( const auto & i : translator.getMessages() )
    {
        if ( 0 == i.first )
        {
            std::cout << input << ": " << i.second << std::endl;
        }
        else
        {
            std::cout << input << ":" << i.first << ": " << i.second << std::endl;
        }
    }
    return ( ( true == result ) ? 0 : 2 );
}
