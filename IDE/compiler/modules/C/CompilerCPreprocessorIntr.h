// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCPreprocessorIntr.h
 */
// =============================================================================

#ifndef COMPILERCPREPROCESSORINTR_H
#define COMPILERCPREPROCESSORINTR_H

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerParserInterface.h"

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCPreprocessorIntr
 */
class CompilerCPreprocessorIntr
{
    ////    Constructors and Destructors    ////
    protected:
        CompilerCPreprocessorIntr ( CompilerParserInterface * compilerCore,
                                    CompilerOptions * opts )
                                  :
                                    m_compilerCore ( compilerCore ),
                                    m_opts ( opts ) {}

    ////    Public Attributes    ////
    public:
        ///
        CompilerParserInterface * m_compilerCore;

        ///
        CompilerOptions * m_opts;

        ///
        bool m_exprResult;
};

#endif // COMPILERCPREPROCESSORINTR_H
