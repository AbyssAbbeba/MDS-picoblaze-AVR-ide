// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsMCUSim
 * @file MCUSimTestScript.h
 */
// =============================================================================

#ifndef MCUSIMTESTSCRIPT_H
#define MCUSIMTESTSCRIPT_H

// Forward declarations.
class MCUSim;
class MCUSimTestScriptStrategy;

// Standard headers.
#include <string>
#include <vector>
#include <fstream>

/*
# comment

STEP
PC == 1
CODE[3] == '0'          # ascii
DATA[1] == 125          # dec.
EEPROM[1] != 0x4        # hex.
STACK[9] != 050         # oct.
REG[0] == 0b10          # bin.

STEP
PC == 2

STEP
PC == 158

*/


/**
 * @brief
 * @class MCUSimTestScript
 * @ingroup testsMCUSim
 */
class MCUSimTestScript
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int MAX_LINE_LENGTH = 5120;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum CommandType
        {
            CT_UNKNOWN,
            CT_ABORT,
            CT_EMPTY,   ///< 0 arguments.
            CT_STEP,    ///< 0 arguments.
            CT_PC_EQ,   ///< 3 arguments: (value).
            CT_PC_NE,   ///< 3 arguments: (value).
            CT_EQ,      ///< 3 arguments: (memory space, address, value).
            CT_NE,      ///< 3 arguments: (memory space, address, value).

            CT__MAX__   ///<
        };

        /**
         * @brief
         */
        struct Command
        {
            CommandType m_type;
            std::vector<int> m_args;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] simulator
         * @param[in,out] strategy
         */
        MCUSimTestScript ( MCUSim * simulator,
                           MCUSimTestScriptStrategy * strategy = NULL );

        /**
         * @brief
         */
        ~MCUSimTestScript();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] inFileName
         * @param[in] outFileName
         * @param[in] useAsmFile
         * @return
         */
        bool runScript ( const std::string & inFileName,
                         const std::string & outFileName,
                         bool useAsmFile = false );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] token
         * @return
         */
        bool checkNumber ( const std::string & token ) const;

        /**
         * @brief
         * @param[in] token
         * @return
         */
        int str2num ( const std::string & token ) const;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] line
         * @param[in] useAsmFile
         * @return
         */
        inline Command processLine ( const char * line,
                                     bool useAsmFile );

        /**
         * @brief
         * @param[in] cmd
         * @param[in,out] outFile
         * @return
         */
        inline bool executeCommand ( const Command & cmd,
                                     std::ofstream & outFile );

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        MCUSim * const m_simulator;

        /**
         * @brief
         */
        MCUSimTestScriptStrategy * const m_strategy;

        /**
         * @brief
         */
        std::string m_execMessage;

        /**
         * @brief
         */
        bool m_success;
};

#endif // MCUSIMTESTSCRIPT_H
