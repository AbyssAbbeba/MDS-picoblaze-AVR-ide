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
 * @file MScriptParserInterface.h
 */
// =============================================================================

#ifndef MSCRIPTPARSERINTERFACE_H
#define MSCRIPTPARSERINTERFACE_H

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptExpr.h"
#include "MScriptStmtTypes.h"
#include "MScriptStatement.h"
#include "MScriptSrcLocation.h"

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
        virtual void parserMessage ( const MScriptSrcLocation & location,
                                     MScriptBase::MessageType type,
                                     const std::string & text ) = 0;

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        virtual void lexerMessage ( const MScriptSrcLocation & location,
                                    MScriptBase::MessageType type,
                                    const std::string & text ) = 0;

    ////    Public Attributes    ////
    public:
        /// Size of the string buffer, used for scanning strings and character literals.
        int m_strMaxSize;
};

#endif // MSCRIPTPARSERINTERFACE_H
