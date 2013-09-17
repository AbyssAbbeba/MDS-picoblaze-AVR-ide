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
 * @file MCUSimTestScriptStrategy.h
 */
// =============================================================================

#ifndef MCUSIMTESTSCRIPTSTRATEGY_H
#define MCUSIMTESTSCRIPTSTRATEGY_H

// Forward declarations.
class MCUSim;

#include "MCUSimTestScript.h"

// Standard headers.
#include <string>
#include <fstream>

/**
 * @brief
 * @class MCUSimTestScriptStrategy
 * @ingroup testsMCUSimPicoBlaze
 */
class MCUSimTestScriptStrategy
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum CommandTypeExt
        {
            CTE_XXX = MCUSimTestScript::CT__MAX__, ///<

            CTE__MAX__                  ///<
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] simulator
         * @param[in,out] execMessage
         * @param[in,out] success
         */
        void link ( MCUSim * simulator,
                    std::string * execMessage,
                    bool * success );

        /**
         * @brief
         * @param[in] line
         * @param[in] useAsmFile
         * @param[out] cmd
         * @return
         */
        virtual bool processLine ( const char * line,
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

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        MCUSim * m_simulator;

        /**
         * @brief
         */
        std::string & m_execMessage;

        /**
         * @brief
         */
        bool & m_success;
};

#endif // MCUSIMTESTSCRIPTSTRATEGY_H
