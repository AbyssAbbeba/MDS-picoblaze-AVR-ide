// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup SimCmdLine
 * @file SimCmdLineBase.h
 */
// =============================================================================

#ifndef SIMCMDLINEBASE_H
#define SIMCMDLINEBASE_H

// Standard headers.
#include <map>
#include <string>

/**
 * @brief
 * @ingroup SimCmdLine
 * @class SimCmdLineBase
 */
class SimCmdLineBase
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum ExitCode
        {
            EXIT_CODE_SUCCESS    = 0, ///<
            EXIT_ERROR_SIMULATOR = 1, ///<
            EXIT_ERROR_CLI       = 2  ///<
        };

    ////    Protected Datatypes    ////
    protected:
        /**
         * @brief
         */
        enum FileType
        {
            FT_INTEL_HEX, ///<
            FT_RAW_HEX,   ///<
            FT_RAW_BIN,   ///<
            FT_S_REC,     ///<
            FT_MEM,       ///<
            FT_VERILOG,   ///<
            FT_VHDL       ///<
        };

    ////    Protected Static Constants    ////
    protected:
        ///
        static const std::map<std::string, FileType> S_FILE_TYPE;
};

#endif // SIMCMDLINEBASE_H
