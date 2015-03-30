
// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerCPicoBlaze
 * @file CompilerCPicoBlazeBackend.h
 */
// =============================================================================

#ifndef COMPILERCPICOBLAZEBACKEND_H
#define COMPILERCPICOBLAZEBACKEND_H

// C compiler header files.
#include "C/core/CompilerCBackend.h"

/**
 * @brief
 * @ingroup CompilerCPicoBlaze
 * @class CompilerCPicoBlazeBackend
 */
class CompilerCPicoBlazeBackend : public CompilerCBackend
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        CompilerCPicoBlazeBackend ( CompilerSemanticInterface * compilerCore,
                                    CompilerOptions * opts );
};

#endif // COMPILERCPICOBLAZEBACKEND_H
