// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
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
        case STMT_EMPTY:     out << "EMPTY";     break;
        case STMT_ROOT:      out << "ROOT";      break;

        case STMT_SCOPE:     out << "SCOPE";     break;
        case STMT_CONDITION: out << "CONDITION"; break;
        case STMT_IF:        out << "IF";        break;
        case STMT_ELSE:      out << "ELSE";      break;
        case STMT_EXPR:      out << "EXPR";      break;
        case STMT_TRIGGER:   out << "TRIGGER";   break;
        case STMT_FUNCTION:  out << "FUNCTION";  break;
        case STMT_FOR:       out << "FOR";       break;
        case STMT_WHILE:     out << "WHILE";     break;
        case STMT_DO_WHILE:  out << "DO_WHILE";  break;
        case STMT_RETURN:    out << "RETURN";    break;
        case STMT_CONTINUE:  out << "CONTINUE";  break;
        case STMT_BREAK:     out << "BREAK";     break;
        case STMT_SWITCH:    out << "SWITCH";    break;
        case STMT_CASE:      out << "CASE";      break;
        case STMT_DEFAULT:   out << "DEFAULT";   break;
        case STMT_DELETE:    out << "DELETE";    break;
        case STMT_VAR:       out << "VAR";       break;
        case STMT_CONST:     out << "CONST";     break;
        case STMT_NAMESPACE: out << "NAMESPACE"; break;
        case STMT_INCLUDE:   out << "INCLUDE";   break;
        case STMT_EVAL:      out << "EVAL";      break;
        case STMT_MESSAGE:   out << "MESSAGE";   break;
        case STMT_WARNING:   out << "WARNING";   break;
        case STMT_ABORT:     out << "ABORT";     break;

        case STMT_CALL:      out << "CALL";      break;
    }
    return out;
}
