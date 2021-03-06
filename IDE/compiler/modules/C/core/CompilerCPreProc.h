// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014, 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCPreProc.h
 */
// =============================================================================

#ifndef COMPILERCPREPROC_H
#define COMPILERCPREPROC_H

// Forward declarations.
class CompilerOptions;
class CompilerParserInterface;

// Standard headers.
#include <map>
#include <string>
#include <vector>

// Compiler header files.
#include "CompilerSourceLocation.h"

// C compiler preprocessor header files.
#include "CompilerCBackend.h"
#include "CompilerCPreProcMacros.h"
#include "CompilerCPreProcSupport.h"
#include "CompilerCPreProcInterface.h"

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCPreProc
 */
class CompilerCPreProc : protected CompilerCPreProcSupport,
                         protected CompilerCPreProcInterface
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum Directive
        {
            DIR_INCLUDE,///<
            DIR_LINE,   ///<
            DIR_PRAGMA, ///<
            DIR_DEFINE, ///<
            DIR_UNDEF,  ///<
            DIR_IF,     ///<
            DIR_ELIF,   ///<
            DIR_IFDEF,  ///<
            DIR_IFNDEF, ///<
            DIR_ELSE,   ///<
            DIR_ENDIF,  ///<
            DIR_WARNING,///<
            DIR_ERROR,  ///<
            DIR_NULL    ///<
        };

    ////    Private Static Constants    ////
    private:
        /// @brief
        static const std::map<std::string, Directive> s_directives;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        CompilerCPreProc ( CompilerParserInterface * compilerCore,
                           const CompilerOptions * opts,
                           CompilerCBackend * backend );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] inputFiles
         * @return
         */
        char * processFiles ( const std::vector<FILE *> & inputFiles );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] expr
         * @param[in] length
         * @return
         */
        bool evaluateExpr ( char * expr,
                            unsigned int length );

        /**
         * @brief
         * @param[in] expr
         * @param[in] length
         * @return
         */
        bool expectSigleId ( char * expr,
                             unsigned int length );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] inBuffer
         * @param[in,out] outBuffer
         * @param[in,out] mergeBuffer
         * @param[in,out] mlineBuffer
         * @return
         */
        inline bool inputProcessing ( Buffer & inBuffer,
                                      Buffer & outBuffer,
                                      Buffer & mergeBuffer,
                                      Buffer & mlineBuffer );

        /**
         * @brief
         * @param[in,out] inBuffer
         * @param[in,out] mergeBuffer
         * @return
         */
        inline bool lineMerging ( Buffer & inBuffer,
                                  Buffer & mergeBuffer );

        /**
         * @brief
         * @param[in,out] buffer
         * @param[out] lineMerge
         * @return
         */
        inline bool initialProcessing ( Buffer & buffer,
                                        bool & lineMerge );

        /**
         * @brief
         * @param[in,out] buffer
         * @return
         */
        inline bool directivesProcessing ( Buffer & buffer );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] directive
         * @return
         */
        inline bool handleDirective ( char * arguments,
                                      Directive directive );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] argLength
         * @param[in] recursion
         * @return
         */
        inline void handleInclude ( char * arguments,
                                    unsigned int argLength,
                                    bool recursion = false );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] argLength
         * @return
         */
        inline void handleLine ( char * arguments,
                                 const unsigned int argLength );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] argLength
         * @return
         */
        inline void handlePragma ( char * arguments,
                                   const unsigned int argLength );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] argLength
         * @return
         */
        inline void handleDefine ( char * arguments,
                                   unsigned int argLength );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] argLength
         * @return
         */
        inline void handleUndef ( char * arguments,
                                  unsigned int argLength );

        /**
         * @brief
         * @param[in,out] length
         * @param[in,out] line
         * @param[in] pos
         */
        inline void cutLine ( ssize_t & length,
                              char * line,
                              unsigned int pos );

        /**
         * @brief
         * @param[out] tokens
         * @param[in] arguments
         * @param[in] length
         */
        inline void tokenize ( std::vector<char*> & tokens,
                               char * arguments,
                               unsigned int length );

    ////    Private Attributes    ////
    private:
        ///
        unsigned int m_lineMerges;

        ///
        InMode m_inmode;

        ///
        Conditional m_conditional;

        ///
        Include m_include;

        ///
        CompilerCPreProcMacros m_macroTable;

        ///
        CompilerSourceLocation m_location;

        ///
        std::vector<CompilerSourceLocation> m_locationStack;
};

#endif // COMPILERCPREPROC_H
