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
#include "StatementTypes.h"

// Used for i18n only
#include <QObject>

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
        }

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        virtual void parserMessage ( CompilerBase::SourceLocation location,
                                     CompilerBase::MessageType type,
                                     const std::string & text ) = 0;

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        virtual void lexerMessage ( CompilerBase::SourceLocation location,
                                    CompilerBase::MessageType type,
                                    const std::string & text ) = 0;

        /**
         * @brief
         * @param[in] filename
         * @param[in] acyclic
         * @return
         */
        virtual FILE * fileOpen ( const std::string & filename,
                                  bool acyclic = true ) = 0;

        /**
         * @brief
         * @param[in] filename
         * @return
         */
        virtual bool pushFileName ( const std::string & filename ) = 0;

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
         * @param[in,out] codeTree
         */
        virtual void syntaxAnalysisComplete ( CompilerStatement * codeTree ) = 0;

        /// @name Methods and attributes specific for Bison and Flex
        //@{
            /**
             * @brief
             * @param[in] yylloc
             * @return
             */
            CompilerBase::SourceLocation toSourceLocation ( const YYLTYPE & yylloc ) const
            {
                    return toSourceLocation(&yylloc);
            }

            /**
             * @brief
             * @param[in] yylloc
             * @return
             */
            CompilerBase::SourceLocation toSourceLocation ( const YYLTYPE * yylloc ) const
            {
                return CompilerBase::SourceLocation ( getFileNumber(),
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
             */
            void parserMessage ( const YYLTYPE & yylloc,
                                CompilerBase::MessageType type,
                                const std::string & text )
            {
                parserMessage(toSourceLocation(yylloc), type, text);
            }

            /**
             * @brief
             * @param[in] yylloc
             * @param[in] type
             * @param[in] text
             */
            void lexerMessage ( const YYLTYPE * yylloc,
                                CompilerBase::MessageType type,
                                const std::string & text )
            {
                lexerMessage(toSourceLocation(yylloc), type, text);
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

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         */
        void resetCompilerParserInterface()
        {
            m_insertEol = false;
            m_yyllocStack.clear();
        }

    ////    Public Attributes    ////
    public:
        /// Size of the string buffer, used for scanning strings and character literals.
        int m_strMaxSize;

        /// Stack to keep track of locations across multiple input buffers
        std::vector<YYLTYPE> m_yyllocStack;

    ////    Protected Attributes    ////
    private:
        ///
        bool m_insertEol;
};

    /*
     * In case this file is included in the compiler core, and not in a parser or lexer, we may also
     * need to declare parser/lexer functions, data types, and variables, used by the compiler core.
     */
    #ifdef COMPILERCORE_H
        /*
         * Make sure that the data type of semantic values is declared (see Bison manual).
         * Actually the compiler core does not work with this data type, it is used only by
         * the Flex generated lexical analyzer prototypes; in other words, each syntax/lexical
         * analyzer might use it's own data type of semantic values.
         */
        #if ! defined ( YYSTYPE ) && ! defined ( YYSTYPE_IS_DECLARED )
            typedef union YYSTYPE {} YYSTYPE;
        #endif

        // Include lexer prototypes (they are used by the core to initialize and destroy a lexer)
        #include "avr8lexer.h"
        #include "pic8lexer.h"
        #include "mcs51lexer.h"
        #include "kcpsm3lexer.h"

        // Parser prototypes (the core uses them to initiate syntactical analysis)
        int avr8parser_parse   ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
        int pic8parser_parse   ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
        int mcs51parser_parse  ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
        int kcpsm3parser_parse ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
    #endif // COMPILERCORE_H

#endif // COMPILERPARSERINTERFACE_H
