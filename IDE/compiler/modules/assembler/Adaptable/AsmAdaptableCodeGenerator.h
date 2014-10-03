// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdaptableAsm
 * @file AsmAdaptableCodeGenerator.h
 */
// =============================================================================

#ifndef ASMADAPTABLECODEGENERATOR_H
#define ASMADAPTABLECODEGENERATOR_H

// Common compiler header files.
#include "AsmCodeGenerator.h"

// Standard headers.
#include <string>

/**
 * @brief
 * @ingroup AdaptableAsm
 * @class AsmAdaptableCodeGenerator
 */
class AsmAdaptableCodeGenerator : public AsmCodeGenerator
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[out] result
         * @param[in] node
         */
        virtual void toSourceLine ( std::string & result,
                                    const CompilerStatement * node ) final override;
};

#endif // ASMADAPTABLECODEGENERATOR_H
