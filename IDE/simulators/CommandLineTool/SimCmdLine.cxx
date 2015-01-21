// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup SimCmdLine
 * @file SimCmdLine.cxx
 */
// =============================================================================

#include "SimCmdLine.h"

// Feature set configuration.
#include "mds.h"

// getopt_long() function.
#include <getopt.h>

#include "SimCmdLineCommands.h"

// Standard headers.
#include <utility>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <iostream>

// Used for i18n only.
#include <QObject>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

// OS compatibility.
#include "utilities/os/os.h"

// MCU memory data container libraries.
#include "utilities/MCUDataFiles/MCUDataFiles.h"

// MCU native debug file.
#include "utilities/DbgFile/DbgFileNative.h"

// Simulator control unit.
#include "../SimControl/MCUSimControl.h"

// Library for working with processor definition files.
#include "utilities/AdjSimProcDef/AdjSimProcDef.h"
#include "utilities/AdjSimProcDef/AdjSimProcDefParser.h"

const char * SimCmdLine::VERSION = MDS_VERSION;

SimCmdLine::ExitCode SimCmdLine::main ( int argc,
                                        char ** argv )
{
    SimCmdLine::ExitCode exitCode;

    if ( EXIT_CODE_SUCCESS != ( exitCode = parseCLI(argc, argv) ) )
    {
        return exitCode;
    }

    if ( EXIT_CODE_SUCCESS != ( exitCode = initializeSim() ) )
    {
        return exitCode;
    }

    SimCmdLineCommands cmd(m_simControl);
    if ( EXIT_CODE_SUCCESS != ( exitCode = cmd.commandLoop() ) )
    {
        return exitCode;
    }

    return EXIT_CODE_SUCCESS;
}

SimCmdLine::ExitCode SimCmdLine::parseCLI ( int argc,
                                            char ** argv )
{
    if ( 1 == argc )
    {
        std::cerr << QObject::tr("Error: option required.").toStdString() << std::endl;
        printUsage(argv[0]);
        return EXIT_ERROR_CLI;
    }

    m_procDefFile.clear();
    m_device.clear();
    m_debugFile.clear();
    m_codeFile.clear();
    m_codeFileType.clear();
    m_silent = false;

    // Disable error messages from getopt_long().
    opterr = 0;

    const char * shortopts = ":hVsp:d:g:c:t:";
    static const struct option longopts[] =
    {
        { "help",           no_argument,       0,   'h' },
        { "version",        no_argument,       0,   'V' },
        { "silent",         no_argument,       0,   's' },

      #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
        { "proc-def-file",  required_argument, 0,   'p' },
      #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
        { "device",         required_argument, 0,   'd' },
        { "debug-file",     required_argument, 0,   'g' },
        { "code-file",      required_argument, 0,   'c' },
        { "code-file-type", required_argument, 0,   't' },
        { 0,                0,                 0, 0     }
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
            case 's': // --silent
                m_silent = true;
                break;
          #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
            case 'p': // --proc-def=<proc-def-file>
                m_procDefFile = optarg;
                break;
          #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
            case 'd': // --dev=<device_name>
                m_device = optarg;
                break;
            case 'g': // --debug-file=<file_name>
                m_debugFile = optarg;
                break;
            case 'c': // --code-file=<file_name>
                m_codeFile = optarg;
                break;
            case 't': // --code-file-type=<type>
                m_codeFileType = optarg;
                break;

            /* Error states */
            case ':':
                std::cerr << QObject::tr("Error: option `%1' requires argument.").arg(argv[optind-1]).toStdString()
                          << std::endl;
                return EXIT_ERROR_CLI;
            default:
                std::cerr << QObject::tr("Error: option `%1' not understood.").arg(argv[optind-1]).toStdString()
                          << std::endl;
                return EXIT_ERROR_CLI;
        }
    }

    bool redundantOptions = false;
    for ( int i = optind; i < argc; i++ )
    {
        std::cerr << QObject::tr("Error: option `%1' not understood.").arg(argv[i]).toStdString() << std::endl;
        redundantOptions = true;
    }
    if ( true == redundantOptions )
    {
        printUsage(argv[0]);
        return EXIT_ERROR_CLI;
    }

    if ( false == m_silent )
    {
        printIntro();
    }

    bool basicSettingsComplete = true;
    static const std::pair<const std::string *, const std::string> basicSettings[] =
    {
        { &m_device,       QObject::tr("device").toStdString() },
        { &m_debugFile,    QObject::tr("debug file").toStdString() },
        { &m_codeFile,     QObject::tr("machine code file").toStdString() },
        { &m_codeFileType, QObject::tr("machine code file type").toStdString() },
        { nullptr,         "" }
    };
    for ( int i = 0; nullptr != basicSettings[i].first; i++)
    {
        if ( true == basicSettings[i].first->empty() )
        {
            basicSettingsComplete = false;
            std::cerr << QObject::tr("Error: %1 not specified.").arg(basicSettings[i].second.c_str()).toStdString()
                      << std::endl;
        }
    }
    if ( false == basicSettingsComplete )
    {
        return EXIT_ERROR_CLI;
    }

    return EXIT_CODE_SUCCESS;
}

inline SimCmdLine::ExitCode SimCmdLine::initializeSim()
{
    using namespace boost::filesystem;

    m_dbgFile = nullptr;
    m_dataFile = nullptr;
    m_procDef = nullptr;
    m_simControl = nullptr;

    int memFileBPR = -1;
    XilHDLFile::OPCodeSize opCodeSize = XilHDLFile::OPCodeSize(-1);

    m_codeFile = path(makeHomeSafe(m_codeFile)).make_preferred().string();

    if ( "kcpsm1cpld" == m_device )
    {
        memFileBPR = 2;
        opCodeSize = XilHDLFile::SIZE_16b;
    }
    else if ( "kcpsm1" == m_device )
    {
        memFileBPR = 2;
        opCodeSize = XilHDLFile::SIZE_16b;
    }
    else if ( "kcpsm2" == m_device )
    {
        memFileBPR = 3;
        opCodeSize = XilHDLFile::SIZE_18b;
    }
    else if ( "kcpsm3" == m_device )
    {
        memFileBPR = 3;
        opCodeSize = XilHDLFile::SIZE_18b;
    }
    else if ( "kcpsm6" == m_device )
    {
        memFileBPR = 3;
        opCodeSize = XilHDLFile::SIZE_18b;
    }

    const auto fileType = S_FILE_TYPE.find(m_codeFileType);
    if ( S_FILE_TYPE.cend() == fileType )
    {
        // Unknown file type.
        std::cerr << QObject::tr("Error: %1 is not a valid file type specification.")
                                .arg(m_codeFileType.c_str()).toStdString() << std::endl;
        return EXIT_ERROR_CLI;
    }

    try
    {
        switch ( fileType->second )
        {
            case FT_INTEL_HEX:
                // Intel 8 HEX, or Intel 16 HEX
                m_dataFile = new HexFile(m_codeFile);
                break;

            case FT_S_REC:
                // Motorola S-Record
                m_dataFile = new SrecFile(m_codeFile);
                break;

            case FT_RAW_BIN:
                // Raw binary file
                m_dataFile = new BinFile(m_codeFile);
                break;

            case FT_RAW_HEX:
                // Raw HEX dump
                m_dataFile = new RawHexDumpFile(RawHexDumpFile::OPCodeSize(opCodeSize), m_codeFile);
                break;

            case FT_VHDL:
                // VHDL file
                if ( -1 == opCodeSize )
                {
                    std::cerr << QObject::tr("Error: cannot use VHDL file for this device.").toStdString() << std::endl;
                    return EXIT_ERROR_CLI;
                }
                m_dataFile = new XilVHDLFile(m_codeFile, "", "", opCodeSize);
                break;

            case FT_VERILOG:
                // Verilog file
                if ( -1 == opCodeSize )
                {
                    std::cerr << QObject::tr("Error: cannot use Verilog file for this device.").toStdString()
                              << std::endl;
                    return EXIT_ERROR_CLI;
                }
                m_dataFile = new XilVerilogFile(m_codeFile, "", "", opCodeSize);
                break;

            case FT_MEM:
                // Xilinx MEM file
                if ( -1 == memFileBPR )
                {
                    std::cerr << QObject::tr("Error: cannot use MEM file for this device.").toStdString() << std::endl;
                    return EXIT_ERROR_CLI;
                }
                m_dataFile = new XilMemFile(m_codeFile, memFileBPR);
                break;
        }
    }
    catch ( const DataFileException & e )
    {
        std::cerr << QObject::tr("Error: unable to read the specified machine code file: `%1', reason: ")
                                .arg(m_codeFile.c_str()).toStdString()
                  << e.toString() << std::endl;
        return EXIT_ERROR_SIMULATOR;
    }

  #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
    if ( "Adaptable" == m_device )
    {
        if ( true == m_procDefFile.empty() )
        {
            std::cerr << QObject::tr("Error: processor definition file has to be specified when using "
                                     "the Adaptable architecture.").toStdString() << std::endl;
            return EXIT_ERROR_CLI;
        }

        m_procDefFile = path(makeHomeSafe(m_procDefFile)).make_preferred().string();

        std::string data;
        std::ifstream file(m_procDefFile, std::ios_base::in | std::ios_base::binary);
        while ( false == file.eof() )
        {
            if ( true == file.bad() )
            {
                std::cerr << QObject::tr("Error: unable to read the processor definition file: %1.")
                                        .arg(m_procDefFile.c_str()).toStdString() << std::endl;
                return EXIT_ERROR_SIMULATOR;
            }

            data += (char) file.get();
        }

        try
        {
            AdjSimProcDefParser procDefParser(data);
            m_procDef = new AdjSimProcDef(procDefParser.data());
        }
        catch ( const std::runtime_error & e )
        {
            std::cerr << QObject::tr("Error: processor definition file %1 is contains invalid data:")
                                    .arg(m_procDefFile.c_str()).toStdString() << std::endl;
            std::cerr << e.what() << std::endl;
            return EXIT_ERROR_SIMULATOR;
        }
    }
  #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR

    try
    {
        m_debugFile = path(makeHomeSafe(m_debugFile)).make_preferred().string();
        m_dbgFile = new DbgFileNative(m_debugFile);
    }
    catch ( const DbgFile::Exception & e )
    {
        std::cerr << QObject::tr("Error: unable to read the specified debug file: `%1', reason: ")
                                .arg(m_debugFile.c_str()).toStdString()
                  << e.toString() << std::endl;
        return EXIT_ERROR_SIMULATOR;
    }

    m_simControl = new MCUSimControl(m_device, m_procDef);
    m_simControl->startSimulation(m_dbgFile, m_dataFile);

    if ( false == m_simControl->getMessages().empty() )
    {
        for ( const auto & msg : m_simControl->getMessages() )
        {
            std::cerr << QObject::tr("Error: ").toStdString() << msg << std::endl;
        }
        return EXIT_ERROR_SIMULATOR;
    }

    m_simControl->start();
    return EXIT_CODE_SUCCESS;
}

void SimCmdLine::printIntro()
{
    std::cout << QObject::tr("MDS processor simulator v%1").arg(VERSION).toStdString() << std::endl
              << QObject::tr("(C) copyright 2015 Moravia Microsystems, s.r.o., Brno, CZ, European Union.")
                            .toStdString() << std::endl
              << "All rights reserved." << std::endl
              << std::endl;
}

inline void SimCmdLine::printHelp ( const char * executable )
{
    printIntro();

    std::cout << QObject::tr("Usage:").toStdString() << std::endl
              << QObject::tr("    %1 <OPTIONS>")
                            .arg(executable).toStdString() << std::endl
              << std::endl;

    std::cout << QObject::tr("Available program options:").toStdString() << std::endl
              << std::endl
              << QObject::tr("    -g, --debug-file <full file name>").toStdString() << std::endl
              << QObject::tr("        (MANDATORY) Specify MDS native debug file.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    -d, --device <device>").toStdString() << std::endl
              << QObject::tr("        (MANDATORY) Specify exact device for simulation.").toStdString() << std::endl
              << QObject::tr("            - for PicoBlaze : kcpsm1, kcpsm1cpld, kcpsm2, kcpsm3, and kcpsm6.")
                            .toStdString() << std::endl
              << std::endl
              << QObject::tr("    -c, --code-file <full file name>").toStdString() << std::endl
              << QObject::tr("        (MANDATORY) Specify file with machine code for simulation.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    -t, --code-file-type <file type>").toStdString() << std::endl
              << QObject::tr("        (MANDATORY) Specify type of the machine code file, supported are:")
                            .toStdString() << std::endl
              << QObject::tr("            - rawhex : Raw HEX dump,").toStdString() << std::endl
              << QObject::tr("            - vhd    : VHDL file (for PicoBlaze only),").toStdString() << std::endl
              << QObject::tr("            - v      : Verilog file (for PicoBlaze only).").toStdString() << std::endl
              << QObject::tr("            - mem    : Xilinx MEM file (for PicoBlaze only),").toStdString() << std::endl
              << QObject::tr("            - hex    : Intel 8 HEX, or Intel 16 HEX,").toStdString() << std::endl
              << QObject::tr("            - srec   : Motorola S-Record,").toStdString() << std::endl
              << QObject::tr("            - bin    : raw binary file,").toStdString() << std::endl
              << std::endl
            #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
              << QObject::tr("    -p, --proc-def-file <processor definition file>").toStdString() << std::endl
              << QObject::tr("        Specify architecture for user defined processor, this option makes sense")
                            .toStdString() << std::endl
              << QObject::tr("        only if --device=Adaptable").toStdString() << std::endl
              << std::endl
            #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
              << QObject::tr("    -h, --help").toStdString() << std::endl
              << QObject::tr("        Print this message.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    -V, --version").toStdString() << std::endl
              << QObject::tr("        Print version information and exit.").toStdString() << std::endl
              << std::endl
              << QObject::tr("    -s, --silent").toStdString() << std::endl
              << QObject::tr("        Do not print any warnings, errors, or any other messages, stay silent.")
                            .toStdString() << std::endl
              << std::endl;

    std::cout << QObject::tr("Examples:").toStdString() << std::endl
              << QObject::tr("  %1 --device=kcpsm6 --debug-file=abc.dbg --code-file=abc.mem --code-file-type=mem")
                            .arg(executable).toStdString() << std::endl
              << QObject::tr("  %1 -d kcpsm6 -g abc.dbg -c abc.hex -t ihex")
                            .arg(executable).toStdString() << std::endl
              << std::endl;
}

void SimCmdLine::printUsage ( const char * executable )
{
    std::cout << QObject::tr("Please type `%1 -h' for help.").arg(executable).toStdString() << std::endl;
}
