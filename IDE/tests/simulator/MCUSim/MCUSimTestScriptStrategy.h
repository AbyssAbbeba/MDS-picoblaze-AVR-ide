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
 * @ingroup testsMCUSim
 * @file MCUSimTestScriptStrategy.h
 */
// =============================================================================

#ifndef MCUSIMTESTSCRIPTSTRATEGY_H
#define MCUSIMTESTSCRIPTSTRATEGY_H

// Forward declarations.
class MCUSim;

#include "MCUSimTestScript.h"

// Standard headers.
#include <cctype>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>

/**
 * @brief
 * @class MCUSimTestScriptStrategy
 * @ingroup testsMCUSim
 */
class MCUSimTestScriptStrategy
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        virtual ~MCUSimTestScriptStrategy() {}

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] simulator
         * @param[in,out] execMessage
         * @param[in,out] success
         * @param[in,out] anyAssertionMade
         */
        void link ( MCUSim * simulator,
                    std::string * execMessage,
                    bool * success,
                    bool * anyAssertionMade );

        /**
         * @brief
         * @param[in,out] tokens
         * @param[in] useAsmFile
         * @param[out] cmd
         * @return
         */
        virtual bool processLine ( std::vector<std::string> * tokens,
                                   bool useAsmFile,
                                   MCUSimTestScript::Command * cmd ) = 0;

        /**
         * @brief
         * @param[in] cmd
         * @param[in,out] outFile
         * @return
         */
        virtual bool executeCommand ( const MCUSimTestScript::Command & cmd,
                                      std::ofstream & outFile ) = 0;

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        MCUSim * m_simulator;

        /**
         * @brief
         */
        std::string * m_execMessage;

        /**
         * @brief
         */
        bool * m_success;

        /**
         * @brief
         */
        bool * m_anyAssertionMade;
};

#endif // MCUSIMTESTSCRIPTSTRATEGY_H
