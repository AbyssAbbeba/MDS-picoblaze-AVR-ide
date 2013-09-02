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
 * @ingroup testsMCUSimPicoBlaze
 * @file MCUSimTestScript.h
 */
// =============================================================================

#ifndef MCUSIMTESTSCRIPT_H
#define MCUSIMTESTSCRIPT_H

// Forward declarations.
class MCUSim;

// Standard headers.
#include <string>
#include <vector>

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
 * @namespace MCUSimTestScript
 * @ingroup testsMCUSimPicoBlaze
 */
class MCUSimTestScript
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int MAX_LINE_LENGTH = 5120;

    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum CmdType
        {
            CT_ABORT,
            CT_EMPTY,   ///< 0 arguments.
            CT_STEP,    ///< 0 arguments.
            CT_PC_EQ,   ///< 3 arguments: (value).
            CT_PC_NE,   ///< 3 arguments: (value).
            CT_EQ,      ///< 3 arguments: (memory space, address, value).
            CT_NE,      ///< 3 arguments: (memory space, address, value).
        };

        /**
         * @brief
         */
        struct Command
        {
            CmdType m_type;
            std::vector<int> m_args;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] simulator
         */
        MCUSimTestScript ( MCUSim * simulator );

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
         * @return
         */
        bool runScript ( const std::string & inFileName,
                         const std::string & outFileName );

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
         * @return
         */
        inline Command processLine ( const char * line );

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
        bool m_success;

        /**
         * @brief
         */
        std::string m_execMessage;
};

#endif // MCUSIMTESTSCRIPT_H
