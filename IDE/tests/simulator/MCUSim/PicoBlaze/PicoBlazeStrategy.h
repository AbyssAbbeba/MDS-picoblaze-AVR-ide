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
 * @ingroup testsMCUSimPicoBlaze
 * @file PicoBlazeStrategy.h
 */
// =============================================================================

#ifndef PICOBLAZESTRATEGY_H
#define PICOBLAZESTRATEGY_H

#include "../MCUSimTestScriptStrategy.h"

/**
 * @brief
 * @class MCUSimTestScriptStrategy
 * @ingroup testsMCUSimPicoBlaze
 */
class PicoBlazeStrategy : public MCUSimTestScriptStrategy
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
         * @param[in] line
         * @param[in] useAsmFile
         * @param[out] cmd
         * @return
         */
        virtual bool processLine ( const char * line,
                                   bool useAsmFile,
                                   MCUSimTestScript::Command * cmd );

        /**
         * @brief
         * @param[in] cmd
         * @param[in,out] outFile
         * @return
         */
        virtual bool executeCommand ( const MCUSimTestScript::Command & cmd,
                                      std::ofstream & outFile );
};

#endif // PICOBLAZESTRATEGY_H
