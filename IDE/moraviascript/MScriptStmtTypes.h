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
 * @ingroup MoraviaScript
 * @file MScriptStmtTypes.h
 */
// =============================================================================

#ifndef MSCRIPTSTMTTYPES_H
#define MSCRIPTSTMTTYPES_H

// Standard header files
#include <ostream>

/**
 * @brief
 * @namespace MScriptStmtTypes
 * @ingroup MoraviaScript
 */
namespace MScriptStmtTypes
{
    /**
     * @brief
     */
    enum Type
    {
        STMT_EMPTY      = -1, ///<
        STMT_ROOT       = 0,  ///<

        STMT_SCOPE,           ///<
        STMT_CONDITION,       ///<
        STMT_IF,              ///<
        STMT_ELSE,            ///<
        STMT_EXPR,            ///<
        STMT_TRIGGER,         ///<
        STMT_FUNCTION,        ///<
        STMT_FOR,             ///<
        STMT_WHILE,           ///<
        STMT_DO_WHILE,        ///<
        STMT_RETURN,          ///<
        STMT_CONTINUE,        ///<
        STMT_BREAK,           ///<
        STMT_SWITCH,          ///<
        STMT_CASE,            ///<
        STMT_DEFAULT,         ///<
        STMT_DELETE,          ///<
        STMT_VAR,             ///<
        STMT_CONST,           ///<

        STMT_CALL             ///<
    };
};

/**
 * @brief Tracing operator
 * @param[in,out] out
 * @param[in] type
 * @return
 */
std::ostream & operator << ( std::ostream & out,
                             MScriptStmtTypes::Type type );

#endif // MSCRIPTSTMTTYPES_H
