// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3SpecialMacros.h
 */
// =============================================================================

#ifndef ASMKCPSM3SPECIALMACROS_H
#define ASMKCPSM3SPECIALMACROS_H

// Forward declarations.
class CompilerStatement;

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3SpecialMacros
 */
class AsmKcpsm3SpecialMacros
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] rtIfTree
         * @return
         */
        CompilerStatement * runTimeCondition ( CompilerStatement * rtIfTree );
};

#endif // ASMKCPSM3SPECIALMACROS_H
