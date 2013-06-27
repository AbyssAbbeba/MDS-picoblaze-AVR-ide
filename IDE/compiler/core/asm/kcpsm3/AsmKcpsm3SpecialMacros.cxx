// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3SpecialMacros.cxx
 */
// =============================================================================

#include "AsmKcpsm3SpecialMacros.h"

// Common compiler header files.
#include "../../CompilerStatementTypes.h"
#include "../../CompilerStatement.h"

CompilerStatement * AsmKcpsm3SpecialMacros::runTimeCondition ( CompilerStatement * rtIfTree )
{
    using namespace CompilerStatementTypes;

    CompilerStatement * result = NULL;

    if ( NULL == rtIfTree )
    {
        return NULL;
    }

    for ( CompilerStatement * node = rtIfTree;
          NULL != node;
          node = node->next() )
    {
        switch ( (int) node->type() )
        {
            case ASMKCPSM3_DIR_RTIF:
            {
            }
            case ASMKCPSM3_DIR_RTELSEIF:
            {
            }
            case ASMKCPSM3_DIR_RTELSE:
            {
            }
            case ASMKCPSM3_DIR_RTENDIF:
            {
            }
        }
    }

    return result;
}
