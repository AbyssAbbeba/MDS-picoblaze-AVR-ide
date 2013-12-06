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
 * @file MScriptCore.h
 */
// =============================================================================

#ifndef MSCRIPTCORE_H
#define MSCRIPTCORE_H

// Forward declarations.
class MScriptExpr;
class MScriptStrategy;
class MScriptVarTable;
class MScriptFuncTable;
class MScriptStatement;

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptInterpret.h"
#include "MScriptParserInterface.h"
#include "MScriptStrategyInterface.h"

// Standard header files.
#include <vector>
#include <string>
#include <cstdio>

/**
 * @brief
 * @class MScriptCore
 * @ingroup MoraviaScript
 */
class MScriptCore : public MScriptInterpret,
                    protected MScriptBase,
                    protected MScriptParserInterface,
                    protected MScriptStrategyInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] strategy
         * @param[in] scriptCode
         */
        MScriptCore ( MScriptStrategy * strategy,
                      const std::string & scriptCode = "" );

        /**
         * @brief
         * @param[in,out] strategy
         * @param[in,out] sourceFile
         */
        MScriptCore ( MScriptStrategy * strategy,
                      FILE * sourceFile,
                      const std::string & fileName );

        /**
         * @brief
         */
        virtual ~MScriptCore();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] scriptCode
         * @return
         */
        bool loadScript ( const std::string & scriptCode );

        /**
         * @brief
         * @param[in,out] sourceFile
         * @param[in] fileName
         * @return
         */
        bool loadScript ( FILE * sourceFile,
                          const std::string & fileName );

        /**
         * @brief
         */
        void unloadScript();

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] file
         * @return
         */
        bool loadFile ( FILE * file );

        /**
         * @brief
         * @param[in] scriptCode
         * @return
         */
        bool loadCode ( const std::string & scriptCode );

        /**
         * @brief
         * @param[in,out] code
         * @param[in] fileNumber
         * @return
         */
        void rewriteFileNumbers ( MScriptStatement * code,
                                  int fileNumber ) const;

        /**
         * @brief
         * @param[in,out] expr
         * @param[in] fileNumber
         * @return
         */
        void rewriteFileNumbers ( MScriptExpr * expr,
                                  int fileNumber ) const;

        /**
         * @brief
         * @param[in,out] code
         * @param[in] location
         * @return
         */
        void locationRelativeTo ( MScriptStatement * code,
                                  const MScriptSrcLocation & location );

        /**
         * @brief
         * @param[in,out] expr
         * @param[in] location
         * @return
         */
        void locationRelativeTo ( MScriptExpr * expr,
                                  const MScriptSrcLocation & location );

        /**
         * @brief
         * @param[in] fileName
         * @return
         */
        int getFileNumber ( const std::string & fileName );

        /// @name Operations declared in parser interface.
        //@{
            /**
             * @brief
             * @param[in,out] codeTree
             */
            virtual void syntaxAnalysisComplete ( MScriptStatement * codeTree );

            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             */
            virtual void parserMessage ( const MScriptSrcLocation & location,
                                         MScriptBase::MessageType type,
                                         const std::string & text );

            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             */
            virtual void lexerMessage ( const MScriptSrcLocation & location,
                                        MScriptBase::MessageType type,
                                        const std::string & text );
        //@}

        /// @name Operations declared in interpret interface.
        //@{
            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             */
            virtual void interpreterMessage ( const MScriptSrcLocation & location,
                                              MScriptBase::MessageType type,
                                              const std::string & text );

            /**
             * @brief
             * @param[in] location
             * @param[in] fileName
             * @return
             */
            virtual MScriptStatement * include ( const MScriptSrcLocation & location,
                                                 const std::string & fileName );

            /**
             * @brief
             * @param[in] location
             * @param[in] code
             * @return
             */
            virtual MScriptStatement * insertCode ( const MScriptSrcLocation & location,
                                                    const std::string & code );

            /**
             * @brief
             * @return
             */
            virtual MScriptBase * getCoreBase();

            /**
             * @brief
             * @return
             */
            virtual MScriptStrategy * getStrategy();
        //@}

        /// @name Operations declared in strategy interface.
        //@{
            /**
             * @brief
             * @return
             */
            virtual MScriptFuncTable * getFuncTbl();

            /**
             * @brief
             * @return
             */
            virtual MScriptVarTable * getVarTbl();

            /**
             * @brief
             * @return
             */
            virtual MScriptNamespaces * getNs();

            /**
             * @brief
             * @param[in] type
             * @param[in] text
             */
            virtual void strategyMessage ( MScriptBase::MessageType type,
                                           const std::string & text );
        //@}

    ////    Public Attributes    ////
    public:
        /// @brief
        std::vector<std::string> m_messages;

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptStrategy * const m_strategy;

        /// @brief
        MScriptStatement * m_codeTree;

        /// @brief
        MScriptStatement * m_includedCode;

        /// @brief
        std::vector<std::string> m_files;

        /// @brief
        bool m_success;
};

#endif // MSCRIPTCORE_H
