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
 * @file CompilerCore.h
 */
// =============================================================================

#ifndef COMPILERCORE_H
#define COMPILERCORE_H

// Forward declarations
class DbgFile;
class DataFile;
class CompilerExpr;
class CompilerOptions;
class CompilerStatement;
class CompilerMsgFilter;
class CompilerMsgObserver;
class CompilerMsgInterface;
class CompilerMessageStack;
class CompilerSemanticAnalyzer;

// Base class and compiler interfaces.
#include "CompilerBase.h"
#include "CompilerSourceLocation.h"
#include "CompilerParserInterface.h"
#include "CompilerSemanticInterface.h"

// Additional compiler components.
#include "CompilerLocationTracker.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// Standard header files.
#include <cstdio>
#include <string>
#include <vector>
#include <utility>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerCore
 */
class CompilerCore : public CompilerBase,
                     public CompilerParserInterface,
                     public CompilerSemanticInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] msgInterface
         */
        CompilerCore ( CompilerMsgInterface * msgInterface = nullptr );

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

        /**
         * @brief Close all opened input files.
         *
         * @note
         * This is normally done automatically anyway, use this method only when you wish to close the files before the
         * cleanup phase.
         */
        void closeInputFiles();

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         * @param[in] forceAsUnique
         * @param[in] noObserver
         */
        void coreMessage ( const CompilerSourceLocation & location,
                           MessageType type,
                           const std::string & text,
                           bool forceAsUnique = false,
                           bool noObserver = false );

        /// @name Interface for syntax and/or lexical analyzer, and for preprocessor.
        //@{
            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             * @param[in] forceAsUnique
             */
            virtual void preprocessorMessage ( const CompilerSourceLocation & location,
                                               CompilerBase::MessageType type,
                                               const std::string & text,
                                               bool forceAsUnique = false ) override;

            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             * @param[in] forceAsUnique
             */
            virtual void lexerMessage ( const CompilerSourceLocation & location,
                                        MessageType type,
                                        const std::string & text,
                                        bool forceAsUnique = false ) override;

            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             * @param[in] forceAsUnique
             */
            virtual void parserMessage ( const CompilerSourceLocation & location,
                                         MessageType type,
                                         const std::string & text,
                                         bool forceAsUnique = false ) override;

            /**
             * @brief
             * @param[in] filename
             * @param[in,out] finalFilename
             * @param[in] acyclic
             * @return
             */
            virtual FILE * fileOpen ( const std::string & filename,
                                      std::string * finalFilename = nullptr,
                                      bool acyclic = true ) override;

            /**
             * @brief
             * @param[in] filename
             * @param[in,out] fileHandle
             * @return
             */
            virtual bool pushFileName ( const std::string & filename,
                                        FILE ** fileHandle ) override;

            /**
             * @brief
             */
            virtual void popFileName() override;

            /**
             * @brief
             * @return
             */
            virtual int getFileNumber() const override;

            /**
             * @brief
             * @param[in] uplevel
             * @return
             */
            virtual int getFileNumber ( unsigned int uplevel ) const override;

            /**
             * @brief
             * @param[in] filename
             * @return
             */
            virtual int getFileNumber ( const std::string & filename ) const override;

            /**
             * @brief
             * @param[in,out] codeTree
             * @return
             */
            virtual void processCodeTree ( CompilerStatement * codeTree ) override;
        //@}

        /// @name Interface common for syntax analyzer, lexical analyzer, preprocessor, and for semantic analyzer.
        //@{
            /**
             * @brief
             * @return
             */
            virtual CompilerLocationTracker & locationTrack() override;
        //@}

        /// @name Interface for semantic analyzer
        //@{
            /**
             * @brief
             * @return
             */
            virtual const std::vector<std::pair<std::string,FILE*>> & listSourceFiles() const override;

            /**
             * @brief
             * @param[in] fileNumber
             * @return
             */
            virtual const std::string & getFileName ( int fileNumber ) const override;

            /**
             * @brief
             * @param[in] location
             * @param[in] main If location is used inside a message text, the to false.
             * @return
             */
            virtual std::string locationToStr ( const CompilerSourceLocation & location,
                                                bool main = false ) const override;

            /**
             * @brief
             * @param[in] location
             * @param[in] type
             * @param[in] text
             * @param[in] forceAsUnique
             */
            virtual void semanticMessage ( const CompilerSourceLocation & location,
                                           MessageType type,
                                           const std::string & text,
                                           bool forceAsUnique = false ) override;

            /**
             * @brief
             * @return
             */
            virtual bool successful() const override;

            /**
             * @brief
             * @param[in,out] observer
             * @return
             */
            virtual void registerMsgObserver ( CompilerMsgObserver * observer ) override;

            /**
             * @brief
             * @param[in] deviceName
             * @param[in] flag
             * @return
             */
            virtual CompilerStatement * loadDevSpecCode ( const std::string & deviceName,
                                                          CompilerBase::DevSpecLoaderFlag * flag = nullptr ) override;
            /**
             * @brief
             * @return
             */
            virtual std::string getBaseIncludeDir() override;

            /**
             * @brief
             * @return
             */
            virtual std::string getBaseName() override;
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
         * @param[in] directory Must be an absolute path (on Windows including c:\ and such stuff).
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
         * @param[in,out] fileHandle In case this is a newly opened file, this parameter has to be set.
         */
        inline void setOpenedFile ( const std::string & filename,
                                    FILE ** fileHandle = nullptr );

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
        inline void coreMessage ( MessageType type,
                                  const std::string & text );

        /**
         * @brief
         * @param[in] type
         * @return
         */
        inline std::string msgType2str ( MessageType type );

    ////    Private Attributes    ////
    private:
        /// @brief
        CompilerMsgFilter * m_msgInterface;

        /// @brief
        CompilerMsgObserver * m_msgObserver;

        /// @brief
        CompilerSemanticAnalyzer * m_semanticAnalyzer;

        /// @brief
        CompilerStatement * m_rootStatement;

        /// @brief
        CompilerOptions * m_opts;

        /// @brief
        CompilerMessageStack * m_messageStack;

        /// @brief
        std::vector<std::string> m_fileNameStack;

        /// @brief
        std::vector<std::pair<std::string,FILE*>> m_openedFiles;

        /// @brief
        CompilerLocationTracker m_locationTracker;

        /// @brief
        std::string m_filename;

        /// @brief
        std::string m_baseIncludeDir;

        /// @brief
        std::string m_device;

        /// @brief
        boost::filesystem::path m_basePath;

        /// @brief
        int m_fileNumber;

        /// @brief
        bool m_success;

        /// @brief
        bool m_devSpecCodeLoaded;
};

#endif // COMPILERCORE_H
