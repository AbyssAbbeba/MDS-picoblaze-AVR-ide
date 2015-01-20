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

constexpr boost::regex::flag_type reFlags = ( boost::regex::extended | boost::regex::icase | boost::regex::optimize );
const boost::regex SimCmdLine::m_reBinaryPrefix      = boost::regex ( "0b[01]+",     reFlags );
const boost::regex SimCmdLine::m_reBinarySuffix      = boost::regex ( "[01]+b",      reFlags );
const boost::regex SimCmdLine::m_reOctalPrefix       = boost::regex ( "0[0-7]+",     reFlags );
const boost::regex SimCmdLine::m_reOctalSuffix       = boost::regex ( "[0-7]+[oq]",  reFlags );
const boost::regex SimCmdLine::m_reDecimalPrefix     = boost::regex ( "[0-9]+",      reFlags );
const boost::regex SimCmdLine::m_reDecimalSuffix     = boost::regex ( "[0-9]+d",     reFlags );
const boost::regex SimCmdLine::m_reHexadecimalPrefix = boost::regex ( "0x[0-9A-F]+", reFlags );
const boost::regex SimCmdLine::m_reHexadecimalSuffix = boost::regex ( "[0-9A-F]+h",  reFlags );

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

    if ( EXIT_CODE_SUCCESS != ( exitCode = commandLoop() ) )
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

    try
    {
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

        if ( "hex" == m_codeFileType )
        {
            // Intel 8 HEX, or Intel 16 HEX
            m_dataFile = new HexFile(m_codeFile);
        }
        else if ( "srec" == m_codeFileType )
        {
            // Motorola S-Record
            m_dataFile = new SrecFile(m_codeFile);
        }
        else if ( "bin" == m_codeFileType )
        {
            // Raw binary file
            m_dataFile = new BinFile(m_codeFile);
        }
        else if ( "rawhex" == m_codeFileType )
        {
            // Raw HEX dump
            m_dataFile = new RawHexDumpFile(RawHexDumpFile::OPCodeSize(opCodeSize), m_codeFile);
        }
        else if ( "vhd" == m_codeFileType )
        {
            // VHDL file
            if ( -1 == opCodeSize )
            {
                std::cerr << QObject::tr("Error: cannot use VHDL file for this device.").toStdString() << std::endl;
                return EXIT_ERROR_CLI;
            }
            m_dataFile = new XilVHDLFile(m_codeFile, "", "", opCodeSize);
        }
        else if ( "v" == m_codeFileType )
        {
            // Verilog file
            if ( -1 == opCodeSize )
            {
                std::cerr << QObject::tr("Error: cannot use Verilog file for this device.").toStdString() << std::endl;
                return EXIT_ERROR_CLI;
            }
            m_dataFile = new XilVerilogFile(m_codeFile, "", "", opCodeSize);
        }
        else if ( "mem" == m_codeFileType )
        {
            // Xilinx MEM file
            if ( -1 == memFileBPR )
            {
                std::cerr << QObject::tr("Error: cannot use MEM file for this device.").toStdString() << std::endl;
                return EXIT_ERROR_CLI;
            }
            m_dataFile = new XilMemFile(m_codeFile, memFileBPR);
        }
        else
        {
            // Unknown
            std::cerr << QObject::tr("Error: %1 is not valid file type specification.")
                                    .arg(m_codeFileType.c_str()).toStdString() << std::endl;
            return EXIT_ERROR_CLI;
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

inline SimCmdLine::ExitCode SimCmdLine::commandLoop()
{
    std::string input;
    std::string command;
    std::string subcommand;
    unsigned int pos;

    while ( false == std::cin.eof() )
    {
        if ( true == std::cin.bad() )
        {
            std::cerr << QObject::tr("Error: unable to read input.").toStdString() << std::endl;
            return EXIT_ERROR_CLI;
        }

        std::getline(std::cin, input);

        pos = 0;
        readWord(command, input, pos);
        readWord(subcommand, input, pos);

        processCommand(command, subcommand, input, pos);
    }

    return EXIT_CODE_SUCCESS;
}

inline void SimCmdLine::done()
{
    std::cout << "done" << std::endl;
}

inline void SimCmdLine::processCommand ( const std::string & command,
                                         const std::string & subcommand,
                                         const std::string & input,
                                         unsigned int & pos )
{
    bool subcmdInvalid = false;

    if ( "sim" == command )
    {
        if ( "step" == subcommand )
        {
            int steps = 1;
            {
                std::string argument;
                readWord(argument, input, pos);
                if ( false == argument.empty() )
                {
                    if ( false == readInt(steps, argument) )
                    {
                        return;
                    }
                }
            }
            if ( steps < 1 )
            {
                std::cout << QObject::tr("Error: number of steps has to be a positive integer.").toStdString()
                          << std::endl;
                return;
            }
            for ( int i = 0; i < steps; i++ )
            {
                m_simControl->stepProgram();
            }
            done();
        }
        else if ( "reset" == subcommand )
        {
            m_simControl->resetProgram();
            done();
        }
        else if ( "animate" == subcommand )
        {
            m_simControl->animateProgram(true);
            done();
        }
        else if ( "run" == subcommand )
        {
            m_simControl->runProgram(true);
            done();
        }
        else if ( "halt" == subcommand )
        {
            m_simControl->stopSimulation();
            done();
        }
        else
        {
            subcmdInvalid = true;
        }
    }
    else if ( "set" == command )
    {
        if ( "pc" == subcommand )
        {
            int value;
            std::string argument;
            readWord(argument, input, pos);
            if ( false == readInt(value, argument) )
            {
                return;
            }

            if ( value < 0 )
            {
                std::cout << QObject::tr("Error: program counter cannot be set to a negative value.").toStdString()
                          << std::endl;
                return;
            }

            MCUSimMemory * codeMem = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_MEM_CODE));
            if ( value >= codeMem->size() )
            {
                std::cout << QObject::tr ( "Error: program counter set to the specified value would exceed the size "
                                           "of program memory." ).toStdString()
                          << std::endl;
                return;
            }

            MCUSimCPU * cpu = dynamic_cast<MCUSimCPU*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_CPU));
            cpu->setProgramCounter((unsigned int) value);
            done();
        }
        else if ( "flag" == subcommand )
        {
            std::string flag;
            std::string value;

            readWord(flag, input, pos);
            readWord(value, input, pos);

            flagCommand(flag, value);
        }
        else if ( "memory" == subcommand )
        {
        }
        else if ( "size" == subcommand )
        {
        }
        else if ( "breakpoint" == subcommand )
        {
        }
        else
        {
            subcmdInvalid = true;
        }
    }
    else if ( "get" == command )
    {
        if ( "pc" == subcommand )
        {
            MCUSimCPU * cpu = dynamic_cast<MCUSimCPU*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_CPU));
            std::cout << cpu->getProgramCounter() << std::endl;
            done();
        }
        else if ( "flag" == subcommand )
        {
            std::string flag;
            readWord(flag, input, pos);
            flagCommand(flag);
        }
        else if ( "memory" == subcommand )
        {
        }
        else if ( "cycles" == subcommand )
        {
            std::cout << m_simControl->getTotalMCycles() << std::endl;
            done();
        }
        else if ( "locations" == subcommand )
        {
        }
        else
        {
            subcmdInvalid = true;
        }
    }
    else if ( "file" == command )
    {
        if ( "load" == subcommand )
        {
        }
        else if ( "save" == subcommand )
        {
        }
        else
        {
            subcmdInvalid = true;
        }
    }
    else if ( "help" == command )
    {
        helpCommand(subcommand);
    }
    else if ( "exit" == command )
    {
        std::cout << QObject::tr("Exiting on user request.").toStdString() << std::endl;

        int code = 0;
        if ( false == subcommand.empty() )
        {
            if ( false == readInt(code, subcommand) )
            {
                return;
            }
        }
        m_simControl->abortAndExit();
        m_simControl->wait();
        exit(code);
    }
    else
    {
        std::cerr << QObject::tr("Error: invalid command: ").toStdString() << command << std::endl;
    }

    if ( true == subcmdInvalid )
    {
        std::cerr << QObject::tr("Error: invalid subcommand: ").toStdString() << subcommand << std::endl;
    }
}

inline void SimCmdLine::flagCommand ( const std::string & flag )
{
}

inline void SimCmdLine::flagCommand ( const std::string & flag,
                                      const std::string & value )
{
}

inline void SimCmdLine::helpCommand ( const std::string & subcommand )
{
}

bool SimCmdLine::readInt ( int & out,
                           const std::string & in )
{
    using namespace boost;

    int base = 0;
    int start = 0;
    int end = (int) in.size();

    if ( true == regex_match(in, m_reBinaryPrefix) )
    {
        start = 2;
        base = 2;
    }
    else if ( true == regex_match(in, m_reBinarySuffix) )
    {
        base = 2;
        end--;
    }
    else if ( true == regex_match(in, m_reOctalPrefix) )
    {
        start = 1;
        base = 8;
    }
    else if ( true == regex_match(in, m_reOctalSuffix) )
    {
        base = 8;
        end--;
    }
    else if ( true == regex_match(in, m_reDecimalPrefix) )
    {
        base = 10;
    }
    else if ( true == regex_match(in, m_reDecimalSuffix) )
    {
        base = 10;
        end--;
    }
    else if ( true == regex_match(in, m_reHexadecimalPrefix) )
    {
        start = 2;
        base = 16;
    }
    else if ( true == regex_match(in, m_reHexadecimalSuffix) )
    {
        base = 16;
        end--;
    }
    else
    {
        std::cerr << QObject::tr("Error: not a valid number: ").toStdString() << in << std::endl;
        return false;
    }

    long number = strtol(in.substr(start, end).c_str(), nullptr, base);
    if ( ( number > INT_MAX ) || ( number < INT_MIN ) )
    {
        std::cerr << QObject::tr("Error: number exceeds allowed range: ").toStdString() << in << std::endl;
        return false;
    }

    out = (int) number;
    return true;
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

void SimCmdLine::readWord ( std::string & word,
                            const std::string & input,
                            unsigned int & pos)
{
    word.clear();

    while ( pos < input.size() )
    {
        char in = input[pos++];
        if ( ( ' ' == in ) || ( '\t' == in ) )
        {
            if ( true == word.empty() )
            {
                continue;
            }
            else
            {
                break;
            }
        }

        word += in;
    }
}
