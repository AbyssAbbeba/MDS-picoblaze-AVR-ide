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
 * @ingroup Disassembler
 * @file main.cxx
 */
// =============================================================================

// Feature set configuration.
#include "../mds.h"

// Disassembler header files.
#ifdef MDS_FEATURE_PICOBLAZE
#  include "DAsmPicoBlazeKcpsm6.h"
#  include "DAsmPicoBlazeKcpsm3.h"
#  include "DAsmPicoBlazeKcpsm2.h"
#  include "DAsmPicoBlazeKcpsm1.h"
#  include "DAsmPicoBlazeKcpsm1CPLD.h"
#endif // MDS_FEATURE_PICOBLAZE
#ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
#  include "DAsmAdaptable.h"
#  include "AdjSimProcDefParser.h"
#endif // MDS_FEATURE_ADAPTABLE_SIMULATOR

// MCU memory data container libraries.
#include "HexFile.h"
#include "BinFile.h"
#include "SrecFile.h"
#include "XilMemFile.h"
#include "XilVHDLFile.h"
#include "XilVerilogFile.h"
#include "RawHexDumpFile.h"

// Standard headers.
#include <cctype>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>

// getopt_long() function.
#include <getopt.h>

// Used for i18n only.
#include <QObject>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

/**
 * @brief Program version string.
 * @note Be careful with this value, it might be expected in "[0-9]+\.[0-9]+(\.[0-9]+)?" format.
 */
static const char * VERSION = MDS_VERSION;

/// @brief Program exit codes.
enum ExitCode
{
    EXIT_CODE_SUCCESS = 0, ///< Everything went smoothly and disassembly was successful.
    EXIT_ERROR_DASM   = 1, ///< Disassembler attempted to execute the given task but for some reason without success.
    EXIT_ERROR_CLI    = 2  ///< Command line interface did not understand the task, disassembler was not even started.
};

/**
 * @brief Print help message, a short introduction how to use this program.
 * @param[in] executable Name of executable binary from which this program was launched.
 */
void printHelp ( const char * executable )
{
    std::cout << QObject::tr("Usage:").toStdString() << std::endl
              << QObject::tr("    %1 <OPTIONS> [ -- ] <input_file>").arg(executable)
                            .toStdString() << std::endl << std::endl;

    std::cout << QObject::tr("Available program options:").toStdString() << std::endl
              << QObject::tr("    -a, --arch <architecture>").toStdString() << std::endl
              << QObject::tr("        Specify processor architecture, supported architectures are:")
                            .toStdString() << std::endl
            #ifdef MDS_FEATURE_AVR8
              << QObject::tr("            - avr8      : 8-bit AVR,").toStdString() << std::endl
            #endif // MDS_FEATURE_AVR8
            #ifdef MDS_FEATURE_PIC8
              << QObject::tr("            - pic8      : 8-bit PIC,").toStdString() << std::endl
            #endif // MDS_FEATURE_PIC8
            #ifdef MDS_FEATURE_MCS51
              << QObject::tr("            - mcs51     : MCS-51,").toStdString() << std::endl
            #endif // MDS_FEATURE_MCS51
            #ifdef MDS_FEATURE_PICOBLAZE
              << QObject::tr("            - PicoBlaze : (K)constant Coded Programmable State Machine.")
                            .toStdString() << std::endl
            #endif // MDS_FEATURE_PICOBLAZE
            #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
              << QObject::tr("            - Adaptable : User defined soft-core processor.").toStdString() << std::endl
            #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
              << std::endl
              << QObject::tr("    -f, --family <family>").toStdString() << std::endl
              << QObject::tr("        Specify processor family, supported families for the given architectures are:")
                            .toStdString() << std::endl
              << QObject::tr("            - PicoBlaze : kcpsm1, kcpsm1cpld, kcpsm2, kcpsm3, kcpsm6.")
                            .toStdString() << std::endl
              << std::endl
              << QObject::tr("    -o, --out <output file>").toStdString() << std::endl
              << QObject::tr("        Specify output file where the resulting assembly language code will be stored.")
                            .toStdString() << std::endl
              << std::endl
              << QObject::tr("    -t, --type <input file format>").toStdString() << std::endl
              << QObject::tr("        Type of the input file; if none provided, disassembler will try to guess the ")
                            .toStdString() << std::endl
              << QObject::tr("        format from input file extension, supported types are:")
                            .toStdString() << std::endl
              << QObject::tr("            - hex    : Intel 8 HEX, or Intel 16 HEX,").toStdString() << std::endl
              << QObject::tr("            - rawhex : Raw HEX dump,").toStdString() << std::endl
              << QObject::tr("            - srec   : Motorola S-Record,").toStdString() << std::endl
              << QObject::tr("            - bin    : raw binary file,").toStdString() << std::endl
              << QObject::tr("            - mem    : Xilinx MEM file (for PicoBlaze only),").toStdString() << std::endl
              << QObject::tr("            - vhd    : VHDL file (for PicoBlaze only),").toStdString() << std::endl
              << QObject::tr("            - v      : Verilog file (for PicoBlaze only).").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --cfg-ind <indentation>").toStdString() << std::endl
              << QObject::tr("        Indent with:").toStdString() << std::endl
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
              << QObject::tr("            - lf : [UNIX] use a single line feed character (0x0a = '\\n') (default),")
                            .toStdString() << std::endl
              << QObject::tr("            - cr : [APPLE] use single carriage return (0x0d = '\\r').")
                            .toStdString() << std::endl
              << std::endl
              << QObject::tr("    --cfg-sym <symbols>").toStdString() << std::endl
              << QObject::tr("        Which kind of addresses should be translated to symbolic names, and which should")
                            .toStdString() << std::endl
              << QObject::tr("        remain to be represented as numbers, available options are:")
                            .toStdString() << std::endl
              << QObject::tr("            - c : program memory,").toStdString() << std::endl
              << QObject::tr("            - d : data memory,").toStdString() << std::endl
              << QObject::tr("            - r : register file,").toStdString() << std::endl
              << QObject::tr("            - p : port address,").toStdString() << std::endl
              << QObject::tr("            - k : immediate constants,").toStdString() << std::endl
              << QObject::tr("        this options takes any combination of these, i.e. for example \"CDR\" stands")
                            .toStdString() << std::endl
              << QObject::tr("        for program memory + data memory + register file.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --cfg-lc <character>").toStdString() << std::endl
              << QObject::tr("        Use uppercase, or lowercase characters:").toStdString() << std::endl
              << QObject::tr("            - l : lowercase (default),").toStdString() << std::endl
              << QObject::tr("            - u : uppercase.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    --cfg-radix <radix>").toStdString() << std::endl
              << QObject::tr("        Radix, available options are:.").toStdString() << std::endl
              << QObject::tr("            - h : hexadecimal (default),").toStdString() << std::endl
              << QObject::tr("            - d : decimal,").toStdString() << std::endl
              << QObject::tr("            - b : binary,").toStdString() << std::endl
              << QObject::tr("            - o : octal.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    -h, --help").toStdString() << std::endl
              << QObject::tr("        (Print this message.)").toStdString() << std::endl
              << std::endl
              << QObject::tr("    -V, --version").toStdString() << std::endl
              << QObject::tr("        Print disassembler version and exit.").toStdString() << std::endl
              << std::endl
              << std::endl;

    std::cout << QObject::tr("Notes:").toStdString() << std::endl
              << QObject::tr("    * `--' marks the end of options, it becomes useful when you want to disassemble file")
                            .toStdString() << std::endl
              << QObject::tr("      which name could be mistaken for a command line option.").toStdString() << std::endl
              << std::endl;
}

/**
 * @brief Print an extremely short help message, just to give the user an idea what to do with this...
 * @param[in] executable Name of executable binary from which this program was launched.
 */
inline void printUsage ( const char * executable )
{
    std::cout << QObject::tr("Type: `").toStdString() << executable << " -h'" << QObject::tr(" for help.").toStdString()
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

/**
 * @brief Program main function.
 * @param[in] argc Number of arguments.
 * @param[in] argv Array of arguments.
 * @return Program exit status, member of ExitCode enumeration.
 */
int main ( int argc, char ** argv )
{
    DAsm * disasm = nullptr;
    DataFile * dataFile = nullptr;
    DAsm::Config config;

    std::string infile;
    std::string outfile;
    std::string architecture;
    std::string family;
    std::string type;

    bool inTypeGuessed = false;

    std::cout << QObject::tr("MDS mutitarget disassembler v%1").arg(VERSION).toStdString() << std::endl
              << QObject::tr("(C) copyright 2013, 2014 Moravia Microsystems, s.r.o., Brno, CZ, European Union.")
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

    const char * shortopts = ":hVo:a:f:t:";
    static const struct option longopts[] =
    {
        { "help",        no_argument,       0, 'h'   },
        { "version",     no_argument,       0, 'V'   },

        { "out",         required_argument, 0, 'o'   },
        { "arch",        required_argument, 0, 'a'   },
        { "family",      required_argument, 0, 'f'   },
        { "type",        required_argument, 0, 't'   },

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
            case 'h': // --help
                printHelp(argv[0]);
                return EXIT_CODE_SUCCESS;

            case 'V': // --version
                std::cout << QObject::tr("VERSION: %1").arg(VERSION).toStdString() << std::endl;
                return EXIT_CODE_SUCCESS;

            case 'o':  outfile      = optarg; break;
            case 'a':  architecture = optarg; break;
            case 'f':  family       = optarg; break;
            case 't':  type         = optarg; break;

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
                    if ( ( i > 2 ) || ( 0 == isdigit(optarg[i]) ) )
                    {
                        failed = true;
                        break;
                    }
                }

                if ( false == failed )
                {
                    config.m_tabSize = (unsigned int) atoi(optarg);
                }
                else
                {
                    invalidCfgOption(argv[optind-1]);
                    return EXIT_ERROR_CLI;
                }
                break;
            }

            case 0x102: // --cfg-eof
                if ( 0 == strcmp(optarg, "lf") )
                {
                    config.m_eol = DAsm::Config::EOL_LF;
                }
                else if ( 0 == strcmp(optarg, "cr") )
                {
                    config.m_eol = DAsm::Config::EOL_CR;
                }
                else if ( 0 == strcmp(optarg, "crlf") )
                {
                    config.m_eol = DAsm::Config::EOL_CRLF;
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
                        case 'c':
                            config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate ( config.m_symbolsToGenerate |
                                                                                           DAsm::Config::STG_CODE );
                            break;
                        case 'd':
                            config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate ( config.m_symbolsToGenerate |
                                                                                           DAsm::Config::STG_DATA );
                            break;
                        case 'r':
                            config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate ( config.m_symbolsToGenerate |
                                                                                           DAsm::Config::STG_REG );
                            break;
                        case 'p':
                            config.m_symbolsToGenerate = DAsm::Config::SymbolsToGenerate ( config.m_symbolsToGenerate |
                                                                                           DAsm::Config::STG_PORT );
                            break;
                        case 'k':
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
                if ( 0 == strcmp(optarg, "l") )
                {
                    config.m_letterCase = DAsm::Config::LC_LOWERCASE;
                }
                else if ( 0 == strcmp(optarg, "u") )
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
                if ( 0 == strcmp(optarg, "b") )
                {
                    config.m_radix = DAsm::Config::R_BIN;
                }
                else if ( 0 == strcmp(optarg, "o") )
                {
                    config.m_radix = DAsm::Config::R_OCT;
                }
                else if ( 0 == strcmp(optarg, "d") )
                {
                    config.m_radix = DAsm::Config::R_DEC;
                }
                else if ( 0 == strcmp(optarg, "h") )
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

    for ( int i = optind; i < argc; i++ )
    {
        if ( false == infile.empty() )
        {
            std::cerr << QObject::tr("Error: only one input file may be specified.").toStdString() << std::endl;
            return EXIT_ERROR_CLI;
        }
        infile = argv[i];
    }

    if ( ( true == architecture.empty() ) || ( true == family.empty() ) )
    {
        std::cerr << QObject::tr("Error: architecture and processor family have to be specified.").toStdString()
                  << std::endl;
        return EXIT_ERROR_CLI;
    }

    int memFileBPR = -1;
    XilHDLFile::OPCodeSize opCodeSize = XilHDLFile::OPCodeSize(-1);

    if ( false ) {}
    #ifdef MDS_FEATURE_PICOBLAZE
    else if ( "PicoBlaze" == architecture )
    {
        if ( "kcpsm1cpld" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm1CPLD();
            memFileBPR = 2;
            opCodeSize = XilHDLFile::SIZE_16b;
        }
        else if ( "kcpsm1" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm1();
            memFileBPR = 2;
            opCodeSize = XilHDLFile::SIZE_16b;
        }
        else if ( "kcpsm2" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm2();
            memFileBPR = 3;
            opCodeSize = XilHDLFile::SIZE_18b;
        }
        else if ( "kcpsm3" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm3();
            memFileBPR = 3;
            opCodeSize = XilHDLFile::SIZE_18b;
        }
        else if ( "kcpsm6" == family )
        {
            disasm = new DAsmPicoBlazeKcpsm6();
            memFileBPR = 3;
            opCodeSize = XilHDLFile::SIZE_18b;
        }
        else
        {
            std::cerr << QObject::tr ( "Error: processor family `%1' is not supported." )
                                     . arg ( architecture.c_str() ).toStdString()
                      << std::endl;
            return EXIT_ERROR_CLI;
        }
    }
    #endif // MDS_FEATURE_PICOBLAZE
    #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
    else if ( "Adaptable" == architecture )
    {
        // TODO: implement this...
//         disasm = new DAsmAdaptable(procDef);
    }
    #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
    else
    {
        std::cerr << QObject::tr("Error: architecture `%1' is not supported.").arg(architecture.c_str()).toStdString()
                  << std::endl;
        return EXIT_ERROR_CLI;
    }

    if ( true == infile.empty() )
    {
        std::cerr << QObject::tr("Error: no input file specified.").toStdString() << std::endl;
        return EXIT_ERROR_CLI;
    }

    if ( true == outfile.empty() )
    {
        std::cerr << QObject::tr("Warning: output file was not specified.").toStdString() << std::endl;
    }

    if ( true == type.empty() )
    {
        std::cerr << QObject::tr ( "Warning: input file type not specified, attempting to guess by file extension." )
                                 . toStdString()
                  << std::endl;

        inTypeGuessed = true;
        type = boost::filesystem::path(infile).extension().string();
        if ( type.size() > 0 )
        {
            type.erase(0, 1);
        }
    }

    try
    {
        if ( ( "hex" == type ) || ( "ihex" == type ) )
        {
            dataFile = new HexFile(infile);
        }
        else if ( "rawhex" == type )
        {
            dataFile = new RawHexDumpFile(RawHexDumpFile::OPCodeSize(opCodeSize), infile);
        }
        else if ( "bin" == type )
        {
            dataFile = new BinFile(infile);
        }
        else if ( "srec" == type )
        {
            dataFile = new SrecFile(infile);
        }
        else if ( "mem" == type )
        {
            if ( -1 == memFileBPR )
            {
                std::cerr << QObject::tr ( "Error: cannot use Xilinx MEM code as input file for this processor." )
                                         . toStdString()
                          << std::endl;
                return EXIT_ERROR_CLI;
            }
            dataFile = new XilMemFile(infile, memFileBPR);
        }
        else if ( "v" == type )
        {
            if ( XilHDLFile::OPCodeSize(-1) == opCodeSize )
            {
                std::cerr << QObject::tr ( "Error: cannot use Verilog source code as input file for this processor." )
                                         . toStdString()
                          << std::endl;
                return EXIT_ERROR_CLI;
            }
            dataFile = new XilVerilogFile(infile, "", "", opCodeSize);
        }
        else if ( "vhd" == type )
        {
            if ( XilHDLFile::OPCodeSize(-1) == opCodeSize )
            {
                std::cerr << QObject::tr ( "Error: cannot use VHDL source code as input file for this processor." )
                                         . toStdString()
                          << std::endl;
                return EXIT_ERROR_CLI;
            }
            dataFile = new XilVHDLFile(infile, "", "", opCodeSize);
        }
        else if ( true == inTypeGuessed )
        {
            std::cerr << QObject::tr ( "Error: unable to determinate format of the input file." )
                                     . toStdString()
                      << std::endl;
            return EXIT_ERROR_CLI;
        }
        else
        {
            std::cerr << QObject::tr ( "Error: invalid file type specification: `%1'." )
                                     . arg ( type.c_str() )
                                     . toStdString()
                      << std::endl;
            return EXIT_ERROR_CLI;
        }
    }
    catch ( const DataFileException & e )
    {
        std::cerr << QObject::tr("Error: unable to read the specified input file: `%1', reason: ")
                                .arg(infile.c_str()).toStdString()
                  << e.toString() << std::endl;
        return EXIT_ERROR_DASM;
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

    for ( const auto & msg : disasm->getMessages() )
    {
        std::cout << msg << std::endl;
    }

    if ( nullptr != dataFile )
    {
        delete dataFile;
    }
    if ( nullptr != disasm )
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
        return EXIT_ERROR_DASM;
    }
}
