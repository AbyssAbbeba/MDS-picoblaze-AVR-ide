// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCTreeDecoder.h
 */
// =============================================================================

#ifndef COMPILERCTREEDECODER_H
#define COMPILERCTREEDECODER_H

// Forward declarations.
class CompilerExpr;
class CompilerOptions;
class CompilerStatement;
class CompilerCSymbolTable;
class CompilerCDeclaration;
class CompilerCExprProcessor;
class CompilerSemanticInterface;

// Compiler header files.
#include "CompilerSourceLocation.h"

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCTreeDecoder
 */
class CompilerCTreeDecoder
{
    ////    Public Datatypes    ////
    public:

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerCTreeDecoder ( CompilerSemanticInterface * compilerCore,
                               CompilerOptions           * opts,
                               CompilerCSymbolTable      * symbolTable,
                               CompilerCExprProcessor    * exprProcessor );

        /**
         * @brief
         */
        virtual ~CompilerCTreeDecoder();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        void processCodeTree ( CompilerStatement * codeTree );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[out] isTypedef
         * @param[in] specifiers
         */
        CompilerCDeclaration * resolveDeclaration ( bool & isTypedef,
                                                    const CompilerExpr * specifiers );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] declaration
         */
        inline void processDeclaration ( CompilerExpr * declaration );

        /**
         * @brief
         * @param[in,out] definition
         */
        inline void processFuncDef ( CompilerStatement * definition );

        /**
         * @brief
         * @param[in,out] expr
         */
        inline void processExpressions ( CompilerExpr * expr );

        /**
         * @brief
         * @param[in] location
         */
        inline void unexpectedNode ( const CompilerSourceLocation & location );

    ////    Private Attributes    ////
    private:
        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;

        ///
        CompilerCSymbolTable * const m_symbolTable;

        ///
        CompilerCExprProcessor * const m_exprProcessor;
};

#endif // COMPILERCTREEDECODER_H
