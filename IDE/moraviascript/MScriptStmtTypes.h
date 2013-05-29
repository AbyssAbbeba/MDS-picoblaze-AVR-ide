// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
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
    enum Type
    {
        EMPTY_STATEMENT = 0,

        STMT_CONDITIONAL_EXEC,
        STMT_IF,
        STMT_ELSE,
        STMT_EXPR
    };
};

/**
 * @brief Tracing operator
 * @param[in,out] out
 * @param[in] type
 * @return
 */
std::ostream & operator << ( std::ostream & out,
                             const MScriptStmtTypes::Type * type );

#endif // MSCRIPTSTMTTYPES_H
