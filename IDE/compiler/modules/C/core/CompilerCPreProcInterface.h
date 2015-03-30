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
 * @file CompilerCPreProcInterface.h
 */
// =============================================================================

#ifndef COMPILERCPREPROCINTERFACE_H
#define COMPILERCPREPROCINTERFACE_H

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerParserInterface.h"

// C compiler preprocessor header files.
#include "CompilerCBackend.h"

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCPreProcInterface
 */
class CompilerCPreProcInterface
{
    ////    Constructors and Destructors    ////
    protected:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @param[in] backend
         */
        CompilerCPreProcInterface ( CompilerParserInterface * compilerCore,
                                    const CompilerOptions * opts,
                                    const CompilerCBackend * backend )
                                  :
                                    m_compilerCore ( compilerCore ),
                                    m_opts ( opts ),
                                    m_backend ( backend ) {}

        /**
         * @brief
         */
        virtual ~CompilerCPreProcInterface() {}

    ////    Public Attributes    ////
    public:
        ///
        CompilerParserInterface * const m_compilerCore;

        ///
        const CompilerOptions * const m_opts;

        ///
        const CompilerCBackend * const m_backend;

        ///
        CompilerSourceLocation m_exprLocation;

        ///
        bool m_exprResult;
};

#endif // COMPILERCPREPROCINTERFACE_H
