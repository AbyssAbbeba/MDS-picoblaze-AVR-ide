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
class CompilerSemanticInterface;

// Base class and compiler interfaces
#include "CompilerBase.h"
#include "CompilerParserInterface.h"

// Standard header files
#include <string>
#include <vector>


/**
 * @brief
 * @ingroup Compiler
 * @class CompilerCore
 */
class CompilerCore : public CompilerBase,
                     private CompilerParserInterface
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
                       CompilerOptions * const opts );

    ////    Private Operations    ////
    private:
        /**
            * @brief
            * @param[in] filename
            */
        inline void setFileName ( const std::string & filename );

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

    public:
        /// @name Interface for semantic analyzer
        //@{
            /**
             * @brief
             * @return
             */
            const std::vector<std::string> & listSourceFiles() const;

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
        //@}

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void resetCompilerCore();

        /**
         * @brief
         * @param[in] lang
         * @param[in] arch
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        inline bool setupSemanticAnalyzer ( LangId lang,
                                            TargetArch arch,
                                            CompilerOptions * const opts );

        /**
         * @brief
         * @param[in] lang
         * @param[in] arch
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        inline bool startLexerAndParser ( LangId lang,
                                          TargetArch arch,
                                          CompilerOptions * const opts );

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        CompilerMsgInterface * const m_msgInterface;

        /**
         * @brief
         */
        CompilerSemanticInterface * m_semanticAnalyzer;

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
};

#endif // COMPILERCORE_H
