// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Compiler
 * @file CompilerOptions.h
 */
// =============================================================================

#ifndef COMPILEROPTIONS_H
#define COMPILEROPTIONS_H

// Standard header files
#include <string>
#include <vector>
#include <ostream>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerOptions
 */
class CompilerOptions
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct ProcessorLimits
        {
            /**
             * @brief
             * Sets all attributes to their default values, in this case it means that no limits will be set.
             *
             * Leaving this structure in its default state causes the compiler to treat the target processor as it has
             * unlimited size of program memory, infinite number of registers, all instructions of its architecture are
             * implemented in hardware, etc.
             */
            ProcessorLimits();

            /// Size of internal program memory in architecture specific words (not octets or bytes).
            int m_iCodeMemSize;

            /// Size of external program memory, only certain architectures use this. Size is in words.
            int m_xCodeMemSize;

            /// Size of internal data memory (for AVR8 it's SRAM, for 8051 it's IDATA, for PicoBlaze it's Scratch Pad...).
            int m_iDataMemSize;

            /// Size of extended data memory, only certain architectures use this.
            int m_eDataMemSize;

            /// Size of external data memory, only certain architectures use this.
            int m_xDataMemSize;

            /// Size of register file, only certain architectures use this.
            int m_regFileSize;

            /// Size of Non Volatile Data Memory, e.g. some data EEPROM; only certain architectures use this.
            int m_nvDataMemSize;

            ///
            std::vector<char> m_disabledInstructions;
        };

        /**
         * @brief
         */
        enum Verbosity
        {
            V_GENERAL  = 0x1, ///<
            V_ERRORS   = 0x2, ///<
            V_WARNINGS = 0x4, ///<
            V_REMARKS  = 0x8  ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerOptions();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         */
        void clearOutputFiles();

        /**
         * @brief
         */
        void normalizeFilePaths();

    ////    Public Attributes    ////
    public:
        /**
         * @name File names.
         *
         * Name of files which the compiler will be instructed to work with, either to read them or to generate them,
         * empty string in case of any of these files means that the file will not be generated or used in any other
         * way. All file names are, of course, case sensitive; and it's advisable to specify them with full path,
         * either relative or absolute. Recommended directory separator character is `/' - slash, not `\' - backslash;
         * the reason for this is to avoid possible compatibility issues across different operating systems.
         */
        //@{
            /// File(s) containing program source code to compile.
            std::vector<std::string> m_sourceFiles;

            /// File in which table of symbols will be stored.
            std::string m_symbolTable;

            /// File in which table of declared and/or defined macros will be stored.
            std::string m_macroTable;

            /// File where to store MDS native debug data.
            std::string m_mdsDebugFile;

            /// Tree of code as generated by syntax analyzer will be stored here, useful for compiler debugging.
            std::string m_codeTree;

            /// Code listing will be stored here.
            std::string m_lstFile;

            /// Absolute machine code of the compiled code will be stored here as 8-bit Intel Hex.
            std::string m_hexFile;

            /// Absolute machine code of the compiled code will be stored here as raw binary data.
            std::string m_binFile;

            /// Absolute machine code of the compiled code will be stored here as Motorola S-record file.
            std::string m_srecFile;

            /// The compiled code as file in Verilog hardware definition language.
            std::string m_verilogFile;

            /// The compiled code as file in VHDL hardware definition language.
            std::string m_vhdlFile;

            ///
            std::string m_memFile;

            ///
            std::string m_verilogTemplate;

            ///
            std::string m_vhdlTemplate;

            /// When this string is set to a file name, compiler will generate a precopiled code and store it the file.
            std::string m_prcTarget;

            /// Write output from C preprocessor in this file.
            std::string m_cunit;

            /// Second output (target architecture specific).
            std::string m_second;
        //@}

        /**
         * @name Other compilation and code generation options.
         */
        //@{
            ///
            std::string m_device;

            ///
            std::vector<std::string> m_includePath;

            ///
            ProcessorLimits m_processorlimits;

            /// Maximum allowed macro expansion level, -1 means unlimited.
            int m_maxMacroExp;

            /// Maximum allowed source file inclusion level, -1 means unlimited.
            int m_maxInclusion;

            /// Maximum allowed size of Intel HEX record, in bytes of data field (1-255).
            int m_hexMaxRecLength;

            ///
            int m_messageLimit;

            ///
            Verbosity m_verbosity;

            /// Do not compile the code, only check whether it's syntactically correct.
            bool m_syntaxCheckOnly;

            /// Make backup copy of every file which would be overwritten by direct output from the compiler.
            bool m_makeBackupFiles;

            ///
            bool m_briefMsgOutput;

            /// Enables additional warning and/or error messages.
            bool m_strict;
        //@}
};

/// @name Tracing operators.
//@{
    /**
     * @brief Tracing operator for ProcessorLimits.
     * @param[in,out] out
     * @param[in] limits
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerOptions::ProcessorLimits & limits );

    /**
     * @brief Tracing operator for CompilerOptions.
     * @param[in,out] out
     * @param[in] opts
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerOptions * opts );


    /// @overload
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerOptions & opts );
//@}

#endif // COMPILEROPTIONS_H
