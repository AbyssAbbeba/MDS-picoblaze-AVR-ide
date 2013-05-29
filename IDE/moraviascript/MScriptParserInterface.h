// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup MoraviaScript
 * @file MScriptParserInterface.h
 */
// =============================================================================

#ifndef MSCRIPTPARSERINTERFACE_H
#define MSCRIPTPARSERINTERFACE_H

#include "MScriptStmtTypes.h"
#include "MScriptStatement.h"
#include "MScriptSrcLocation.h"

/**
 * @brief
 * @class MScriptParserInterface
 * @ingroup MoraviaScript
 */
class MScriptParserInterface
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        virtual void syntaxAnalysisComplete ( MScriptStatement * codeTree ) = 0;
};

#endif // MSCRIPTPARSERINTERFACE_H
