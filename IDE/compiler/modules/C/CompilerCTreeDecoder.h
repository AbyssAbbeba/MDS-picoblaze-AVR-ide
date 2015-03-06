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
class CompilerCExprProcessor;
class CompilerSemanticInterface;
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

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] node
         */
        inline void unexpectedNode ( const CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] declaration
         */
        inline void processDeclaration ( CompilerStatement * declaration );

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
