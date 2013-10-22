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
class CompilerSemanticAnalyzer;
class CompilerMsgObserver;
class CompilerMessageStack;
class DbgFile;
class DataFile;

// Base class and compiler interfaces.
#include "CompilerBase.h"
#include "CompilerParserInterface.h"
#include "CompilerSemanticInterface.h"
#include "CompilerSourceLocation.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// Standard header files.
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
    ////    Public Static Constants    ////
    public:
        /// @brief Maximum allowed number of messages.
        static const unsigned int MAX_MESSAGES = 1024;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] msgInterface
         */
        CompilerCore ( CompilerMsgInterface * msgInterface = NULL );

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
         * @param[in] genSimData
         * @return
         */
        bool compile ( LangId lang,
                       TargetArch arch,
                       CompilerOptions * opts,
                       bool genSimData = false );

        /**
         * @brief
         * @return
         */
        DbgFile * getSimDbg();

        /**
         * @brief
         * @return
         */
        DataFile * getSimData();

        /**
         * @brief
         * @param[in] fileName
         * @param[in] hide
         * @return
         */
        CompilerStatement * loadPrecompiledCode ( const std::string & fileName,
                                                  bool hide = false );

        /**
         * @brief
         * @param[in] fileName
         * @param[in] code
         * @return
         */
        bool savePrecompiledCode ( const std::string & fileName,
                                   const CompilerStatement * code );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        void localMessage ( const CompilerSourceLocation & location,
                            MessageType type,
                            const std::string & text );

        /// @name Interface for syntax and/or lexical analyzer
        //@{
            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             */
            void parserMessage ( const CompilerSourceLocation & location,
                                 MessageType type,
                                 const std::string & text );

            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             */
            void lexerMessage ( const CompilerSourceLocation & location,
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
            void processCodeTree ( CompilerStatement * codeTree );
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
            std::string locationToStr ( const CompilerSourceLocation & location ) const;

            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             */
            void compilerMessage ( const CompilerSourceLocation & location,
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

            /**
             * @brief
             * @param[in,out] observer
             * @return
             */
            void registerMsgObserver ( CompilerMsgObserver * observer );

            /**
             * @brief
             * @param[in] deviceName
             * @param[in] flag
             * @return
             */
            CompilerStatement * loadDevSpecCode ( const std::string & deviceName,
                                                  CompilerBase::DevSpecLoaderFlag * flag = NULL );
            /**
             * @brief
             * @return
             */
            std::string getBaseIncludeDir();

            /**
             * @brief
             * @return
             */
            std::string getBaseName();
        //@}

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        CompilerOptions * getCompilationOptions() const
        {
            return m_opts;
        }

        /**
         * @brief
         * @param[in] directory
         */
        void setBaseIncludeDir ( const std::string & directory )
        {
            m_baseIncludeDir = directory;
        }

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
        inline bool setupSemanticAnalyzer();

        /**
         * @brief
         * @param[in] lang
         * @param[in] arch
         * @return
         */
        inline bool startLexerAndParser();

        /**
         * @brief
         * @param[in] lang
         * @param[in] arch
         * @return
         */
        inline bool checkOptions();

        /**
         * @brief
         * @return
         */
        inline bool startCompilation();

        /**
         * @brief
         * @param[in] type
         * @param[in] text
         */
        inline void localMessage ( MessageType type,
                                   const std::string & text );

        /**
         * @brief
         * @param[in] type
         * @return
         */
        inline std::string msgType2str ( MessageType type );

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        CompilerMsgInterface * const m_msgInterface;

        /**
         * @brief
         */
        CompilerMsgObserver * m_msgObserver;

        /**
         * @brief
         */
        CompilerSemanticAnalyzer * m_semanticAnalyzer;

        /**
         * @brief
         */
        CompilerStatement * m_rootStatement;

        /**
         * @brief
         */
        CompilerOptions * m_opts;

        /**
         * @brief
         */
        CompilerMessageStack * m_messageStack;

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
        std::string m_baseIncludeDir;

        /**
         * @brief
         */
        std::string m_device;

        /**
         * @brief
         */
        boost::filesystem::path m_basePath;

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
        bool m_devSpecCodeLoaded;
};

#endif // COMPILERCORE_H
