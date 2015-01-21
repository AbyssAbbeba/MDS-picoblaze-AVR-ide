// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup SimCmdLine
 * @file SimCmdLineBase.cxx
 */
// =============================================================================

#include "SimCmdLineBase.h"

const std::map<std::string, SimCmdLineBase::FileType> SimCmdLineBase::S_FILE_TYPE =
{
    { "hex",    FT_INTEL_HEX    },
    { "rawhex", FT_RAW_HEX      },
    { "bin",    FT_RAW_BIN      },
    { "srec",   FT_S_REC        },
    { "mem",    FT_MEM          },
    { "v",      FT_VERILOG      },
    { "vhdl",   FT_VHDL         }
};
