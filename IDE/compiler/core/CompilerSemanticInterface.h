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
 * @ingroup Compiler
 * @file CompilerSemanticInterface.h
 */
// =============================================================================

#ifndef COMPILERSEMANTICINTERFACE_H
#define COMPILERSEMANTICINTERFACE_H

// Forward declarations.
class CompilerMsgObserver;

// Standard headers.
#include <string>
#include <vector>
#include <cstdio>

// Include basic general compiler declarations and definitions.
#include "CompilerBase.h"

// Include compiler components used by parsers and/or lexers.
#include "CompilerExpr.h"
#include "CompilerStatement.h"
#include "CompilerStatementTypes.h"

// Used for i18n only.
#include <QObject>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerSemanticInterface
 */
class CompilerSemanticInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerSemanticInterface() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        virtual const std::vector<std::string> & listSourceFiles() const = 0;

        /**
         * @brief
         * @param[in] fileNumber
         * @return
         */
        virtual const std::string & getFileName ( int fileNumber ) const = 0;

        /**
         * @brief
         * @param[in] location
         * @return
         */
        virtual std::string locationToStr ( const CompilerBase::SourceLocation & location ) const = 0;

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        virtual void compilerMessage ( CompilerBase::SourceLocation location,
                                       CompilerBase::MessageType type,
                                       const std::string & text ) = 0;

        /**
         * @brief
         * @param[in] type
         * @param[in] text
         */
        virtual void compilerMessage ( CompilerBase::MessageType type,
                                       const std::string & text ) = 0;

        /**
         * @brief
         * @return
         */
        virtual bool successful() const = 0;

        /**
         * @brief
         * @param[in,out] observer
         * @return
         */
        virtual void registerMsgObserver ( CompilerMsgObserver * observer ) = 0;
};

#endif // COMPILERSEMANTICINTERFACE_H
