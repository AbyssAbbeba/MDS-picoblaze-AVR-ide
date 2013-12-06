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
 * @ingroup MoraviaScript
 * @file MScriptStrategyInterface.h
 */
// =============================================================================

#ifndef MSCRIPTSTRATEGYINTERFACE_H
#define MSCRIPTSTRATEGYINTERFACE_H

// Forward declarations.
class MScriptVarTable;
class MScriptFuncTable;
class MScriptNamespaces;

// MScript language interpreter header files.
#include "MScriptBase.h"

/**
 * @brief
 * @class MScriptStrategyInterface
 * @ingroup MoraviaScript
 */
class MScriptStrategyInterface
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        virtual MScriptFuncTable * getFuncTbl() = 0;

        /**
         * @brief
         * @return
         */
        virtual MScriptVarTable * getVarTbl() = 0;

        /**
         * @brief
         * @return
         */
        virtual MScriptNamespaces * getNs() = 0;

        /**
         * @brief
         * @param[in] type
         * @param[in] text
         */
        virtual void strategyMessage ( MScriptBase::MessageType type,
                                       const std::string & text ) = 0;
};

#endif // MSCRIPTSTRATEGYINTERFACE_H
