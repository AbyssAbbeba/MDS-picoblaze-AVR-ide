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

// MScript language interpreter header files.
#include "MScriptStmtTypes.h"
#include "MScriptStatement.h"
#include "MScriptExpr.h"
#include "MScriptSrcLocation.h"
#include "MScriptBase.h"

// Standard header files.
#include <string>

// Used for i18n only.
#include <QObject>

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

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        virtual void parserMessage ( MScriptSrcLocation location,
                                     MScriptBase::MessageType type,
                                     const std::string & text ) = 0;

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        virtual void lexerMessage ( MScriptSrcLocation location,
                                    MScriptBase::MessageType type,
                                    const std::string & text ) = 0;
};

#endif // MSCRIPTPARSERINTERFACE_H
