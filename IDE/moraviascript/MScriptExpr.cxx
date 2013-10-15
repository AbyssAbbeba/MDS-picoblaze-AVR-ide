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
 * @file MScriptExpr.cxx
 */
// =============================================================================

#include "MScriptExpr.h"

// Standard header files.
#include <cstring>
#include <cctype>
#include <cstdlib>

MScriptExpr::MScriptExpr ( MScriptSrcLocation location )
{
    m_operator = OPER_NONE;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( MScriptValue value,
                           MScriptSrcLocation location )
{
    m_lValue = value;
    m_operator = OPER_NONE;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( Operator oper,
                           MScriptValue value,
                           MScriptSrcLocation location )
{
    m_operator = oper;
    m_rValue = value;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( MScriptValue value,
                           Operator oper,
                           MScriptSrcLocation location )
{
    m_operator = Operator(oper);
    m_lValue = value;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr::MScriptExpr ( MScriptValue lValue,
                           Operator oper,
                           MScriptValue rValue,
                           MScriptSrcLocation location )
{
    m_lValue = lValue;
    m_operator = oper;
    m_rValue = rValue;
    m_location = location;

    m_next = NULL;
    m_prev = NULL;
}

MScriptExpr * MScriptExpr::first()
{
    MScriptExpr * expr = this;

    if ( NULL == this )
    {
        return NULL;
    }

    while ( NULL != expr->m_prev )
    {
        expr = expr->m_prev;
    }

    return expr;
}

MScriptExpr * MScriptExpr::last()
{
    MScriptExpr * result = this;
    while ( NULL != result->m_next )
    {
        result = result->m_next;
    }
    return result;
}

MScriptExpr * MScriptExpr::insertLink ( MScriptExpr * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    MScriptExpr * chainLinkOrig = chainLink;
    MScriptExpr * chainLinkLast = chainLink->last();
    MScriptExpr * nextOrig = m_next;

    chainLink = chainLink->first();

    m_next = chainLink;
    chainLink->m_prev = this;

    chainLinkLast->m_next = nextOrig;
    nextOrig->m_prev = chainLinkLast;

    return chainLinkOrig;
}

MScriptExpr * MScriptExpr::appendLink ( MScriptExpr * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    MScriptExpr * chainLinkOrig = chainLink;
    MScriptExpr * lastLink = last();

    chainLink = chainLink->first();
    lastLink->m_next = chainLink;
    chainLink->m_prev = lastLink;

    return chainLinkOrig;
}

MScriptExpr * MScriptExpr::prependLink ( MScriptExpr * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    MScriptExpr * chainLinkOrig = chainLink;
    MScriptExpr * firstLink = first();

    chainLink = chainLink->last();
    chainLink->m_next = firstLink;
    firstLink->m_prev = chainLink;

    return chainLinkOrig;
}

void MScriptExpr::completeDelete ( MScriptExpr * expr )
{
    expr->completeDelete();
}

void MScriptExpr::completeDelete()
{
    if ( NULL == this )
    {
        return;
    }

    m_lValue.completeDelete();
    m_rValue.completeDelete();

    if ( NULL != m_next )
    {
        m_next->m_prev = NULL;
        m_next->completeDelete();
        m_next = NULL;
    }

    if ( NULL != m_prev )
    {
        m_prev->m_next = NULL;
        m_prev->completeDelete();
        m_prev = NULL;
    }

    delete this;
}

MScriptExpr * MScriptExpr::operator [] ( int index )
{
    MScriptExpr * result = this;
    for ( int i = 0; i < index; i++ )
    {
        result = result->m_next;
        if ( NULL == result )
        {
            return NULL;
        }
    }
    return result;
}

MScriptExpr * MScriptExpr::copyEntireChain() const
{
    if ( NULL == this )
    {
        return NULL;
    }

    MScriptExpr * result = copyChainLink();

    const MScriptExpr * next = this;
    while ( NULL != ( next = next->m_next ) )
    {
        result->appendLink(next->copyChainLink());
    }

    const MScriptExpr * prev = this;
    while ( NULL != ( prev = prev->m_prev ) )
    {
        result->prependLink(prev->copyChainLink());
    }

    return result;
}

MScriptExpr * MScriptExpr::copyChainLink() const
{
    if ( NULL == this )
    {
        return NULL;
    }

    MScriptExpr * result = new MScriptExpr();
    result->m_operator = m_operator;
    result->m_location = m_location;
    m_lValue.makeCopy(result->m_lValue);
    m_rValue.makeCopy(result->m_rValue);
    return result;
}

MScriptExpr * MScriptExpr::unlink()
{
    if ( NULL != m_next )
    {
        m_next->m_prev = m_prev;
        m_next = NULL;
    }
    if ( NULL != m_prev )
    {
        m_prev->m_next = m_next;
        m_prev = NULL;
    }
    return this;
}

void MScriptExpr::serialize ( MScriptSerializer & output ) const
{
}

void MScriptExpr::deserialize ( MScriptSerializer & input )
{
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptExpr::Operator & opr )
{
    switch ( opr )
    {
        case MScriptExpr::OPER_NONE:                            break;
        case MScriptExpr::OPER_ADD:             out << "+";     break;
        case MScriptExpr::OPER_SUB:             out << "-";     break;
        case MScriptExpr::OPER_MULT:            out << "*";     break;
        case MScriptExpr::OPER_DIV:             out << "/";     break;
        case MScriptExpr::OPER_MOD:             out << "%";     break;
        case MScriptExpr::OPER_BOR:             out << "|";     break;
        case MScriptExpr::OPER_BXOR:            out << "^";     break;
        case MScriptExpr::OPER_BAND:            out << "&";     break;
        case MScriptExpr::OPER_LOR:             out << "||";    break;
        case MScriptExpr::OPER_LAND:            out << "&&";    break;
        case MScriptExpr::OPER_EQ:              out << "==";    break;
        case MScriptExpr::OPER_NE:              out << "!=";    break;
        case MScriptExpr::OPER_LT:              out << "<";     break;
        case MScriptExpr::OPER_LE:              out << "<=";    break;
        case MScriptExpr::OPER_GE:              out << ">=";    break;
        case MScriptExpr::OPER_GT:              out << ">";     break;
        case MScriptExpr::OPER_SHR:             out << ">>";    break;
        case MScriptExpr::OPER_SHL:             out << "<<";    break;
        case MScriptExpr::OPER_CMPL:            out << "~";     break;
        case MScriptExpr::OPER_NOT:             out << "!";     break;
        case MScriptExpr::OPER_ADD_ASSIGN:      out << "+=";    break;
        case MScriptExpr::OPER_SUB_ASSIGN:      out << "-=";    break;
        case MScriptExpr::OPER_MUL_ASSIGN:      out << "*=";    break;
        case MScriptExpr::OPER_DIV_ASSIGN:      out << "/=";    break;
        case MScriptExpr::OPER_MOD_ASSIGN:      out << "%=";    break;
        case MScriptExpr::OPER_SHL_ASSIGN:      out << "<<=";   break;
        case MScriptExpr::OPER_SHR_ASSIGN:      out << ">>=";   break;
        case MScriptExpr::OPER_BAND_ASSIGN:     out << "&=";    break;
        case MScriptExpr::OPER_BOR_ASSIGN:      out << "|=";    break;
        case MScriptExpr::OPER_LAND_ASSIGN:     out << "&&=";   break;
        case MScriptExpr::OPER_LOR_ASSIGN:      out << "||=";   break;
        case MScriptExpr::OPER_XOR_ASSIGN:      out << "^=";    break;
        case MScriptExpr::OPER_ASSIGN:          out << "=";     break;
        case MScriptExpr::OPER_ASSIGN_REF:      out << "=&";    break;
        case MScriptExpr::OPER_TERNARY:         out << "?";     break;
        case MScriptExpr::OPER_COLON:           out << ":";     break;
        case MScriptExpr::OPER_COMMA:           out << ",";     break;

        case MScriptExpr::OPER_INDEX:           out << "idx[]"; break;
        case MScriptExpr::OPER_PRE_INC:         out << "++pre"; break;
        case MScriptExpr::OPER_PRE_DEC:         out << "--pre"; break;
        case MScriptExpr::OPER_POST_INC:        out << "post++";break;
        case MScriptExpr::OPER_POST_DEC:        out << "post--";break;
        case MScriptExpr::OPER_CALL:            out << "<call>";break;
        case MScriptExpr::OPER_REF:             out << "<&ref>";break;
        case MScriptExpr::OPER_INT_PROM:        out << "<+pos>";break;
        case MScriptExpr::OPER_ADD_INV:         out << "<-neg>";break;
    }
    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptExpr * const expr )
{
    if ( NULL == expr )
    {
        out << "(<ERROR:NULL!>)";
        return out;
    }

    if ( MScriptExpr::OPER_NONE == expr->m_operator )
    {
        out << "(" << expr->m_lValue << ")";
    }
    else
    {
        out << "(" << expr->m_lValue << " " << expr->m_operator << " " << expr->m_rValue << std::string(")");
    }

    out << " {";
    out << expr->m_location;
    out << "}";

    if ( NULL != expr->m_next )
    {
        out << " | " << expr->m_next;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptExpr & expr )
{
    out << &expr;
    return out;
}
