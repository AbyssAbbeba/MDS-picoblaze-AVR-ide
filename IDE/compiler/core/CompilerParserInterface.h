// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014, 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Compiler
 * @file CompilerParserInterface.h
 */
// =============================================================================

#ifndef COMPILERPARSERINTERFACE_H
#define COMPILERPARSERINTERFACE_H

// Standard headers
#include <string>
#include <vector>
#include <cstdio>

// Include basic general compiler declarations and definitions
#include "CompilerBase.h"

// Include compiler components used by parsers and/or lexers
#include "CompilerExpr.h"
#include "CompilerStatement.h"
#include "CompilerLocationMap.h"
#include "CompilerStatementTypes.h"
#include "CompilerSourceLocation.h"
#include "CompilerLocationTracker.h"
#include "CompilerParserExtension.h"

// Used for i18n only
#include <QObject>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

// Make sure that the data type of locations is declared (see Bison manual for details)
#if ! defined ( YYLTYPE ) && ! defined ( YYLTYPE_IS_DECLARED )
    typedef struct YYLTYPE
    {
        int first_line;
        int first_column;
        int last_line;
        int last_column;
    } YYLTYPE;
    #define YYLTYPE_IS_TRIVIAL 1
    #define YYLTYPE_IS_DECLARED 1
#endif // ! YYLTYPE && ! YYLTYPE_IS_DECLARED

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerParserInterface
 */
class CompilerParserInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerParserInterface()
        {
            m_strMaxSize = 0;
            m_insertEol = false;
            m_insertEof = false;
            m_parserExtension = nullptr;
        }

        /**
         * @brief
         */
        ~CompilerParserInterface()
        {
            if ( nullptr != m_parserExtension )
            {
                delete m_parserExtension;
            }
        }

    ////    Public Operations    ////
    public:
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
                                           bool forceAsUnique = false ) = 0;

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         * @param[in] forceAsUnique
         */
        virtual void lexerMessage ( const CompilerSourceLocation & location,
                                    CompilerBase::MessageType type,
                                    const std::string & text,
                                    bool forceAsUnique = false ) = 0;

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         * @param[in] forceAsUnique
         */
        virtual void parserMessage ( const CompilerSourceLocation & location,
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
        virtual CompilerLocationMap & locationMap() = 0;

        /**
         * @brief
         * @param[in] filename
         * @param[in,out] finalFilename
         * @param[in] acyclic
         * @param[in] systemOnly
         * @return
         */
        virtual FILE * fileOpen ( const std::string & filename,
                                  std::string * finalFilename = nullptr,
                                  bool acyclic = true,
                                  bool systemOnly = false ) = 0;

        /**
         * @brief
         * @param[in] filename
         * @param[in,out] fileHandle
         * @return
         */
        virtual bool pushFileName ( const std::string & filename,
                                    FILE ** fileHandle ) = 0;

        /**
         * @brief
         */
        virtual void popFileName() = 0;

        /**
         * @brief
         * @return
         */
        virtual int getFileNumber() const = 0;

        /**
         * @brief
         * @param[in] uplevel
         * @return
         */
        virtual int getFileNumber ( unsigned int uplevel ) const = 0;

        /**
         * @brief
         * @param[in] filename
         * @return
         */
        virtual int getFileNumber ( const std::string & filename ) const = 0;

        /**
         * @brief
         * @param[in] filename
         */
        virtual void setVirtualFileName ( const std::string & filename ) = 0;

        /**
         * @brief
         * @param[in,out] codeTree
         */
        virtual void processCodeTree ( CompilerStatement * codeTree ) = 0;

        /**
         * @brief
         * @param[in] fileNumber
         * @return
         */
        virtual const std::string & getFileName ( int fileNumber = -1 ) const = 0;

        /**
         * @brief
         * @return
         */
        virtual const boost::filesystem::path & getBasePath() const = 0;

        /// @name Methods and attributes specific for Bison and Flex
        //@{
            /**
             * @brief
             * @param[in] yylloc
             * @return
             */
            CompilerSourceLocation toSourceLocation ( const YYLTYPE & yylloc ) const
            {
                return toSourceLocation(&yylloc);
            }

            /**
             * @brief
             * @param[in] yylloc
             * @return
             */
            CompilerSourceLocation toSourceLocation ( const YYLTYPE * yylloc ) const
            {
                return CompilerSourceLocation ( getFileNumber(),
                                                yylloc->first_line,
                                                yylloc->last_line,
                                                yylloc->first_column,
                                                yylloc->last_column );
            }

            /**
             * @brief
             * @param[in] yylloc
             * @param[in] type
             * @param[in] text
             * @param[in] forceAsUnique
             */
            void parserMessage ( const YYLTYPE & yylloc,
                                CompilerBase::MessageType type,
                                const std::string & text,
                                bool forceAsUnique = false  )
            {
                parserMessage(toSourceLocation(yylloc), type, text, forceAsUnique);
            }

            /**
             * @brief
             * @param[in] yylloc
             * @param[in] type
             * @param[in] text
             * @param[in] forceAsUnique
             */
            void lexerMessage ( const YYLTYPE * yylloc,
                                CompilerBase::MessageType type,
                                const std::string & text,
                                bool forceAsUnique = false  )
            {
                lexerMessage(toSourceLocation(yylloc), type, text, forceAsUnique);
            }
        //@}

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        bool getInsertEOL()
        {
            if ( true == m_insertEol )
            {
                m_insertEol = false;
                return true;
            }
            return false;
        }

        /**
         * @brief
         */
        void setInsertEOL()
        {
            m_insertEol = true;
        }

        /**
         * @brief
         * @return
         */
        bool getInsertEOF()
        {
            if ( true == m_insertEof )
            {
                m_insertEof = false;
                return true;
            }
            return false;
        }

        /**
         * @brief
         */
        void setInsertEOF()
        {
            m_insertEof = true;
        }

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         */
        void resetCompilerParserInterface()
        {
            m_insertEof = false;
            m_yyllocStack.clear();

            if ( nullptr != m_parserExtension )
            {
                delete m_parserExtension;
            }
        }

    ////    Public Attributes    ////
    public:
        /// Size of the string buffer, used for scanning strings and character literals.
        int m_strMaxSize;

        /// Stack to keep track of locations across multiple input buffers
        std::vector<YYLTYPE> m_yyllocStack;

        ///
        CompilerParserExtension * m_parserExtension;

    ////    Protected Attributes    ////
    private:
        ///
        bool m_insertEol;

        ///
        bool m_insertEof;
};

#endif // COMPILERPARSERINTERFACE_H
