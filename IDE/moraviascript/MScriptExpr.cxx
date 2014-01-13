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
 * @file MScriptExpr.cxx
 */
// =============================================================================

#include "MScriptExpr.h"

// Standard header files.
#include <cstring>
#include <cctype>
#include <cstdlib>

MScriptExpr::MScriptExpr ( MScriptSerializer & input )
{
    deserialize(input);
}

MScriptExpr::MScriptExpr ( MScriptSrcLocation location )
{
    m_operator = OPER_NONE;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

MScriptExpr::MScriptExpr ( MScriptValue value,
                           MScriptSrcLocation location )
{
    m_lValue = value;
    m_operator = OPER_NONE;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

MScriptExpr::MScriptExpr ( Operator oper,
                           MScriptValue value,
                           MScriptSrcLocation location )
{
    m_operator = oper;
    m_rValue = value;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

MScriptExpr::MScriptExpr ( MScriptValue value,
                           Operator oper,
                           MScriptSrcLocation location )
{
    m_operator = Operator(oper);
    m_lValue = value;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
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

    m_next = nullptr;
    m_prev = nullptr;
}

MScriptExpr * MScriptExpr::first()
{
    MScriptExpr * expr = this;

    if ( nullptr == this )
    {
        return nullptr;
    }

    while ( nullptr != expr->m_prev )
    {
        expr = expr->m_prev;
    }

    return expr;
}

MScriptExpr * MScriptExpr::last()
{
    MScriptExpr * result = this;
    while ( nullptr != result->m_next )
    {
        result = result->m_next;
    }
    return result;
}

MScriptExpr * MScriptExpr::insertLink ( MScriptExpr * chainLink )
{
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
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
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
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
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
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
    if ( nullptr == this )
    {
        return;
    }

    m_lValue.completeDelete();
    m_rValue.completeDelete();

    if ( nullptr != m_next )
    {
        m_next->m_prev = nullptr;
        m_next->completeDelete();
        m_next = nullptr;
    }

    if ( nullptr != m_prev )
    {
        m_prev->m_next = nullptr;
        m_prev->completeDelete();
        m_prev = nullptr;
    }

    delete this;
}

MScriptExpr * MScriptExpr::operator [] ( int index )
{
    MScriptExpr * result = this;
    for ( int i = 0; i < index; i++ )
    {
        result = result->m_next;
        if ( nullptr == result )
        {
            return nullptr;
        }
    }
    return result;
}

MScriptExpr * MScriptExpr::copyEntireChain() const
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    MScriptExpr * result = copyChainLink();

    const MScriptExpr * next = this;
    while ( nullptr != ( next = next->m_next ) )
    {
        result->appendLink(next->copyChainLink());
    }

    const MScriptExpr * prev = this;
    while ( nullptr != ( prev = prev->m_prev ) )
    {
        result->prependLink(prev->copyChainLink());
    }

    return result;
}

MScriptExpr * MScriptExpr::copyChainLink() const
{
    if ( nullptr == this )
    {
        return nullptr;
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
    if ( nullptr != m_next )
    {
        m_next->m_prev = m_prev;
        m_next = nullptr;
    }
    if ( nullptr != m_prev )
    {
        m_prev->m_next = m_next;
        m_prev = nullptr;
    }
    return this;
}

void MScriptExpr::serialize ( MScriptSerializer & output ) const
{
    output.write ( (uint16_t) m_operator );
    output << m_lValue << m_rValue << m_location;

    if ( nullptr == m_next )
    {
        output << MARK_TERMINAL;
    }
    else
    {
        output << MARK_NEXT;
        output << m_next;
    }
}

void MScriptExpr::deserialize ( MScriptSerializer & input )
{
    m_next = nullptr;
    m_prev = nullptr;

    m_operator =  Operator(input.read_ui16());
    input >> m_lValue >> m_rValue >> m_location;

    SerializationMark mark;
    input >> mark;
    if ( MARK_NEXT == mark )
    {
        m_next = new MScriptExpr(input);
        m_next->m_prev = this;
    }
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
        case MScriptExpr::OPER_POW_ASSIGN:      out << "**=";   break;
        case MScriptExpr::OPER_ASSIGN:          out << "=";     break;
        case MScriptExpr::OPER_ASSIGN_REF:      out << "=&";    break;
        case MScriptExpr::OPER_TERNARY:         out << "?";     break;
        case MScriptExpr::OPER_COLON:           out << ":";     break;
        case MScriptExpr::OPER_COMMA:           out << ",";     break;
        case MScriptExpr::OPER_POW:             out << "**";    break;

        case MScriptExpr::OPER_INDEX:           out << "idx[]"; break;
        case MScriptExpr::OPER_PRE_INC:         out << "++pre"; break;
        case MScriptExpr::OPER_PRE_DEC:         out << "--pre"; break;
        case MScriptExpr::OPER_POST_INC:        out << "post++";break;
        case MScriptExpr::OPER_POST_DEC:        out << "post--";break;
        case MScriptExpr::OPER_CALL:            out << "<call>";break;
        case MScriptExpr::OPER_REF:             out << "<&ref>";break;
        case MScriptExpr::OPER_INT_PROM:        out << "<+pos>";break;
        case MScriptExpr::OPER_ADD_INV:         out << "<-neg>";break;

        case MScriptExpr::OPER_LENGTH:          out << "<length>";      break;
        case MScriptExpr::OPER_IS_BOOL:         out << "<is_bool>";     break;
        case MScriptExpr::OPER_IS_INT:          out << "<is_int>";      break;
        case MScriptExpr::OPER_IS_STRING:       out << "<is_string>";   break;
        case MScriptExpr::OPER_IS_FLOAT:        out << "<is_float>";    break;
        case MScriptExpr::OPER_IS_COMPLEX:      out << "<is_complex>";  break;
        case MScriptExpr::OPER_IS_EMPTY:        out << "<is_empty>";    break;
        case MScriptExpr::OPER_TO_BOOL:         out << "<to_bool>";     break;
        case MScriptExpr::OPER_TO_INT:          out << "<to_int>";      break;
        case MScriptExpr::OPER_TO_STRING:       out << "<to_string>";   break;
        case MScriptExpr::OPER_TO_FLOAT:        out << "<to_float>";    break;
        case MScriptExpr::OPER_TO_COMPLEX:      out << "<to_complex>";  break;
        case MScriptExpr::OPER_IS_NAN:          out << "<is_nan>";      break;
        case MScriptExpr::OPER_IS_INFINITY:     out << "<is_infinity>"; break;
        case MScriptExpr::OPER_IS_POSITIVE:     out << "<is_positive>"; break;
        case MScriptExpr::OPER_IS_NEGATIVE:     out << "<is_negative>"; break;
        case MScriptExpr::OPER_IS_FINITE:       out << "<is_finite>";   break;
        case MScriptExpr::OPER_REAL:            out << "<Re>";          break;
        case MScriptExpr::OPER_IMG_UNIT:        out << "<Im>";          break;
        case MScriptExpr::OPER_SIN:             out << "<sin>";         break;
        case MScriptExpr::OPER_COS:             out << "<cos>";         break;
        case MScriptExpr::OPER_TAN:             out << "<tan>";         break;
        case MScriptExpr::OPER_ARCSIN:          out << "<arcsin>";      break;
        case MScriptExpr::OPER_ARCCOS:          out << "<arccos>";      break;
        case MScriptExpr::OPER_ARCTAN:          out << "<arctan>";      break;
        case MScriptExpr::OPER_SINH:            out << "<sinh>";        break;
        case MScriptExpr::OPER_COSH:            out << "<cosh>";        break;
        case MScriptExpr::OPER_TANH:            out << "<tanh>";        break;
        case MScriptExpr::OPER_ARCSINH:         out << "<arcsinh>";     break;
        case MScriptExpr::OPER_ARCCOSH:         out << "<arccosh>";     break;
        case MScriptExpr::OPER_ARCTANH:         out << "<arctanh>";     break;
        case MScriptExpr::OPER_CEIL:            out << "<ceil>";        break;
        case MScriptExpr::OPER_ROUND:           out << "<round>";       break;
        case MScriptExpr::OPER_FLOOR:           out << "<floor>";       break;
        case MScriptExpr::OPER_ABS:             out << "<abs>";         break;
        case MScriptExpr::OPER_MIN:             out << "<min>";         break;
        case MScriptExpr::OPER_MAX:             out << "<max>";         break;
    }
    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptExpr * const expr )
{
    if ( nullptr == expr )
    {
        out << "(<ERROR:nullptr!>)";
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

    if ( nullptr != expr->m_next )
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
