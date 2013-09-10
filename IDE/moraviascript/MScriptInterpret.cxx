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
 * @ingroup MoraviaScript
 * @file MScriptInterpret.cxx
 */
// =============================================================================

#include "MScriptInterpret.h"

// MScript language interpreter header files.
#include "MScriptStatement.h"
#include "MScriptExpr.h"

// Used for i18n only
#include <QObject>
#include <iostream> // DEBUG

MScriptInterpret::MScriptInterpret()
{
    m_varTable = new MScriptVarTable(this);
    m_funcTable = new MScriptFuncTable(this);
}

MScriptInterpret::~MScriptInterpret()
{
    delete m_varTable;
    delete m_funcTable;
}

void MScriptInterpret::init ( MScriptStatement * rootNode )
{
    clear();
    if ( true == postprocessCode(rootNode) )
    {
        MScriptExecContext::init(rootNode);
    }
}

void MScriptInterpret::clear()
{
    MScriptExecContext::clear();
}

bool MScriptInterpret::step()
{
    using namespace MScriptStmtTypes;

    if ( true == MScriptExecContext::empty() )
    {
        // Program finished.
        return false;
    }

    const MScriptStatement * node = getNextNode();

    std::cout << "MScriptInterpret::step(): " << (void*)node << "\n";

    switch ( node->type() )
    {
        // These statements are supposed to be evaluated here.
        case STMT_EXPR:         evalExpr ( node );      break;
        case STMT_SCOPE:        evalScope ( node );     break;
        case STMT_FOR:          evalFor ( node );       break;
        case STMT_WHILE:        evalWhile ( node );     break;
        case STMT_DO_WHILE:     evalDoWhile ( node );   break;
        case STMT_CONDITION:    evalCondition ( node ); break;
        case STMT_SWITCH:       evalSwitch ( node );    break;
        case STMT_BREAK:        evalBreak ( node );     break;
        case STMT_CONTINUE:     evalContinue ( node );  break;
        case STMT_RETURN:       evalReturn ( node );    break;
        case STMT_DELETE:       evalDelete ( node );    break;

        // These statements are supposed to be ignored.
        case STMT_ROOT:
        case STMT_EMPTY:
            replaceNext(node->next());
            break;

        // These statements should never appear here.
        case STMT_CASE:
        case STMT_DEFAULT:
        case STMT_IF:
        case STMT_ELSE:
            break;

        // These statements should have been already processed in a previous phase of script analysis.
        case STMT_TRIGGER:
        case STMT_FUNCTION:
            break;
    }

    return true;
}

inline void MScriptInterpret::evalFor ( const MScriptStatement * node )
{
    // Check whether this is the first iteration or not.
    if ( FLAG_LOOP & getNextFlags() )
    {
        /*
         * Next iteration.
         */

        // for ( ...; ...; <EVAL_THIS> ) { ... }
        node->args()[2].eval();
    }
    else
    {
        /*
         * First iteration.
         */

        // for ( <EVAL_THIS>; ...; ... ) { ... }
        node->args()[0].eval();
    }

    // for ( ...; <EVAL_THIS>; ... ) { ... }
    if ( 0 != node->args()[1].eval() )
    {
        // Mark this node as a loop and "for" loop.
        setNextFlags ( FLAG_LOOP );

        // Condition positive => for ( ...; ...; ... ) { <ENTER_HERE> }
        addNext ( node->branch() );
    }
    else
    {
        // Condition negative => for ( ...; ...; ... ) { ... } <ENTER_HERE>
        replaceNext ( node->next() );
    }
}

inline void MScriptInterpret::evalDoWhile ( const MScriptStatement * node )
{
    // Check whether this is the first iteration or not.
    if ( FLAG_LOOP & getNextFlags() )
    {
        /*
         * Next iteration.
         */

        // do { ... } while ( <EVAL_THIS> ); ...
        if ( 0 != node->args()->eval() )
        {
            // Condition positive => do { <ENTER_HERE> } while ( ... ); ...
            addNext ( node->branch() );
        }
        else
        {
            // Condition negative => do { ... } while ( ... ); <ENTER_HERE>
            replaceNext ( node->next() );
        }
    }
    else
    {
        /*
         * First iteration.
         */

        // Mark this node as a loop.
        setNextFlags ( FLAG_LOOP );

        // do { <ENTER_HERE> } while ( ... ); ...
        addNext ( node->branch() );
    }
}

inline void MScriptInterpret::evalWhile ( const MScriptStatement * node )
{
    // Mark this node as a loop.
    setNextFlags ( FLAG_LOOP );

    // while ( <EVAL_THIS> ) { ... } ...
    if ( 0 != node->args()->eval() )
    {
        // Condition positive => while ( ... ) { <ENTER_HERE> } ...
        addNext ( node->branch() );
    }
    else
    {
        // Condition negative => while ( ... ) { ... } <ENTER_HERE>
        replaceNext ( node->next() );
    }
}

inline void MScriptInterpret::evalCondition ( const MScriptStatement * node )
{
    // if ( ... ) { ... } [ else { ... } ] <ENTER_HERE>
    replaceNext ( node->next() );

    // Go into the CONDITION branch, see code tree for details.
    node = node->branch();
    if ( NULL == node )
    {
        return;
    }

    // if ( <EVAL_THIS> ) { ... } [ else { ... } ] ...
    if ( 0 != node->args()->eval() )
    {
        // Condition positive => if ( ... ) { <ENTER_HERE> } [ else { ... } ] ...
        addNext ( node->branch() );
    }
    else if ( NULL != node->next() )
    {
        // Condition negative => if ( ... ) { ... } else { <ENTER_HERE> } ...
        addNext ( node->next()->branch() );
    }
}

inline void MScriptInterpret::evalExpr ( const MScriptStatement * node )
{
    node->args()->eval();
    replaceNext(node->next());
}

inline void MScriptInterpret::evalScope ( const MScriptStatement * node )
{
    if ( FLAG_SCOPE & getNextFlags() )
    {
        // Leaving scope.
        popNext();
        m_varTable->popScope();
    }
    else
    {
        // Entering scope.
        setNextFlags ( FLAG_SCOPE );
        addNext(node->branch());
        m_varTable->pushScope();
    }
}

inline void MScriptInterpret::evalSwitch ( const MScriptStatement * node )
{
    using namespace MScriptStmtTypes;

    if ( ( NULL == node->branch() ) || ( FLAG_SWITCH & getNextFlags() ) )
    {
        replaceNext(node->next());
        return;
    }

    setNextFlags ( FLAG_SWITCH );

    const int switchArg = node->args()->eval();
    bool positive = false;

    for ( const MScriptStatement * caseNode = node->branch();
          caseNode != NULL;
          caseNode = caseNode->next() )
    {
        if ( true == positive )
        {
            addNext(caseNode->branch());
        }
        else
        {
            if ( STMT_DEFAULT & caseNode->type() )
            {
                positive = true;
            }
            else if ( STMT_CASE & caseNode->type() )
            {
                for ( const MScriptExpr * arg = caseNode->args();
                      arg != NULL;
                      arg = arg->next() )
                {
                    if ( switchArg == arg->eval() )
                    {
                        positive = true;
                        break;
                    }
                }
            }
        }
    }
}

inline void MScriptInterpret::evalBreak ( const MScriptStatement * node )
{
    int arg = 1;
    if ( NULL != node->args() )
    {
        arg = node->args()->eval();
    }

    if ( arg <= 0 )
    {
        interpreterMessage ( node->location(),
                            MScriptBase::MT_ERROR,
                            QObject::tr("invalid argument of break command").toStdString() );
        return;
    }

    if ( false == abadonCode ( ExecFlags( FLAG_SWITCH | FLAG_LOOP ), (unsigned int) (arg) ) )
    {
        interpreterMessage ( node->location(),
                             MScriptBase::MT_ERROR,
                             QObject::tr("invalid use of break command").toStdString() );
    }
}

inline void MScriptInterpret::evalContinue ( const MScriptStatement * node )
{
    int arg = 1;
    if ( NULL != node->args() )
    {
        arg = node->args()->eval();
    }

    if ( arg <= 0 )
    {
        interpreterMessage ( node->location(),
                            MScriptBase::MT_ERROR,
                            QObject::tr("invalid argument of continue command").toStdString() );
        return;
    }

    if ( false == abadonCode(FLAG_LOOP, (unsigned int) (arg), true) )
    {
        interpreterMessage ( node->location(),
                             MScriptBase::MT_ERROR,
                             QObject::tr("invalid use of continue command").toStdString() );
    }
}

inline void MScriptInterpret::evalReturn ( const MScriptStatement * node )
{
    if ( false == abadonCode(FLAG_FUNCTION) )
    {
        interpreterMessage ( node->location(),
                             MScriptBase::MT_ERROR,
                             QObject::tr("there is no function to return from").toStdString() );
    }
}

inline bool MScriptInterpret::abadonCode ( ExecFlags upTo,
                                           unsigned int times,
                                           bool exclusive )
{
    unsigned int level = ( true == exclusive ) ? 0 : 1;

    for ( std::vector<ProgPtr>::reverse_iterator it = getProgramPointer().rbegin();
          it != getProgramPointer().rend();
          it++ )
    {
        if ( FLAG_SCOPE & it->second )
        {
            m_varTable->popScope();
        }

        if ( upTo & it->second )
        {
            times--;
            if ( 0 == times )
            {
                cutOffBranch(level);
                return true;
            }
        }

        level++;
    }

    return false;
}

inline void MScriptInterpret::evalDelete ( const MScriptStatement * node )
{
    m_varTable -> remove ( node->args()->lVal().m_data.m_symbol,
                           node->location() );

    replaceNext(node->next());
}

inline bool MScriptInterpret::postprocessCode ( MScriptStatement * rootNode )
{
    using namespace MScriptStmtTypes;

    for ( MScriptStatement * node = rootNode->next();
          node != NULL;
          node = node->next() )
    {
        switch ( node->type() )
        {
            case STMT_EMPTY:
                // Remove empty statements.
                break;

            case STMT_TRIGGER:
                // TODO: add to trigger table.
                break;

            case STMT_FUNCTION:
                // Add function to the Function Table.
//                 m_funcTable->define()
                break;

            default:
                // By default, nodes will not be removed from the tree.
                continue;
        }

        // Remove the inspected node from the tree.
        node = node->prev();
        delete node->next();
    }

    return true;
}
