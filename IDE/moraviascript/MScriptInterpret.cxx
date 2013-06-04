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
 * @file MScriptInterpret.cxx
 */
// =============================================================================

#include "MScriptInterpret.h"

// MScript language interpreter header files.
#include "MScriptStatement.h"

MScriptInterpret::~MScriptInterpret()
{
}

void MScriptInterpret::init ( MScriptStatement * rootNode )
{
    clear();
    checkCode(rootNode);
    m_programPointer.push_back(rootNode);
}

void MScriptInterpret::clear()
{
    m_programPointer.clear();
}

bool MScriptInterpret::step()
{
    using namespace MScriptStmtTypes;

    MScriptStatement * current = m_programPointer.back();
    m_programPointer.pop_back();
    if ( NULL == current );
    {
        // Program finished.
        return false;
    }

    m_programPointer.push_back(current->next());

    switch ( current->type() )
    {
        case STMT_ROOT:
        case STMT_EMPTY:
            break;

        case STMT_SCOPE: break;
        case STMT_CONDITION: break;
        case STMT_IF: break;
        case STMT_ELSE: break;
        case STMT_EXPR:
//             current->args()
            break;
        case STMT_FOR: break;
        case STMT_WHILE: break;
        case STMT_DO_WHILE: break;
        case STMT_RETURN: break;
        case STMT_CONTINUE: break;
        case STMT_BREAK: break;
        case STMT_SWITCH: break;
        case STMT_CASE: break;
        case STMT_DEFAULT: break;
        case STMT_DELETE: break;

        // These statements should have been already processed in a previous phase of script analysis.
        case STMT_TRIGGER:
        case STMT_FUNCTION:
            break;
    }
}

inline void MScriptInterpret::checkCode ( MScriptStatement * rootNode )
{
    
}
