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
 * @ingroup MoraviaScript
 * @file MScriptInterpretInterface.h
 */
// =============================================================================

#ifndef MSCRIPTINTERPRETINTERFACE_H
#define MSCRIPTINTERPRETINTERFACE_H

// Forward declarations.
class MScriptStrategy;
class MScriptStatement;

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptSrcLocation.h"

// Standard header files.
#include <string>

/**
 * @brief
 * @class MScriptInterpretInterface
 * @ingroup MoraviaScript
 */
class MScriptInterpretInterface
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        virtual void interpreterMessage ( MScriptSrcLocation location,
                                          MScriptBase::MessageType type,
                                          const std::string & text ) = 0;
};

#endif // MSCRIPTINTERPRETINTERFACE_H