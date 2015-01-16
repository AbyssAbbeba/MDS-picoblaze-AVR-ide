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
 * @ingroup Compiler
 * @file main.cxx
 */
// =============================================================================

// Feature set configuration.
#include "mds.h"

// getopt_long() function.
#include <getopt.h>

// Standard headers.
#include <utility>
#include <cstring>
#include <fstream>
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


/**
 * @brief Program version string.
 * @note Be careful with this value, it might be expected in "[0-9]+\.[0-9]+(\.[0-9]+)?" format.
 */
static const char * VERSION = MDS_VERSION;

/// @brief Program exit codes.
enum ExitCode
{
    EXIT_CODE_SUCCESS    = 0, ///< Everything went smoothly and simulation was successful.
    EXIT_ERROR_SIMULATOR = 1, ///< Simulator attempted to execute the given task but for some reason without success.
    EXIT_ERROR_CLI       = 2  ///< Command line interface did not understand the task, simulator was not even started.
};

/**
 * @brief
 */
void printIntro()
{
    std::cout << QObject::tr("MDS processor simulator v%1").arg(VERSION).toStdString() << std::endl
              << QObject::tr("(C) copyright 2015 Moravia Microsystems, s.r.o., Brno, CZ, European Union.")
                            .toStdString() << std::endl
              << "All rights reserved." << std::endl
              << std::endl;
}

/**
 * @brief Print help message, a short introduction how to use this program.
 * @param[in] executable Name of executable binary from which this program was launched.
 */
inline void printHelp ( const char * executable )
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

/**
 * @brief Print an extremely short help message, just to give the user an idea what to do with this...
 * @param[in] executable Name of executable binary from which this program was launched.
 */
void printUsage ( const char * executable )
{
    std::cout << QObject::tr("Please type `%1 -h' for help.").arg(executable).toStdString() << std::endl;
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

    if ( 1 == argc )
    {
        std::cerr << QObject::tr("Error: option required.").toStdString() << std::endl;
        printUsage(argv[0]);
        return EXIT_ERROR_CLI;
    }

    // Basic settings.
    std::string procDefFile;
    std::string device;
    std::string debugFile;
    std::string codeFile;
    std::string codeFileType;
    bool silent = false;

    // Disable error messages from getopt_long().
    opterr = 0;

    const char * shortopts = ":hVsp:d:g:c:t:";
    static const struct option longopts[] =
    {
        { "help",           no_argument,       0,   'h' },
        { "version",        no_argument,       0,   'V' },
        { "silent",         no_argument,       0,   's' },

        { "proc-def-file",  required_argument, 0,   'p' },
        { "device",         required_argument, 0,   'd' },
        { "debug-file",     required_argument, 0,   'g' },
        { "code-file",      required_argument, 0,   'c' },
        { "code-file-type", required_argument, 0,   't' },
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
            case 's': // --silent
                silent = true;
                break;

            case 'p': // --proc-def=<proc-def-file>
                procDefFile = optarg;
                break;
            case 'd': // --dev=<device_name>
                device = optarg;
                break;
            case 'g': // --debug-file=<file_name>
                debugFile = optarg;
                break;
            case 'c': // --code-file=<file_name>
                codeFile = optarg;
                break;
            case 't': // --code-file-type=<type>
                codeFileType = optarg;
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

    if ( false == silent )
    {
        printIntro();
    }

    std::cout << "procDefFile=" << procDefFile << '\n';
    std::cout << "device=" << device << '\n';
    std::cout << "debugFile=" << debugFile << '\n';
    std::cout << "codeFile=" << codeFile << '\n';
    std::cout << "codeFileType=" << codeFileType << '\n';
    std::cout << "silent=" << silent << '\n';

    bool basicSettingsComplete = true;
    static const std::pair<const std::string *, const std::string> basicSettings[] =
    {
        { &device,       QObject::tr("device").toStdString() },
        { &debugFile,    QObject::tr("debug file").toStdString() },
        { &codeFile,     QObject::tr("machine code file").toStdString() },
        { &codeFileType, QObject::tr("machine code file type").toStdString() },
        { nullptr,       "" }
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

    DbgFile * dbgFile = nullptr;
    DataFile * dataFile = nullptr;
    AdjSimProcDef * procDef = nullptr;
    MCUSimControl * simControl = nullptr;

    try
    {
        int memFileBPR = -1;
        XilHDLFile::OPCodeSize opCodeSize = XilHDLFile::OPCodeSize(-1);

        codeFile = path(makeHomeSafe(codeFile)).make_preferred().string();

        if ( "kcpsm1cpld" == device )
        {
            memFileBPR = 2;
            opCodeSize = XilHDLFile::SIZE_16b;
        }
        else if ( "kcpsm1" == device )
        {
            memFileBPR = 2;
            opCodeSize = XilHDLFile::SIZE_16b;
        }
        else if ( "kcpsm2" == device )
        {
            memFileBPR = 3;
            opCodeSize = XilHDLFile::SIZE_18b;
        }
        else if ( "kcpsm3" == device )
        {
            memFileBPR = 3;
            opCodeSize = XilHDLFile::SIZE_18b;
        }
        else if ( "kcpsm6" == device )
        {
            memFileBPR = 3;
            opCodeSize = XilHDLFile::SIZE_18b;
        }

        if ( "hex" == codeFileType )
        {
            // Intel 8 HEX, or Intel 16 HEX
            dataFile = new HexFile(codeFile);
        }
        else if ( "srec" == codeFileType )
        {
            // Motorola S-Record
            dataFile = new SrecFile(codeFile);
        }
        else if ( "bin" == codeFileType )
        {
            // Raw binary file
            dataFile = new BinFile(codeFile);
        }
        else if ( "rawhex" == codeFileType )
        {
            // Raw HEX dump
            dataFile = new RawHexDumpFile(RawHexDumpFile::OPCodeSize(opCodeSize), codeFile);
        }
        else if ( "vhd" == codeFileType )
        {
            // VHDL file
            if ( -1 == opCodeSize )
            {
                std::cerr << QObject::tr("Error: cannot use VHDL file for this device.").toStdString() << std::endl;
                return EXIT_ERROR_CLI;
            }
            dataFile = new XilVHDLFile(codeFile, "", "", opCodeSize);
        }
        else if ( "v" == codeFileType )
        {
            // Verilog file
            if ( -1 == opCodeSize )
            {
                std::cerr << QObject::tr("Error: cannot use Verilog file for this device.").toStdString() << std::endl;
                return EXIT_ERROR_CLI;
            }
            dataFile = new XilVerilogFile(codeFile, "", "", opCodeSize);
        }
        else if ( "mem" == codeFileType )
        {
            // Xilinx MEM file
            if ( -1 == memFileBPR )
            {
                std::cerr << QObject::tr("Error: cannot use MEM file for this device.").toStdString() << std::endl;
                return EXIT_ERROR_CLI;
            }
            dataFile = new XilMemFile(codeFile, memFileBPR);
        }
        else
        {
            // Unknown
            std::cerr << QObject::tr("Error: %1 is not valid file type specification.")
                                    .arg(codeFileType.c_str()).toStdString() << std::endl;
            return EXIT_ERROR_CLI;
        }
    }
    catch ( const DataFileException & e )
    {
        std::cerr << QObject::tr("Error: unable to read the specified machine code file: `%1', reason: ")
                                .arg(codeFile.c_str()).toStdString()
                  << e.toString() << std::endl;
        return EXIT_ERROR_SIMULATOR;
    }

  #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
    if ( "Adaptable" == device )
    {
        if ( true == procDefFile.empty() )
        {
            std::cerr << QObject::tr("Error: processor definition file has to be specified when using "
                                     "the Adaptable architecture.").toStdString() << std::endl;
            return EXIT_ERROR_CLI;
        }

        procDefFile = path(makeHomeSafe(procDefFile)).make_preferred().string();

        std::string data;
        std::ifstream file(procDefFile, std::ios_base::in | std::ios_base::binary);
        while ( false == file.eof() )
        {
            if ( true == file.bad() )
            {
                std::cerr << QObject::tr("Error: unable to read the processor definition file: %1.")
                                        .arg(procDefFile.c_str()).toStdString() << std::endl;
                return EXIT_ERROR_SIMULATOR;
            }

            data += (char) file.get();
        }

        try
        {
            AdjSimProcDefParser procDefParser(data);
            procDef = new AdjSimProcDef(procDefParser.data());
        }
        catch ( const std::runtime_error & e )
        {
            std::cerr << QObject::tr("Error: processor definition file %1 is contains invalid data:")
                                    .arg(procDefFile.c_str()).toStdString() << std::endl;
            std::cerr << e.what() << std::endl;
            return EXIT_ERROR_SIMULATOR;
        }
    }
  #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR

    try
    {
        debugFile = path(makeHomeSafe(debugFile)).make_preferred().string();
        dbgFile = new DbgFileNative(debugFile);
    }
    catch ( const DbgFile::Exception & e )
    {
        std::cerr << QObject::tr("Error: unable to read the specified debug file: `%1', reason: ")
                                .arg(debugFile.c_str()).toStdString()
                  << e.toString() << std::endl;
        return EXIT_ERROR_SIMULATOR;
    }

    simControl = new MCUSimControl(device, procDef);
    simControl->startSimulation(dbgFile, dataFile);

    if ( false == simControl->getMessages().empty() )
    {
        for ( const auto & msg : simControl->getMessages() )
        {
            std::cerr << QObject::tr("Error: ").toStdString() << msg << std::endl;
        }
        return EXIT_ERROR_SIMULATOR;
    }

    std::string input;
    while ( false == std::cin.eof() )
    {
        std::getline(std::cin, input);
        std::cout << "input='"<<input<<"'\n";

	unsigned int pos = 0;
	const unsigned int size = input.size();
	std::string command;
        while ( pos < size )
        {
            char in = input[pos++];
            if ( ( ' ' == in ) || ( '\t' == in ) )
	    {
	    	if ( true == command.empty() )
		{
		    	continue;
		}
		else
		{
			break;
		}
	    }

	    command += in;
        }

	std::cout << "command = '"<<command<<"'\n";
	if ( "exit" == command )
	{
	    std::cout << "Exiting on user request." << std::endl;
	    break;
	}
    }

    return EXIT_CODE_SUCCESS;
}

/*
 *
== CLI options ==
 -a, --architecture
 -d, --device
 -g, --debug-file
 -c, --code-file
 -t. --code-file-type
 -h, --help
 -V, --version
 -s, --silent

<exec> [OPTIONS]

== Commands ==
set
 - pc <address>
 - flag <flag> <value>
 - memory <space> <address> <value>
 - size <what> <value>
 - breakpoint <file:line> <value>
get
 - pc
 - flag <flag>
 - memory <space> <address> [ .. <end-address>]
 - size <what>
 - cycles // machine cycles
file
 - load <space> <type> <file>
 - save <space> <type> <file>

sim
 - step
 - animate
 - run
 - halt
 - reset

exit [<code>]

 */
