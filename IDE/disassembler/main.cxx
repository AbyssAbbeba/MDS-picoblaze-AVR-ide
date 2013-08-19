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
 * @ingroup Disassembler
 * @file main.cxx
 */
// =============================================================================

// Disassembler header files.
#include "DAsmPicoBlazeKcpsm2.h"
#include "DAsmPicoBlazeKcpsm3.h"
#include "DAsmPicoBlazeKcpsm6.h"

// MCU memory data container libraries.
#include "HexFile.h"
#include "BinFile.h"
#include "SrecFile.h"

// Standard headers.
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

// getopt_long() function.
#include <getopt.h>

// Used for i18n only.
#include <QObject>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

/**
 * @brief Program version string.
 * @note Be careful with this value, it might be expected in "[0-9]+\.[0-9]+(\.[0-9]+)?" format.
 */
static const char * VERSION = "1.0";

/// @brief Program exit codes.
enum ExitCode
{
    EXIT_CODE_SUCCESS   = 0, ///< Everything went smoothly and disassembly was successful.
    EXIT_ERROR_COMPILER = 1, ///< Disassembler attempted to execute the given task but for some reason without success.
    EXIT_ERROR_CLI      = 2  ///< Command line interface did not understand the task, disassembler was not even started.
};

/**
 * @brief Print help message, a short introduction how to use this program.
 * @param[in] executable Name of executable binary from which this program was launched.
 */
void printHelp ( const char * executable )
{
    std::cout << QObject::tr("Usage:").toStdString() << std::endl
              << QObject::tr("    %1 <OPTIONS>").arg(executable).toStdString() << std::endl
              << std::endl;
}

/**
 * @brief Print an extremely short help message, just to give the user an idea what to do with this...
 * @param[in] executable Name of executable binary from which this program was launched.
 */
inline void printUsage ( const char * executable )
{
    std::cout << "Type: `" << executable << " -h'" << QObject::tr(" for help.").toStdString() << std::endl;
}

/**
 * @brief
 * @param[in] opt
 */
inline void invalidCfgOption ( const char * opt )
{
    std::cout << QObject::tr("invalid configuration option value: ").toStdString() << "`" << opt << "'." << std::endl;
}

/**
 * @brief Program main function.
 * @param[in] argc Number of arguments.
 * @param[in] argv Array of arguments.
 * @return Program exit status, member of ExitCode enumeration.
 */
int main ( int argc, char ** argv )
{
    using namespace boost::filesystem;

    DAsm * disasm = NULL;
    DataFile * dataFile = NULL;
    DAsm::Config config;

    std::string outfile;
    std::string architecture;
    std::string family;
    std::string inhex;
    std::string inbin;
    std::string insrec;

    std::cout << QObject::tr("MDS mutitarget disassembler v%1").arg(VERSION).toStdString() << std::endl
              << QObject::tr("(C) copyright 2013 Moravia Microsystems, s.r.o., Brno, CZ, European Union.")
                            .toStdString() << std::endl
              << "All rights reserved." << std::endl
              << std::endl;

    if ( 1 == argc )
    {
        std::cerr << QObject::tr("Error: option required.").toStdString() << std::endl;
        printUsage(argv[0]);
        return EXIT_ERROR_CLI;
    }

    // Disable error messages from getopt_long().
    opterr = 0;

    const char * shortopts = ":hVo:a:f:x:b:s:";
    static const struct option longopts[] =
    {
        { "help",        no_argument,       0, 'h'   },
        { "version",     no_argument,       0, 'V'   },

        { "out",         required_argument, 0, 'o'   },
        { "arch",        required_argument, 0, 'a'   },
        { "family",      required_argument, 0, 'f'   },
        { "hex",         required_argument, 0, 'x'   },
        { "binary",      required_argument, 0, 'b'   },
        { "srec",        required_argument, 0, 's'   },

        { "cfg-ind",     required_argument, 0, 0x100 },
        { "cfg-tabsz",   required_argument, 0, 0x101 },
        { "cfg-eof",     required_argument, 0, 0x102 },
        { "cfg-sym",     required_argument, 0, 0x103 },
        { "cfg-lc",      required_argument, 0, 0x104 },
        { "cfg-radix",   required_argument, 0, 0x105 },

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
                return EXIT_CODE_SUCCESS;
            case 'V':
                std::cout << QObject::tr("VERSION: %1").arg(VERSION).toStdString() << std::endl;
                return EXIT_CODE_SUCCESS;

            case 'o':  outfile      = optarg; break;
            case 'a':  architecture = optarg; break;
            case 'f':  family       = optarg; break;
            case 'x':  inhex        = optarg; break;
            case 'b':  inbin        = optarg; break;
            case 's':  insrec       = optarg; break;

            case 0x100: // --cfg-ind
                if ( 0 == strcmp(optarg, "tabs") )
                {
                    config.m_indentation = DAsm::Config::IND_TABS;
                }
                else if ( 0 == strcmp(optarg, "spaces") )
                {
                    config.m_indentation = DAsm::Config::IND_SPACES;
                }
                else
                {
                    invalidCfgOption(argv[optind-1]);
                    return EXIT_ERROR_CLI;
                }
                break;

            case 0x101: // --cfg-tabsz
            {
                int len = strlen(optarg);
                bool failed = false;

                for ( int i = 0; i < len; i++ )
                {
                    if ( ( i > 999 ) || ( 0 == isdigit(optarg[i]) ) )
                    {
                        failed = true;
                        break;
                    }
                }

                if ( false == failed )
                {
                    config.m_tabSize = atoi(optarg);
                }
                {
                    invalidCfgOption(argv[optind-1]);
                    return EXIT_ERROR_CLI;
                }
                break;
            }

            case 0x102: // --cfg-eof
                if ( 0 == strcmp(optarg, "lf") )
                {
                    config.m_eof = DAsm::Config::EOF_LF;
                }
                else if ( 0 == strcmp(optarg, "cr") )
                {
                    config.m_eof = DAsm::Config::EOF_CR;
                }
                else if ( 0 == strcmp(optarg, "crlf") )
                {
                    config.m_eof = DAsm::Config::EOF_CRLF;
                }
                else
                {
                    invalidCfgOption(argv[optind-1]);
                    return EXIT_ERROR_CLI;
                }
                break;

            case 0x103: // --cfg-sym
            {
                config.m_symbolsToGenerate = DAsm::Config::STG_NONE;
                int len = strlen(optarg);
                for ( int i = 0; i < len; i++ )
                {
                    switch ( optarg[i] )
                    {
                        case 'C':
                            config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate ( config.m_symbolsToGenerate |
                                                                                           DAsm::Config::STG_CODE );
                            break;
                        case 'D':
                            config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate ( config.m_symbolsToGenerate |
                                                                                           DAsm::Config::STG_DATA );
                            break;
                        case 'R':
                            config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate ( config.m_symbolsToGenerate |
                                                                                           DAsm::Config::STG_REG );
                            break;
                        case 'P':
                            config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate ( config.m_symbolsToGenerate |
                                                                                           DAsm::Config::STG_PORT );
                            break;
                        case 'K':
                            config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate ( config.m_symbolsToGenerate |
                                                                                           DAsm::Config::STG_CONST );
                            break;
                        default:
                            invalidCfgOption(argv[optind-1]);
                            return EXIT_ERROR_CLI;
                    }
                }
                break;
            }

            case 0x104: // --cfg-lc
                if ( 0 == strcmp(optarg, "L") )
                {
                    config.m_letterCase = DAsm::Config::LC_LOWERCASE;
                }
                else if ( 0 == strcmp(optarg, "U") )
                {
                    config.m_letterCase = DAsm::Config::LC_UPPERCASE;
                }
                else
                {
                    invalidCfgOption(argv[optind-1]);
                    return EXIT_ERROR_CLI;
                }
                break;

            case 0x105: // --cfg-radix
                if ( 0 == strcmp(optarg, "B") )
                {
                    config.m_radix = DAsm::Config::R_BIN;
                }
                else if ( 0 == strcmp(optarg, "O") )
                {
                    config.m_radix = DAsm::Config::R_OCT;
                }
                else if ( 0 == strcmp(optarg, "D") )
                {
                    config.m_radix = DAsm::Config::R_DEC;
                }
                else if ( 0 == strcmp(optarg, "H") )
                {
                    config.m_radix = DAsm::Config::R_HEX;
                }
                else
                {
                    invalidCfgOption(argv[optind-1]);
                    return EXIT_ERROR_CLI;
                }
                break;

            /* Error states */
            case ':':
                std::cerr << QObject::tr("Error: option `%1' requires argument.").arg(argv[optind-1]).toStdString()
                          << std::endl;
                return EXIT_ERROR_CLI;
            case '?':
                std::cerr << QObject::tr("Error: option `%1' not understood.").arg(argv[optind-1]).toStdString()
                          << std::endl;
                return EXIT_ERROR_CLI;
            default:
                return EXIT_ERROR_CLI;
        }
    }

    if ( ( true == architecture.empty() ) || ( true == family.empty() ) )
    {
        std::cerr << QObject::tr("Error: architecture and processor family have to be specified.").toStdString()
                  << std::endl;
        return EXIT_ERROR_CLI;
    }

    if ( false == inhex.empty() )
    {
        dataFile = new HexFile(inhex);
    }
    else if ( false == inbin.empty() )
    {
        dataFile = new BinFile(inbin);
    }
    else if ( false == insrec.empty() )
    {
        dataFile = new SrecFile(insrec);
    }
    else
    {
        std::cerr << QObject::tr("Error: no input file specified.").toStdString()
                  << std::endl;
        return EXIT_ERROR_CLI;
    }

    if ( true == outfile.empty() )
    {
        std::cerr << QObject::tr("Warning: output file was not specified.").toStdString()
                  << std::endl;
        return EXIT_ERROR_CLI;
    }

    if ( "PicoBlaze" == architecture )
    {
        if ( "kcpsm2" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm2();
        }
        else if ( "kcpsm3" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm3();
        }
        else if ( "kcpsm6" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm6();
        }
        else
        {
            std::cerr << QObject::tr ( "Error: processor family `%1' is not supported." )
                                     . arg ( architecture.c_str() ).toStdString()
                      << std::endl;
            return EXIT_ERROR_CLI;
        }
    }
    else
    {
        std::cerr << QObject::tr("Error: architecture `%1' is not supported.").arg(architecture.c_str()).toStdString()
                  << std::endl;
        return EXIT_ERROR_CLI;
    }

    disasm->m_config = config;
    bool finalResult = disasm->disassemble(*dataFile);

    if ( false == outfile.empty() )
    {
        std::ofstream file ( outfile, ( std::fstream::out | std::fstream::trunc ) );
        file << *disasm;

        if ( file.bad() )
        {
            finalResult = false;
            std::cerr << QObject::tr("Error: I/O error on the output file.").toStdString()
                      << std::endl;
        }
    }

    const std::vector<std::string> & messages = disasm->getMessages();
    for ( std::vector<std::string>::const_iterator it = messages.cbegin();
          it != messages.cend();
          it++ )
    {
        std::cout << *it << std::endl;
    }

    if ( NULL != dataFile )
    {
        delete dataFile;
    }
    if ( NULL != disasm )
    {
        delete disasm;
    }

    if ( true == finalResult )
    {
        std::cout << QObject::tr("Disassembly successful.").toStdString() << std::endl;
        return EXIT_CODE_SUCCESS;
    }
    else
    {
        std::cerr << QObject::tr("Disassembly FAILED!").toStdString() << std::endl;
        return EXIT_ERROR_COMPILER;
    }
}
