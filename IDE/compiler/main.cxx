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
 * @ingroup Compiler
 * @file main.cxx
 */
// =============================================================================

// Feature set configuration.
#include "../mds.h"

// Common compiler header files.
#include "Compiler.h"
#include "CompilerMsgIntfStdout.h"

// getopt_long() function.
#include <getopt.h>

// Standard headers.
#include <iostream>
#include <cstring>

// Used for i18n only.
#include <QObject>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

// OS compatibility.
#include "../utilities/os/os.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
#  include <windows.h>
#endif

/**
 * @brief Program version string.
 * @note Be careful with this value, it might be expected in "[0-9]+\.[0-9]+(\.[0-9]+)?" format.
 */
static const char * VERSION = MDS_VERSION;

/// @brief Program exit codes.
enum ExitCode
{
    EXIT_CODE_SUCCESS   = 0, ///< Everything went smoothly and compilation was successful.
    EXIT_ERROR_COMPILER = 1, ///< Compiler attempted to execute the given task but for some reason without success.
    EXIT_ERROR_CLI      = 2  ///< Command line interface did not understand the task, compiler was not even started.
};

/**
 * @brief
 */
void printIntro()
{
    std::cout << QObject::tr("MDS mutitarget macro-assembler v%1").arg(VERSION).toStdString() << std::endl
              << QObject::tr("(C) copyright 2013, 2014, 2015 Moravia Microsystems, s.r.o., Brno, CZ, European Union.")
                            .toStdString() << std::endl
              << "All rights reserved." << std::endl
              << std::endl;
}

/**
 * @brief Print help message, a short introduction how to use this program.
 * @param[in] executable Name of executable binary from which this program was launched.
 * @note There is also a hidden option "--codetree \<code tree file\>" which is not mentioned in this help message.
 */
void printHelp ( const char * executable )
{
    printIntro();
    std::cout << QObject::tr("Usage:").toStdString() << std::endl
              << QObject::tr("    %1 <OPTIONS> [ -- ] <source file> [ <source file> ... ]")
                            .arg(executable).toStdString() << std::endl
              << std::endl;

    std::cout << QObject::tr("Available program options:").toStdString() << std::endl
              << QObject::tr("    -a, --architecture <architecture>").toStdString() << std::endl
              << QObject::tr("        (MANDATORY) Specify target architecture, supported architectures are:")
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
              << QObject::tr("    -l, --language <programming language>").toStdString() << std::endl
              << QObject::tr("        (MANDATORY) Specify programming language, supported languages are:").toStdString()<<std::endl
            #ifdef MDS_FEATURE_C_COMPILER
              << QObject::tr("            - c   : C language,").toStdString() << std::endl
            #endif // MDS_FEATURE_C_COMPILER
              << QObject::tr("            - asm : assembly language.").toStdString() << std::endl
              << QObject::tr("    -x, --hex <Intel HEX file>").toStdString() << std::endl
              << QObject::tr("        Specify output file with machine code generated as a result of compilation,")
                            .toStdString() << std::endl
              << QObject::tr("        data will be stored in Intel 8 Hex format.").toStdString() << std::endl
              << QObject::tr("    -g, --debug <MDS native debug file>").toStdString() << std::endl
              << QObject::tr("        Specify output file with code for MCU simulator and other debugging tools.")
                            .toStdString() << std::endl
              << QObject::tr("    --srec <Motorola S-REC file>").toStdString() << std::endl
              << QObject::tr("        Specify output file with machine code generated as a result of compilation,")
                            .toStdString() << std::endl
              << QObject::tr("        data will be stored in Motorola S-record format.").toStdString() << std::endl
              << QObject::tr("    --binary <binary file>").toStdString() << std::endl
              << QObject::tr("        Specify output file with machine code generated as a result of compilation,")
                            .toStdString() << std::endl
              << QObject::tr("        data will be stored in raw binary format.").toStdString() << std::endl
              << QObject::tr("    --lst <code listing>").toStdString() << std::endl
              << QObject::tr("        Specify output file where code listing generated during compilation will be "
                             "stored.").toStdString() << std::endl
              << QObject::tr("    -m, --mtable <table of macros>").toStdString() << std::endl
              << QObject::tr("        Specify file in which the compiler will put table of macros defined in your "
                             "code.").toStdString() << std::endl
              << QObject::tr("    -s, --stable <table of symbols>").toStdString() << std::endl
              << QObject::tr("        Specify file in which the compiler will put table of symbols defined in your "
                             "code.").toStdString() << std::endl
              << QObject::tr("    --strtable <table of strings>").toStdString() << std::endl
              << QObject::tr("        Specify file in which the compiler will put table of strings defined in your "
                             "code.").toStdString() << std::endl
              << QObject::tr("    -h, --help").toStdString() << std::endl
              << QObject::tr("        (Print this message.)").toStdString() << std::endl
              << QObject::tr("    -V, --version").toStdString() << std::endl
              << QObject::tr("        Print compiler version and exit.").toStdString() << std::endl
              << QObject::tr("    -c, --check").toStdString() << std::endl
              << QObject::tr("        Do not perform the actual compilation, do only lexical and syntax analysis of"
                             " the").toStdString() << std::endl
              << QObject::tr("        the provided source code and exit.").toStdString() << std::endl
              << QObject::tr("    --no-backup").toStdString() << std::endl
              << QObject::tr("        Don't generate backup files.").toStdString() << std::endl
              << QObject::tr("    --brief-msg").toStdString() << std::endl
              << QObject::tr("        Print only unique messages.").toStdString() << std::endl
              << QObject::tr("    --no-strict").toStdString() << std::endl
              << QObject::tr("        Disable certain error and warning messages.").toStdString() << std::endl
              << QObject::tr("    --no-warnings").toStdString() << std::endl
              << QObject::tr("        Do not print any warnings.").toStdString() << std::endl
              << QObject::tr("    --no-errors").toStdString() << std::endl
              << QObject::tr("        Do not print any errors.").toStdString() << std::endl
              << QObject::tr("    --no-remarks").toStdString() << std::endl
              << QObject::tr("        Do not print any remarks.").toStdString() << std::endl
              << QObject::tr("    --silent").toStdString() << std::endl
              << QObject::tr("        Do not print any warnings, errors, or any other messages, stay completely "
                             "silent.").toStdString() << std::endl
              << QObject::tr("    -I, --include <directory>").toStdString() << std::endl
              << QObject::tr("        Add directory where the compiler will search for include files.")
                            .toStdString() << std::endl
              << QObject::tr("    -d, --device <device>").toStdString() << std::endl
              << QObject::tr("        Specify exact target device.").toStdString() << std::endl
              << QObject::tr("    -P, --precompile <.prc file>").toStdString() << std::endl
              << QObject::tr("        Specify target file for generation of precompiled code.")
                            .toStdString() << std::endl
              << QObject::tr("    --vhdl <.vhd file>").toStdString() << std::endl
              << QObject::tr("        Specify target file for generation of VHDL code.").toStdString() << std::endl
              << QObject::tr("    --vhdl-tmpl <.vhd file>").toStdString() << std::endl
              << QObject::tr("        Specify VHDL template file.").toStdString() << std::endl
              << QObject::tr("    --verilog <.v file>").toStdString() << std::endl
              << QObject::tr("        Specify target file for generation of verilog code.").toStdString() << std::endl
              << QObject::tr("    --verilog-tmpl <.v file>").toStdString() << std::endl
              << QObject::tr("        Specify verilog template file.").toStdString() << std::endl
              << QObject::tr("    --mem <.mem file>").toStdString() << std::endl
              << QObject::tr("        Specify target file for generation of MEM file.").toStdString() << std::endl
              << QObject::tr("    --raw-hex-dump <.hex file>").toStdString() << std::endl
              << QObject::tr("        Specify target file for Raw Hex Dump (sequence of 5 digit long hexadecimal "
                             "numbers separated by CRLF sequence).").toStdString() << std::endl
            #ifdef MDS_FEATURE_PICOBLAZE
              << QObject::tr("    --secondary <.hex file>").toStdString() << std::endl
              << QObject::tr("        Specify target file for SPR initialization, output format is Intel HEX.").toStdString() << std::endl
            #endif // MDS_FEATURE_PICOBLAZE
            #ifdef MDS_FEATURE_C_COMPILER
              << QObject::tr("    --cunit <preprocessor_output>").toStdString() << std::endl
              << QObject::tr("        Specify target file for preprocessor output, this file is not needed or even "
                             "used by the compiler itself, it's intended for the user (valid for C language "
                             "only).").toStdString() << std::endl
            #endif // MDS_FEATURE_C_COMPILER
              << std::endl;

    std::cout << QObject::tr("Notes:").toStdString() << std::endl
              << QObject::tr("    * `--' marks the end of options, it becomes useful when you want to compile file(s) ")
                            .toStdString() << std::endl
              << QObject::tr("      which name(s) could be mistaken for a command line option.").toStdString()
                            << std::endl
              << QObject::tr("    * When multiple source files are specified, they are compiled as one unit in the ")
                            .toStdString() << std::endl
              << QObject::tr("      order in which they appear on the command line (from left to right).").toStdString()
                            << std::endl
              << std::endl;

    std::cout << QObject::tr("Examples:").toStdString() << std::endl
              << QObject::tr("  %1 --architecture=PicoBlaze --language=asm --hex=abc.hex abc.asm")
                            .arg(executable).toStdString() << std::endl
              << QObject::tr("      Compile source code file `abc.asm' (--file=abc.asm) for architecture PicoBlaze")
                            .toStdString() << std::endl
              << QObject::tr("      (--architecture=PicoBlaze) written in assembly language (--language=asm), and create file")
                            .toStdString() << std::endl
              << QObject::tr("      `abc.hex' containing machine code generated generated by the compiler.")
                            .toStdString() << std::endl
              << QObject::tr("  %1 --language asm --architecture PicoBlaze --hex abc.hex abc.asm")
                            .arg(executable).toStdString() << std::endl
              << QObject::tr("      Do the same at the above, only in this case we have used another variant")
                            .toStdString() << std::endl
              << QObject::tr("      of usage of switches with argument.").toStdString() << std::endl
              << QObject::tr("  %1 -l asm -a PicoBlaze -x abc.hex abc.asm").arg(executable).toStdString() << std::endl
              << QObject::tr("      Do the same at the above, only in this case we have used short version of")
                            .toStdString() << std::endl
              << QObject::tr("      the switches.").toStdString() << std::endl
              << std::endl;
}

/**
 * @brief Print an extremely short help message, just to give the user an idea what to do with this...
 * @param[in] executable Name of executable binary from which this program was launched.
 */
void printUsage ( const char * executable )
{
    printIntro();
    std::cout << "Type: `" << executable << " -h'" << QObject::tr(" for help.").toStdString() << std::endl;
}

/**
 * @brief Determinate target architecture from the given ID string.
 * @param[in] optarg Architecture ID string (e.g. "avr8").
 * @return Target architecture.
 */
CompilerBase::TargetArch whichArch ( const char * optarg )
{
    if ( false ) {}
  #ifdef MDS_FEATURE_AVR8
    else if ( 0 == strcmp(optarg, "avr8") )
    {
        return CompilerBase::TA_AVR8;
    }
  #endif // MDS_FEATURE_AVR8
  #ifdef MDS_FEATURE_PIC8
    else if ( 0 == strcmp(optarg, "pic8") )
    {
        return CompilerBase::TA_PIC8;
    }
  #endif // MDS_FEATURE_PIC8
  #ifdef MDS_FEATURE_MCS51
    else if ( 0 == strcmp(optarg, "mcs51") )
    {
        return CompilerBase::TA_MCS51;
    }
  #endif // MDS_FEATURE_MCS51
  #ifdef MDS_FEATURE_PICOBLAZE
    else if ( 0 == strcmp(optarg, "PicoBlaze") )
    {
        return CompilerBase::TA_PICOBLAZE;
    }
  #endif // MDS_FEATURE_PICOBLAZE
  #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
    else if ( 0 == strcmp(optarg, "Adaptable") )
    {
        return CompilerBase::TA_ADAPTABLE;
    }
  #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
    else
    {
        std::cerr << QObject::tr("Error: invalid architecture specification `%1'.").arg(optarg).toStdString()
                  << std::endl;
        return CompilerBase::TA_INVALID;
    }
}

/**
 * @brief Determinate source code language from the given ID string.
 * @param[in] optarg Programming language ID string (e.g. "asm").
 * @return Programming language.
 */
CompilerBase::LangId whichLang ( const char * optarg )
{
    if ( 0 == strcmp(optarg, "asm") )
    {
        return CompilerBase::LI_ASM;
    }
  #ifdef MDS_FEATURE_C_COMPILER
    else if ( 0 == strcmp(optarg, "c") )
    {
        return CompilerBase::LI_C;
    }
  #endif // MDS_FEATURE_C_COMPILER
    else
    {
        std::cerr << QObject::tr("Error: invalid language specification `%1'.").arg(optarg).toStdString() << std::endl;
        return CompilerBase::LI_INVALID;
    }
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

    path baseIncludePath;

    // Determine the base include path.
    {
      #ifdef __linux__
        baseIncludePath = canonical("/proc/self/exe");
      #else // __linux__
        char ownPth[MAX_PATH];
        HMODULE hModule = GetModuleHandle(NULL);
        GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
        baseIncludePath = ownPth;
      #endif // __linux__

        baseIncludePath = baseIncludePath.parent_path();

      #ifdef NDEBUG
        baseIncludePath = baseIncludePath.parent_path();
      #endif // NDEBUG

        baseIncludePath = system_complete(baseIncludePath);
        baseIncludePath /= "include";

      #ifdef NDEBUG
        baseIncludePath /= "mds";
      #endif // NDEBUG
    }

    bool silent = false;
    CompilerOptions opts;
    CompilerMsgIntfStdout msgInterface;
    Compiler compiler(&msgInterface, baseIncludePath.string());
    CompilerBase::TargetArch targetArchitecture = CompilerBase::TA_INVALID;
    CompilerBase::LangId targetLanguage = CompilerBase::LI_INVALID;

    if ( 1 == argc )
    {
        std::cerr << QObject::tr("Error: option required.").toStdString() << std::endl;
        printUsage(argv[0]);
        return EXIT_ERROR_CLI;
    }

    // Disable error messages from getopt_long().
    opterr = 0;

    const char * shortopts = ":hVca:p:m:s:l:x:d:D:b:S:t:WERNI:P:D:";
    static const struct option longopts[] =
    {
        { "help",        no_argument,       0,   'h' },
        { "version",     no_argument,       0,   'V' },
        { "check",       no_argument,       0,   'c' },
        { "no-warnings", no_argument,       0,   'W' },
        { "no-errors",   no_argument,       0,   'E' },
        { "no-remarks",  no_argument,       0,   'R' },
        { "silent",      no_argument,       0,   'N' },

        { "no-backup",   no_argument,       0, 0x200 },
        { "brief-msg",   no_argument,       0, 0x201 },
        { "no-strict",   no_argument,       0, 0x202 },

        { "architecture",required_argument, 0,   'a' },
        { "language",    required_argument, 0,   'l' },
        { "mtable",      required_argument, 0,   'm' },
        { "stable",      required_argument, 0,   's' },
        { "hex",         required_argument, 0,   'x' },
        { "debug",       required_argument, 0,   'g' },
        { "device",      required_argument, 0,   'd' },
        { "binary",      required_argument, 0,   'b' },
        { "codetree",    required_argument, 0,   't' },
        { "srec",        required_argument, 0,   'S' },
        { "include",     required_argument, 0,   'I' },
        { "precompile",  required_argument, 0,   'P' },
        { "define",      required_argument, 0,   'D' },

        { "vhdl",        required_argument, 0, 0x100 },
        { "verilog",     required_argument, 0, 0x101 },
        { "vhdl-tmpl",   required_argument, 0, 0x102 },
        { "verilog-tmpl",required_argument, 0, 0x103 },
        { "mem",         required_argument, 0, 0x104 },
        { "cunit",       required_argument, 0, 0x105 },
        { "raw-hex-dump",required_argument, 0, 0x106 },
        { "base-inc-dir",required_argument, 0, 0x107 },
        { "strtable",    required_argument, 0, 0x108 },
        { "secondary",   required_argument, 0, 0x109 },
        { "lst",         required_argument, 0, 0x10A },

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
            case 'c':
                opts.m_syntaxCheckOnly = true;
                break;
            case 'a': // --architecture
                targetArchitecture = whichArch(optarg);
                if ( CompilerBase::TA_INVALID == targetArchitecture )
                {
                    return EXIT_ERROR_CLI;
                }
                else
                {
                    break;
                }
            case 'l': // --language=<language>
                targetLanguage = whichLang(optarg);
                if ( CompilerBase::LI_INVALID == targetLanguage )
                {
                    return EXIT_ERROR_CLI;
                }
                else
                {
                    break;
                }
            case 'm':
                opts.m_macroTable = optarg;
                break;
            case 's':
                opts.m_symbolTable = optarg;
                break;
            case 'x': // --hex
                opts.m_hexFile = optarg;
                break;
            case 'g': // --debug
                opts.m_mdsDebugFile = optarg;
                break;
            case 'd': // --dev=<device_name>
                opts.m_device = optarg;
                break;
            case 'b': // --binary=<file_name>
                opts.m_binFile = optarg;
                break;
            case 'S': // --srec=<file_name>
                opts.m_srecFile = optarg;
                break;
            case 't': // --codetree=<file_name>
                opts.m_codeTree = optarg;
                break;
            case 'W': // --no-warnings
                opts.m_verbosity = CompilerOptions::Verbosity ( opts.m_verbosity & ~(CompilerOptions::V_WARNINGS) );
                break;
            case 'E': // --no-errors
                opts.m_verbosity = CompilerOptions::Verbosity ( opts.m_verbosity & ~(CompilerOptions::V_ERRORS) );
                break;
            case 'R': // --no-remarks
                opts.m_verbosity = CompilerOptions::Verbosity ( opts.m_verbosity & ~(CompilerOptions::V_REMARKS) );
                break;
            case 'N': // --silent
                silent = true;
                opts.m_verbosity = CompilerOptions::Verbosity(0);
                break;
            case 'I': // --include=<dir>
                opts.m_includePathSystem.push_back(optarg);
                break;
            case 'P': // --precompile=<file>
                opts.m_prcTarget = optarg;
                break;
            case 'D': // --define
                opts.m_define.push_back(optarg);
                break;
            case 0x100: // --vhdl=<file>
                opts.m_vhdlFile = optarg;
                break;
            case 0x101: // --verilog=<file>
                opts.m_verilogFile = optarg;
                break;
            case 0x102: // --vhdl-tmpl=<file>
                opts.m_vhdlTemplate = optarg;
                break;
            case 0x103: // --verilog-tmpl=<file>
                opts.m_verilogTemplate = optarg;
                break;
            case 0x104: // --mem=<file>
                opts.m_memFile = optarg;
                break;
            case 0x105: // --cunit=<compilation_unit>
                opts.m_cunit = optarg;
                break;
            case 0x106: // --raw-hex-dump
                opts.m_rawHexDumpFile = optarg;
                break;
            case 0x107: // --base-inc-dir
                compiler.setBaseIncludeDir(optarg);
                break;
            case 0x108: // --strtable
                opts.m_stringTable = optarg;
                break;
            case 0x109: // --secondray
                opts.m_second = optarg;
                break;
            case 0x10A: // --lst
                opts.m_lstFile = optarg;
                break;

            case 0x200: // --no-backup
                opts.m_makeBackupFiles = false;
                break;
            case 0x201: // --brief-msg
                opts.m_briefMsgOutput = true;
                break;
            case 0x202: // --no-strict
                opts.m_strict = false;
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
        opts.m_sourceFiles.push_back(argv[i]);
    }

    if ( false == silent )
    {
        printIntro();
    }

    if ( true == opts.m_sourceFiles.empty() )
    {
        std::cout << QObject::tr("Error: no source files specified.").toStdString() << std::endl;
        return EXIT_ERROR_CLI;
    }

    if ( true == compiler.compile ( targetLanguage, targetArchitecture, &opts ) )
    {
        std::cout << QObject::tr("Compilation successful.").toStdString() << std::endl;
        return EXIT_CODE_SUCCESS;
    }
    else
    {
        std::cerr << QObject::tr("Compilation FAILED!").toStdString() << std::endl;
        return EXIT_ERROR_COMPILER;
    }
}
