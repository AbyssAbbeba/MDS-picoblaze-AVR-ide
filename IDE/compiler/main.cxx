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
 * @ingroup Compiler
 * @file main.cxx
 */
// =============================================================================

// Common compiler header files.
#include "Compiler.h"
#include "CompilerMsgIntfStdout.h"

// getopt_long() function
#include <getopt.h>

// Standard headers
#include <iostream>
#include <cstring>

// Used for i18n only.
#include <QObject>

/**
 * @brief Program version string.
 * @note Be careful with this value, it might be expected in "[0-9]+\.[0-9]+(\.[0-9]+)?" format.
 */
static const char * VERSION = "1.0";

/// @brief Program exit codes.
enum ExitCode
{
    EXIT_CODE_SUCCESS   = 0, ///< Everything went smoothly and compilation was successful.
    EXIT_ERROR_COMPILER = 1, ///< Compiler attempted to execute the given task but for some reason without success.
    EXIT_ERROR_CLI      = 2  ///< Command line interface did not understand the task, compiler was not even started.
};

/**
 * @brief Print help message, a short introduction how to use this program.
 * @param[in] executable Name of executable binary from which this program was launched.
 * @note There is also a hidden option "--codetree \<code tree file\>" which is not mentioned in this help message.
 */
void printHelp ( const char * executable )
{
    std::cout << QObject::tr("Usage:").toStdString() << std::endl
              << QObject::tr("    %1 <OPTIONS>").arg(executable).toStdString() << std::endl
              << std::endl;

    std::cout << QObject::tr("Available program options:").toStdString() << std::endl
              << QObject::tr("    -a, --arch <architecture>").toStdString() << std::endl
              << QObject::tr("        Specify target architecture, supported architectures are:")
                            .toStdString() << std::endl
              << QObject::tr("            - avr8      : 8-bit AVR,").toStdString() << std::endl
              << QObject::tr("            - pic8      : 8-bit PIC,").toStdString() << std::endl
              << QObject::tr("            - mcs51     : MCS-51,").toStdString() << std::endl
              << QObject::tr("            - PicoBlaze : (K)constant Coded Programmable State Machine.")
                            .toStdString() << std::endl
              << QObject::tr("    -p, --plang <programming language>").toStdString() << std::endl
              << QObject::tr("        Specify programming language, supported languages are:").toStdString()<<std::endl
              << QObject::tr("            - asm : assembly language.").toStdString() << std::endl
              << QObject::tr("    -f, --file <source file>").toStdString() << std::endl
              << QObject::tr("        Specify input file containing source code to compile.").toStdString() << std::endl
              << QObject::tr("    -x, --hex <Intel HEX file>").toStdString() << std::endl
              << QObject::tr("        Specify output file with machine code generated as a result of compilation,")
                            .toStdString() << std::endl
              << QObject::tr("        data will be stored in Intel 8 Hex format.").toStdString() << std::endl
              << QObject::tr("    -d, --dbg <MDS native debug file>").toStdString() << std::endl
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
              << QObject::tr("    -l, --lst <code listing>").toStdString() << std::endl
              << QObject::tr("        Specify output file where code listing generated during compilation will be "
                             "stored.").toStdString() << std::endl
              << QObject::tr("    -m, --mtable <table of macros>").toStdString() << std::endl
              << QObject::tr("        Specify file in which the compiler will put table of macros defined in your "
                             "code.").toStdString() << std::endl
              << QObject::tr("    -s, --stable <table of symbols>").toStdString() << std::endl
              << QObject::tr("        Specify file in which the compiler will put table of symbols defined in your "
                             "code.").toStdString() << std::endl
              << QObject::tr("    -h, --help").toStdString() << std::endl
              << QObject::tr("        (Print this message.)").toStdString() << std::endl
              << QObject::tr("    -V, --version").toStdString() << std::endl
              << QObject::tr("        Print compiler version and exit.").toStdString() << std::endl
              << QObject::tr("    -c, --check").toStdString() << std::endl
              << QObject::tr("        Do not perform the actual compilation, do only lexical and syntax analysis of"
                             " the").toStdString() << std::endl
              << QObject::tr("        the provided source code and exit.").toStdString() << std::endl
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
              << QObject::tr("    -D, --dev <device>").toStdString() << std::endl
              << QObject::tr("        Specify exact target device.").toStdString() << std::endl
              << QObject::tr("    -P, --precompile <.prc file>").toStdString() << std::endl
              << QObject::tr("        Specify target file for generation of precompiled code.")
                            .toStdString() << std::endl
              << std::endl;

    std::cout << QObject::tr("Examples:").toStdString() << std::endl
              << QObject::tr("  %1 --file=abc.asm --arch=mcs51 --plang=asm --hex=abc.hex")
                            .arg(executable).toStdString() << std::endl
              << QObject::tr("      Compile source code file `abc.asm' (--file=abc.asm) for architecture MCS-51")
                            .toStdString() << std::endl
              << QObject::tr("      (--arch=mcs51) written in assembly language (--plang=asm), and create file")
                            .toStdString() << std::endl
              << QObject::tr("      `abc.hex' containing machine code generated generated by the compiler.")
                            .toStdString() << std::endl
              << QObject::tr("  %1 --file abc.asm --plang asm --arch mcs51 --hex abc.hex")
                            .arg(executable).toStdString() << std::endl
              << QObject::tr("      Do the same at the above, only in this case we have used another variant")
                            .toStdString() << std::endl
              << QObject::tr("      of usage of switches with argument.").toStdString() << std::endl
              << QObject::tr("  %1 -f abc.asm -p asm -a mcs51 -x abc.hex").arg(executable).toStdString() << std::endl
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
    std::cout << "Type: `" << executable << " -h'" << QObject::tr(" for help.").toStdString() << std::endl;
}

/**
 * @brief Determinate target architecture from the given ID string.
 * @param[in] optarg Architecture ID string (e.g. "avr8").
 * @return Target architecture.
 */
CompilerBase::TargetArch whichArch ( const char * optarg )
{
    if ( 0 == strcmp(optarg, "avr8") )
    {
        return CompilerBase::TA_AVR8;
    }
    else if ( 0 == strcmp(optarg, "pic8") )
    {
        return CompilerBase::TA_PIC8;
    }
    else if ( 0 == strcmp(optarg, "mcs51") )
    {
        return CompilerBase::TA_MCS51;
    }
    else if ( 0 == strcmp(optarg, "PicoBlaze") )
    {
        return CompilerBase::TA_KCPSM3;
    }
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
    CompilerOptions opts;
    CompilerMsgIntfStdout msgInterface;
    Compiler compiler(&msgInterface, "include");
    CompilerBase::TargetArch targetArchitecture = CompilerBase::TA_INVALID;
    CompilerBase::LangId targetLanguage = CompilerBase::LI_INVALID;

    std::cout << QObject::tr("MDS mutitarget macro-assembler v%1").arg(VERSION).toStdString() << std::endl
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

    const char * shortopts = ":hVca:p:f:m:s:l:x:d:D:b:S:t:WERNI:P:";
    static const struct option longopts[] =
    {
        { "help",        no_argument,       0,   'h' },
        { "version",     no_argument,       0,   'V' },
        { "check",       no_argument,       0,   'c' },
        { "no-warnings", no_argument,       0,   'W' },
        { "no-errors",   no_argument,       0,   'E' },
        { "no-remarks",  no_argument,       0,   'R' },
        { "silent",      no_argument,       0,   'N' },

        { "arch",        required_argument, 0,   'a' },
        { "plang",       required_argument, 0,   'p' },
        { "file",        required_argument, 0,   'f' },
        { "mtable",      required_argument, 0,   'm' },
        { "stable",      required_argument, 0,   's' },
        { "lst",         required_argument, 0,   'l' },
        { "hex",         required_argument, 0,   'x' },
        { "dbg",         required_argument, 0,   'd' },
        { "dev",         required_argument, 0,   'D' },
        { "binary",      required_argument, 0,   'b' },
        { "codetree",    required_argument, 0,   't' },
        { "srec",        required_argument, 0,   'S' },
        { "include",     required_argument, 0,   'I' },
        { "precompile",  required_argument, 0,   'P' },

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
            case 'a':
                targetArchitecture = whichArch(optarg);
                if ( CompilerBase::TA_INVALID == targetArchitecture )
                {
                    return EXIT_ERROR_CLI;
                }
                else
                {
                    break;
                }
            case 'p':
                targetLanguage = whichLang(optarg);
                if ( CompilerBase::LI_INVALID == targetLanguage )
                {
                    return EXIT_ERROR_CLI;
                }
                else
                {
                    break;
                }
            case 'f':
                opts.m_sourceFile = optarg;
                break;
            case 'm':
                opts.m_macroTable = optarg;
                break;
            case 's':
                opts.m_symbolTable = optarg;
                break;
            case 'l':
                opts.m_lstFile = optarg;
                break;
            case 'x':
                opts.m_hexFile = optarg;
                break;
            case 'd':
                opts.m_mdsDebugFile = optarg;
                break;
            case 'D': // --dev=<device_name>
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
                opts.m_verbosity = CompilerOptions::Verbosity(0);
                break;
            case 'I': // --include=<dir>
                opts.m_includePath.push_back(optarg);
                break;
            case 'P': // --precompile=<file>
                opts.m_prcTarget = optarg;
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
