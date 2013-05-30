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
        case EMPTY_STATEMENT:       out << "EMPTY_STATEMENT";       break;
        case STMT_SCOPE:            out << "STMT_SCOPE";            break;
        case STMT_CONDITIONAL_EXEC: out << "STMT_CONDITIONAL_EXEC"; break;
        case STMT_IF:               out << "STMT_IF";               break;
        case STMT_ELSE:             out << "STMT_ELSE";             break;
        case STMT_EXPR:             out << "STMT_EXPR";             break;
        case STMT_TRIGGER:          out << "STMT_TRIGGER";          break;
        case STMT_FUNCTION:         out << "STMT_FUNCTION";         break;
        case STMT_FOR:              out << "STMT_FOR";              break;
        case STMT_WHILE:            out << "STMT_WHILE";            break;
        case STMT_DO_WHILE:         out << "STMT_DO_WHILE";         break;
        case STMT_RETURN:           out << "STMT_RETURN";           break;
        case STMT_CONTINUE:         out << "STMT_CONTINUE";         break;
        case STMT_BREAK:            out << "STMT_BREAK";            break;
        case STMT_SWITCH:           out << "STMT_SWITCH";           break;
        case STMT_CASE:             out << "STMT_CASE";             break;
        case STMT_DELETE:           out << "STMT_DELETE";           break;
    }
    return out;
}
