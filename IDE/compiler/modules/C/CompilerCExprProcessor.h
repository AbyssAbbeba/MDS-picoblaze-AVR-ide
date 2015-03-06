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
 * @file CompilerCExprProcessor.h
 */
// =============================================================================

#ifndef COMPILERCEXPRPROCESSOR_H
#define COMPILERCEXPRPROCESSOR_H

// Forward declarations.
class CompilerExpr;
class CompilerOptions;
class CompilerSemanticInterface;

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCExprProcessor
 */
class CompilerCExprProcessor
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerCExprProcessor ( CompilerSemanticInterface * compilerCore,
                                 CompilerOptions           * opts );

        /**
         * @brief
         */
        virtual ~CompilerCExprProcessor();

    ////    Public Operations    ////
    public:

    ////    Inline Private Operations    ////
    private:

    ////    Private Attributes    ////
    private:
        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;
};

#endif // COMPILERCEXPRPROCESSOR_H
