// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup MoraviaScript
 * @file MScriptStmtTypes.cxx
 */
// =============================================================================

#include "MScriptStmtTypes.h"

std::ostream & operator << ( std::ostream & out,
                             MScriptStmtTypes::Type type )
{
    using namespace MScriptStmtTypes;

    switch ( type )
    {
        case EMPTY_STATEMENT:           out << "EMPTY_STATEMENT";               break;
        case STMT_CONDITIONAL_EXEC:     out << "STMT_CONDITIONAL_EXEC";         break;
        case STMT_IF:                   out << "STMT_IF";                       break;
        case STMT_ELSE:                 out << "STMT_ELSE";                     break;
        case STMT_EXPR:                 out << "STMT_EXPR";                     break;

    }
    return out;
}
