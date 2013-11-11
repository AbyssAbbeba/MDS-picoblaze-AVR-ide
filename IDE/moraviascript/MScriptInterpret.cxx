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
#include "MScriptExpr.h"
#include "MScriptVarTable.h"
#include "MScriptFuncTable.h"
#include "MScriptStatement.h"
#include "MScriptExprSolver.h"
#include "MScriptExprProcessor.h"
#include "MScriptNamespaces.h"

// Used for i18n only.
#include <QObject>

#include <iostream> // DEBUG

MScriptInterpret::MScriptInterpret()
{
    m_namespaces    = new MScriptNamespaces(this);
    m_varTable      = new MScriptVarTable(this,m_namespaces);
    m_funcTable     = new MScriptFuncTable(this,m_namespaces);
    m_exprSolver    = new MScriptExprSolver(this, m_varTable);
    m_exprProcessor = new MScriptExprProcessor(this);
}

MScriptInterpret::~MScriptInterpret()
{
    delete m_namespaces;
    delete m_varTable;
    delete m_funcTable;
    delete m_exprSolver;
    delete m_exprProcessor;
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
    m_varTable->clear();
    m_funcTable->clear();
    m_namespaces->clear();
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
        case STMT_VAR:          evalVar ( node );       break;
        case STMT_CONST:        evalConst ( node );     break;
        case STMT_CALL:         evalCall ( node );      break;
        case STMT_TRIGGER:      evalFunction ( node );  break;
        case STMT_NAMESPACE:    evalNamespace ( node ); break;
        case STMT_INCLUDE:      evalInclude ( node );   break;
        case STMT_EVAL:         evalEval ( node );      break;

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
        m_exprSolver->eval ( node->args()[2] );
    }
    else
    {
        /*
         * First iteration.
         */

        // for ( <EVAL_THIS>; ...; ... ) { ... }
        m_exprSolver->eval ( node->args()[0] );
    }

    // for ( ...; <EVAL_THIS>; ... ) { ... }
    const MScriptExpr & e = node->args()[1];
    if ( true == m_exprSolver->eval(e).toBool(this, e.location()) )
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
        if ( true == m_exprSolver->eval(node->args()).toBool(this, node->args()->location()) )
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
    if ( true == m_exprSolver->eval(node->args()).toBool(this, node->args()->location()) )
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
    if ( true == m_exprSolver->eval(node->args()).toBool(this, node->args()->location()) )
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
    m_exprSolver->eval(node->args());
    replaceNext(node->next());
}

inline void MScriptInterpret::evalScope ( const MScriptStatement * node )
{
    if ( FLAG_SCOPE & getNextFlags() )
    {
        // Leaving scope.
        replaceNext(node->next());
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

    const MScriptValue switchArg = m_exprSolver->eval(node->args());
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
                    if ( switchArg == m_exprSolver->eval(arg) )
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
    long long arg = 1;
    if ( NULL != node->args() )
    {
        arg = m_exprSolver->eval(node->args()).toInt(this, node->args()->location());
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
                             QObject::tr("break command used outside of a loop or switch").toStdString() );
    }
}

inline void MScriptInterpret::evalContinue ( const MScriptStatement * node )
{
    long long arg = 1;
    if ( NULL != node->args() )
    {
        arg = m_exprSolver->eval(node->args()).toInt(this, node->args()->location());
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
                             QObject::tr("continue command used outside of a loop").toStdString() );
    }
}

inline void MScriptInterpret::evalReturn ( const MScriptStatement * node )
{
    MScriptValue returnValue;
    if ( NULL != node->args() )
    {
        returnValue = m_exprSolver->eval(node->args());
    }

    if ( true == abadonCode(FLAG_FUNCTION) )
    {
        m_namespaces->leave();
        m_varTable->assign("0", node->location(), returnValue);
    }
    else
    {
        interpreterMessage ( node->location(),
                             MScriptBase::MT_ERROR,
                             QObject::tr("there is no function to return from").toStdString() );
    }
}

inline void MScriptInterpret::evalDelete ( const MScriptStatement * node )
{
    m_varTable -> remove ( node->args()->lVal().m_data.m_symbol,
                           node->location() );

    replaceNext(node->next());
}

inline void MScriptInterpret::evalVar ( const MScriptStatement * node )
{
    m_exprSolver->declaration ( node->args() );
}

inline void MScriptInterpret::evalConst ( const MScriptStatement * node )
{
    m_exprSolver->declaration ( node->args(), MScriptExprSolver::DF_CONST );
}

inline void MScriptInterpret::evalCall ( const MScriptStatement * node )
{
    if ( FLAG_FUNCTION & getNextFlags() )
    {
        // Returning from function.
        m_namespaces->leave();
        m_varTable->assign("0", node->location(), MScriptValue());
        replaceNext(node->next());
    }
    else
    {
        // Entering function.

        const char * name = node->args()->lVal().m_data.m_symbol;
        std::vector<MScriptValue> arguments;

        for ( const MScriptExpr * arg = node->args()->next();
              NULL != arg;
              arg = arg->next() )
        {
            arguments.push_back(m_exprSolver->eval(arg));
        }

        MScriptFuncTable::Function * func = m_funcTable->get(name, node->location(), arguments);

        m_namespaces->enter(func->m_ns);
        setNextFlags ( FLAG_FUNCTION );
        addNext ( func->m_code );

        // Enter the function's scope.
        step();

        // Initialize function's parameters as local variables in that scope.
        for ( size_t i = 0; i < func->m_params->size(); i++ )
        {
            const MScriptValue * arg;
            if ( i >= arguments.size() )
            {
                arg = func->m_params->at(i).m_value;
            }
            else
            {
                arg = &( arguments[i] );
            }

            const std::string & paramName = func->m_params->at(i).m_name;
            if ( true == func->m_params->at(i).m_reference )
            {
                if ( MScriptValue::TYPE_SYMBOL != arg->m_type )
                {
                    interpreterMessage ( node->location(),
                                         MScriptBase::MT_ERROR,
                                         QObject::tr ( "only variables can be referenced" ) . toStdString() );
                    return;
                }
                m_varTable -> refer ( paramName, arg->m_data.m_symbol, node->location() );
            }
            else
            {
                m_varTable -> assign ( paramName, node->location(), arg );
            }
        }
    }
}

inline void MScriptInterpret::evalFunction ( const MScriptStatement * node )
{
    const char * funcName = node->args()->lVal().m_data.m_symbol;
    std::vector<MScriptFuncTable::Parameter> * params = new std::vector<MScriptFuncTable::Parameter>;

    for ( MScriptExpr * expr = node->args()->next();
          NULL != expr;
          expr = expr->next() )
    {
        const char * param = expr->lVal().m_data.m_expr->lVal().m_data.m_symbol;

        if ( MScriptExpr::OPER_ASSIGN == expr->oper() )
        {
            const MScriptExpr * defVal = expr->rVal().m_data.m_expr;
            if ( true == m_exprProcessor->isConstantExpr ( defVal ) )
            {
                params->push_back ( MScriptFuncTable::Parameter ( param, m_exprSolver->eval(defVal) ) );
            }
            else
            {
                interpreterMessage ( defVal->location(),
                                     MScriptBase::MT_ERROR,
                                     QObject::tr ( "default value of function parameter has to be a constant "
                                                   "expression" ) . toStdString() );
                return;
            }
        }
        else if ( MScriptExpr::OPER_REF == expr->oper() )
        {
            params->push_back ( MScriptFuncTable::Parameter ( param, true ) );
        }
        else
        {
            params->push_back ( MScriptFuncTable::Parameter ( param ) );
        }
    }

    m_funcTable->define(funcName, node->location(), params, node->branch());
}

inline void MScriptInterpret::evalNamespace ( const MScriptStatement * node )
{
    if ( FLAG_NAMESPACE & getNextFlags() )
    {
        // Leaving namespace.
        replaceNext(node->next());
        m_namespaces->leave();
    }
    else
    {
        // Entering namespace.
        setNextFlags ( FLAG_NAMESPACE );
        addNext(node->branch());
        m_namespaces->defineEnter ( node->location(), node->args()->lVal().m_data.m_symbol );
    }
}

inline void MScriptInterpret::evalInclude ( const MScriptStatement * node )
{
    const MScriptValue & arg = node->args()->lVal();
    const std::string filename = std::string(arg.m_data.m_string.m_data, arg.m_data.m_string.m_size);

    MScriptStatement * code = include ( node->location(), filename );
    postprocessCode(code);

    const_cast<MScriptStatement*>(node)->insertLink(code);
}

inline void MScriptInterpret::evalEval ( const MScriptStatement * node )
{
    const MScriptValue & arg = node->args()->lVal();
    MScriptStatement * code = insertCode ( node->location(),
                                           std::string(arg.m_data.m_string.m_data, arg.m_data.m_string.m_size) );

    postprocessCode(code);

    const_cast<MScriptStatement*>(node)->insertLink(code);
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
            // Leave scope.
            m_varTable->popScope();
        }
        if ( FLAG_NAMESPACE & it->second )
        {
            // Leave namespace.
            m_namespaces->leave();
        }

        if ( upTo & it->second )
        {
            times--;
            if ( 0 == times )
            {
                const MScriptStatement * last = cutOffBranch(level);
                if ( false == exclusive )
                {
                    replaceNext(last);
                }

                return true;
            }
        }

        level++;
    }

    return false;
}

bool MScriptInterpret::postprocessCode ( MScriptStatement * rootNode )
{
    using namespace MScriptStmtTypes;

    for ( MScriptStatement * node = rootNode->next();
          node != NULL;
          node = node->next() )
    {
        if ( NULL != node->branch() )
        {
            postprocessCode(node->branch());
            if ( NULL == node->branch()->next() )
            {
                node->m_branch = NULL;
            }
        }

        switch ( node->type() )
        {
            case STMT_EMPTY:
                // Remove empty statements.
                break;

            case STMT_EXPR:
                // Decompose expression into a sequence of trivial expressions.
                node->insertLink(m_exprProcessor->decompose(node));
                break;

            case STMT_TRIGGER:
                // TODO: add to trigger table.
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
