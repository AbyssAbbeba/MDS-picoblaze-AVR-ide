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
 * @ingroup Compiler
 * @file CompilerSemanticInterface.h
 */
// =============================================================================

#ifndef COMPILERSEMANTICINTERFACE_H
#define COMPILERSEMANTICINTERFACE_H

// Forward declarations.
class CompilerMsgObserver;
class DbgFile;
class DataFile;

// Standard headers.
#include <string>
#include <vector>
#include <cstdio>

// Include basic general compiler declarations and definitions.
#include "CompilerBase.h"

// Include compiler components used by semantic analyzers..
#include "CompilerExpr.h"
#include "CompilerStatement.h"
#include "CompilerStatementTypes.h"
#include "CompilerSourceLocation.h"
#include "CompilerLocationTracker.h"

// Used for i18n only.
#include <QObject>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerSemanticInterface
 */
class CompilerSemanticInterface
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct SimulatorData
        {
            ///
            bool m_genSimData;

            ///
            DbgFile * m_simDbg;

            ///
            DataFile * m_simData;
        };

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
        virtual const std::vector<std::pair<std::string,FILE*>> & listSourceFiles() const = 0;

        /**
         * @brief
         * @param[in] fileNumber
         * @return
         */
        virtual const std::string & getFileName ( int fileNumber ) const = 0;

        /**
         * @brief
         * @param[in] location
         * @param[in] colon
         * @return
         */
        virtual std::string locationToStr ( const CompilerSourceLocation & location,
                                            bool colon = false ) const = 0;

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         * @param[in] forceAsUnique
         */
        virtual void semanticMessage ( const CompilerSourceLocation & location,
                                       CompilerBase::MessageType type,
                                       const std::string & text,
                                       bool forceAsUnique = false ) = 0;

        /**
         * @brief
         * @return
         */
        virtual CompilerLocationTracker & locationTrack() = 0;

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

        /**
         * @brief
         * @param[in] deviceName
         * @param[in] flag
         * @return
         */
        virtual CompilerStatement * loadDevSpecCode ( const std::string & deviceName,
                                                      CompilerBase::DevSpecLoaderFlag * flag = nullptr ) = 0;

        /**
         * @brief
         * @return
         */
        virtual std::string getBaseIncludeDir() = 0;

        /**
         * @brief
         * @return
         */
        virtual std::string getBaseName() = 0;

    ////    Public Attributes    ////
    public:
        ///
        SimulatorData m_simulatorData;
};

#endif // COMPILERSEMANTICINTERFACE_H
