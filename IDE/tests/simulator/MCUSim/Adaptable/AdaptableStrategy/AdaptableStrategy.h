// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsMCUSimAdaptable
 * @file AdaptableStrategy.h
 */
// =============================================================================

#ifndef ADAPTABLESTRATEGY_H
#define ADAPTABLESTRATEGY_H

#include "../../MCUSimTestScriptStrategy.h"

/**
 * @brief
 * @class MCUSimTestScriptStrategy
 * @ingroup testsMCUSimAdaptable
 */
class AdaptableStrategy : public MCUSimTestScriptStrategy
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum CommandTypeExt
        {
            CTE_FLAG_EQ = MCUSimTestScript::CT__MAX__,  ///<
            CTE_FLAG_NE,                                ///<
            CTE_INTERRUPT,                              ///<

            CTE__MAX__                                  ///<
        };

        /**
         * @brief
         */
        enum FlagID
        {
            FID_Z,  ///<
            FID_C,  ///<
            FID_PZ, ///<
            FID_PC, ///<
            FID_IE, ///<
            FID_I   ///<
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] tokens
         * @param[in] useAsmFile
         * @param[out] cmd
         * @return
         */
        virtual bool processLine ( std::vector<std::string> * tokens,
                                   bool useAsmFile,
                                   MCUSimTestScript::Command * cmd ) override;

        /**
         * @brief
         * @param[in] cmd
         * @param[in,out] outFile
         * @return
         */
        virtual bool executeCommand ( const MCUSimTestScript::Command & cmd,
                                      std::ofstream & outFile ) override;
};

#endif // ADAPTABLESTRATEGY_H
