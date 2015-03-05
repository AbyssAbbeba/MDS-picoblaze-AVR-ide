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
class CompilerOptions;
class CompilerStatement;
class CompilerCSymbolTable;
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
                               CompilerCSymbolTable      * symbolTable );

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
        void processDeclarations ( CompilerStatement * codeTree );

    ////    Inline Private Operations    ////
    private:
        inline void unexpectedNode ( CompilerStatement * node );

    ////    Private Attributes    ////
    private:
        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;

        ///
        CompilerCSymbolTable * const m_symbolTable;
};

#endif // COMPILERCTREEDECODER_H
