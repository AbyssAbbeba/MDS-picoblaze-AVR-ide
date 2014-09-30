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
 * @ingroup Assembler
 * @file AsmCodeGenerator.h
 */
// =============================================================================

#ifndef ASMCODEGENERATOR_H
#define ASMCODEGENERATOR_H

// Forward declarations.
class CompilerStatement;

// Standard headers.
#include <string>

/**
 * @brief
 * @ingroup Assembler
 * @class AsmCodeGenerator
 */
class AsmCodeGenerator
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[out] result
         * @param[in] node
         */
        virtual void toSourceLine ( std::string & result,
                                    const CompilerStatement * node ) = 0;
};

#endif // ASMCODEGENERATOR_H
