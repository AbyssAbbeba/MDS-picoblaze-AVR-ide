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
 * @ingroup Compiler
 * @file CompilerCore.h
 */
// =============================================================================

#ifndef COMPILERCORE_H
#define COMPILERCORE_H

// Forward declarations
class CompilerExpr;
class CompilerStatement;
class CompilerMsgInterface;
class CompilerOptions;
class SemanticAnalyzer;

// Base class and compiler interfaces
#include "CompilerBase.h"
#include "CompilerParserInterface.h"
#include "CompilerSemanticInterface.h"

// Standard header files
#include <string>
#include <vector>


/**
 * @brief
 * @ingroup Compiler
 * @class CompilerCore
 */
class CompilerCore : public CompilerBase,
                     private CompilerParserInterface,
                     private CompilerSemanticInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] msgInterface
         */
        CompilerCore ( CompilerMsgInterface * msgInterface );

        /**
         * @brief
         */
        ~CompilerCore();

    private:
        /**
         * @brief Forbidden constructor
         */
        CompilerCore();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] lang
         * @param[in] arch
         * @param[in] opts
         * @return
         */
        bool compile ( LangId lang,
                       TargetArch arch,
                       CompilerOptions * opts );

    ////    Private Operations    ////
    private:
        /// @name Interface for syntax and/or lexical analyzer
        //@{
            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             */
            void parserMessage ( SourceLocation location,
                                 MessageType type,
                                 const std::string & text );

            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             */
            void lexerMessage ( SourceLocation location,
                                MessageType type,
                                const std::string & text );

            /**
             * @brief
             * @param[in] filename
             * @param[in] acyclic
             * @return
             */
            FILE * fileOpen ( const std::string & filename,
                              bool acyclic = true );

            /**
             * @brief
             * @param[in] filename
             */
            bool pushFileName ( const std::string & filename );

            /**
             * @brief
             */
            void popFileName();

            /**
             * @brief
             * @return
             */
            int getFileNumber() const;

            /**
             * @brief
             * @param[in] uplevel
             * @return
             */
            int getFileNumber ( unsigned int uplevel ) const;

            /**
             * @brief
             * @param[in] filename
             * @return
             */
            int getFileNumber ( const std::string & filename ) const;

            /**
             * @brief
             * @param[in,out] codeTree
             * @return
             */
            void syntaxAnalysisComplete ( CompilerStatement * codeTree );
        //@}

        /// @name Interface for semantic analyzer
        //@{
            /**
             * @brief
             * @return
             */
            const std::vector<std::string> & listSourceFiles() const;

            /**
             * @brief
             * @param[in] fileNumber
             * @return
             */
            const std::string & getFileName ( int fileNumber ) const;

            /**
             * @brief
             * @param[in] location
             * @return
             */
            std::string locationToStr ( const CompilerBase::SourceLocation & location ) const;

            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             */
            void compilerMessage ( SourceLocation location,
                                   MessageType type,
                                   const std::string & text );

            /**
             * @brief
             * @param[in] type
             * @param[in] text
             */
            void compilerMessage ( MessageType type,
                                   const std::string & text );

            /**
             * @brief
             * @return
             */
            bool successful() const;
        //@}

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] filename
         */
        inline void setFileName ( const std::string & filename );

        /**
         * @brief
         */
        inline void resetCompilerCore();

        /**
         * @brief
         * @param[in] lang
         * @param[in] arch
         * @return
         */
        inline bool setupSemanticAnalyzer ( LangId lang,
                                            TargetArch arch );

        /**
         * @brief
         * @param[in] lang
         * @param[in] arch
         * @return
         */
        inline bool startLexerAndParser ( LangId lang,
                                          TargetArch arch );

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        CompilerMsgInterface * const m_msgInterface;

        /**
         * @brief
         */
        SemanticAnalyzer * m_semanticAnalyzer;

        /**
         * @brief
         */
        CompilerStatement * m_rootStatement;

        /**
         * @brief
         */
        std::vector<std::string> m_fileNameStack;

        /**
         * @brief
         */
        std::vector<std::string> m_fileNames;

        /**
         * @brief
         */
        std::string m_filename;

        /**
         * @brief
         */
        int m_fileNumber;

        /**
         * @brief
         */
        bool m_success;

        /**
         * @brief
         */
        CompilerOptions * m_opts;
};

#endif // COMPILERCORE_H
