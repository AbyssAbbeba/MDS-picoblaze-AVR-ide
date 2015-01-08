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

// Standard headers.
#include <map>

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
            FID_Z,      ///< Zero
            FID_C,      ///< Carry
            FID_PZ,     ///< Previous Zero
            FID_PC,     ///< Previous Carry
            FID_IE,     ///< Interrupt Enable
            FID_I,      ///< Interrupted
            FID_OV,     ///< Overflow
            FID_N,      ///< Negative
            FID_HC,     ///< Half Carry
            FID_P,      ///< Parity
            FID_F,      ///< Flag
            FID_POV,    ///< Previous Overflow
            FID_PN,     ///< Previous Negative
            FID_PHC,    ///< Previous Half Carry
            FID_PP      ///< Previous Parity
        };

    ////    Private Static Constants    ////
    private:
        /// @brief
        static const std::map<std::string, FlagID> PROCESSOR_FLAGS;

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
