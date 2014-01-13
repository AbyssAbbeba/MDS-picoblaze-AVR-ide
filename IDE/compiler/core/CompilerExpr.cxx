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
 * @ingroup Compiler
 * @file CompilerExpr.cxx
 */
// =============================================================================

#include "CompilerExpr.h"

// Standard header files.
#include <cstring>
#include <cctype>
#include <cstdlib>

CompilerExpr::CompilerExpr ( CompilerSerializer & input )
{
    deserialize(input);
}

CompilerExpr::CompilerExpr ( CompilerSourceLocation location )
{
    m_operator = OPER_NONE;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

CompilerExpr::CompilerExpr ( CompilerValue value,
                             CompilerSourceLocation location )
{
    m_lValue = value;
    m_operator = OPER_NONE;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

CompilerExpr::CompilerExpr ( Operator oper,
                             CompilerValue value,
                             CompilerSourceLocation location )
{
    m_operator = oper;
    m_rValue = value;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

CompilerExpr::CompilerExpr ( char oper,
                             CompilerValue value,
                             CompilerSourceLocation location )
{
    m_operator = Operator(oper);
    m_rValue = value;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

CompilerExpr::CompilerExpr ( CompilerValue value,
                             Operator oper,
                             CompilerSourceLocation location )
{
    m_operator = Operator(oper);
    m_lValue = value;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

CompilerExpr::CompilerExpr ( CompilerValue value,
                             char oper,
                             CompilerSourceLocation location )
{
    m_operator = Operator(oper);
    m_lValue = value;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

CompilerExpr::CompilerExpr ( CompilerValue lValue,
                             Operator oper,
                             CompilerValue rValue,
                             CompilerSourceLocation location )
{
    m_lValue = lValue;
    m_operator = oper;
    m_rValue = rValue;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

CompilerExpr::CompilerExpr ( CompilerValue lValue,
                             char oper,
                             CompilerValue rValue,
                             CompilerSourceLocation location )
{
    m_lValue = lValue;
    m_operator = Operator(oper);
    m_rValue = rValue;
    m_location = location;

    m_next = nullptr;
    m_prev = nullptr;
}

CompilerExpr * CompilerExpr::first()
{
    CompilerExpr * expr = this;

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

CompilerExpr * CompilerExpr::last()
{
    CompilerExpr * result = this;
    while ( nullptr != result->m_next )
    {
        result = result->m_next;
    }
    return result;
}

CompilerExpr * CompilerExpr::insertLink ( CompilerExpr * chainLink )
{
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
    {
        return chainLink;
    }

    CompilerExpr * chainLinkOrig = chainLink;
    CompilerExpr * chainLinkLast = chainLink->last();
    CompilerExpr * nextOrig = m_next;

    chainLink = chainLink->first();

    m_next = chainLink;
    chainLink->m_prev = this;

    chainLinkLast->m_next = nextOrig;
    nextOrig->m_prev = chainLinkLast;

    return chainLinkOrig;
}

CompilerExpr * CompilerExpr::appendLink ( CompilerExpr * chainLink )
{
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
    {
        return chainLink;
    }

    CompilerExpr * chainLinkOrig = chainLink;
    CompilerExpr * lastLink = last();

    chainLink = chainLink->first();
    lastLink->m_next = chainLink;
    chainLink->m_prev = lastLink;

    return chainLinkOrig;
}

CompilerExpr * CompilerExpr::prependLink ( CompilerExpr * chainLink )
{
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
    {
        return chainLink;
    }

    CompilerExpr * chainLinkOrig = chainLink;
    CompilerExpr * firstLink = first();

    chainLink = chainLink->last();
    chainLink->m_next = firstLink;
    firstLink->m_prev = chainLink;

    return chainLinkOrig;
}

void CompilerExpr::completeDelete ( CompilerExpr * expr )
{
    expr->completeDelete();
}

void CompilerExpr::completeDelete()
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

CompilerExpr * CompilerExpr::operator [] ( int index )
{
    CompilerExpr * result = this;
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

CompilerExpr * CompilerExpr::copyEntireChain() const
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    CompilerExpr * result = copyChainLink();

    const CompilerExpr * next = this;
    while ( nullptr != ( next = next->m_next ) )
    {
        result->appendLink(next->copyChainLink());
    }

    const CompilerExpr * prev = this;
    while ( nullptr != ( prev = prev->m_prev ) )
    {
        result->prependLink(prev->copyChainLink());
    }

    return result;
}

CompilerExpr * CompilerExpr::copyChainLink() const
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    CompilerExpr * result = new CompilerExpr();

    CompilerValue * val;

    val = m_lValue.makeCopy();
    result->m_lValue = *val;
    delete val;

    val = m_rValue.makeCopy();
    result->m_rValue = *val;
    delete val;

    result->m_operator = m_operator;
    result->m_location = m_location;

    return result;
}

CompilerExpr * CompilerExpr::unlink()
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

std::ostream & operator << ( std::ostream & out,
                             const CompilerExpr::Operator & opr )
{
    switch ( opr )
    {
        case CompilerExpr::OPER_NONE:                           break;
        case CompilerExpr::OPER_ADD:         out << "+";        break;
        case CompilerExpr::OPER_SUB:         out << "-";        break;
        case CompilerExpr::OPER_MULT:        out << "*";        break;
        case CompilerExpr::OPER_DIV:         out << "/";        break;
        case CompilerExpr::OPER_MOD:         out << "%";        break;
        case CompilerExpr::OPER_DOT:         out << ".";        break;
        case CompilerExpr::OPER_BOR:         out << "|";        break;
        case CompilerExpr::OPER_BXOR:        out << "^";        break;
        case CompilerExpr::OPER_BAND:        out << "&";        break;
        case CompilerExpr::OPER_LOR:         out << "||";       break;
        case CompilerExpr::OPER_LXOR:        out << "^^";       break;
        case CompilerExpr::OPER_LAND:        out << "&&";       break;
        case CompilerExpr::OPER_LOW:         out << "LOW ";     break;
        case CompilerExpr::OPER_HIGH:        out << "HIGH ";    break;
        case CompilerExpr::OPER_EQ:          out << "==";       break;
        case CompilerExpr::OPER_NE:          out << "!=";       break;
        case CompilerExpr::OPER_LT:          out << "<";        break;
        case CompilerExpr::OPER_LE:          out << "<=";       break;
        case CompilerExpr::OPER_GE:          out << ">=";       break;
        case CompilerExpr::OPER_GT:          out << ">";        break;
        case CompilerExpr::OPER_SHR:         out << ">>";       break;
        case CompilerExpr::OPER_SHL:         out << "<<";       break;
        case CompilerExpr::OPER_CALL:        out << "<CALL>";   break;
        case CompilerExpr::OPER_INT_PROM:    out << "<+pos>";   break;
        case CompilerExpr::OPER_ADD_INV:     out << "<-neg>";   break;
        case CompilerExpr::OPER_CMPL:        out << "~";        break;
        case CompilerExpr::OPER_NOT:         out << "!";        break;
        case CompilerExpr::OPER_ADD_ASSIGN:  out << "+=";       break;
        case CompilerExpr::OPER_SUB_ASSIGN:  out << "-=";       break;
        case CompilerExpr::OPER_MUL_ASSIGN:  out << "*=";       break;
        case CompilerExpr::OPER_DIV_ASSIGN:  out << "/=";       break;
        case CompilerExpr::OPER_MOD_ASSIGN:  out << "%=";       break;
        case CompilerExpr::OPER_SHL_ASSIGN:  out << "<<=";      break;
        case CompilerExpr::OPER_SHR_ASSIGN:  out << ">>=";      break;
        case CompilerExpr::OPER_AND_ASSIGN:  out << "&=";       break;
        case CompilerExpr::OPER_ORB_ASSIGN:  out << "|=";       break;
        case CompilerExpr::OPER_XOR_ASSIGN:  out << "^=";       break;
        case CompilerExpr::OPER_INC:         out << "++";       break;
        case CompilerExpr::OPER_DEC:         out << "--";       break;
        case CompilerExpr::OPER_AT:          out << "@";        break;
        case CompilerExpr::OPER_INTERVALS:   out << "..";       break;
        case CompilerExpr::OPER_ASSIGN:      out << "=";        break;
        case CompilerExpr::OPER_NAND:        out << "!&";       break;
        case CompilerExpr::OPER_HASH:        out << "#";        break;
        case CompilerExpr::OPER_REF:         out << "&";        break;
        case CompilerExpr::OPER_DEREF:       out << "*";        break;
        case CompilerExpr::OPER_TERNARY:     out << "?:";       break;
        case CompilerExpr::OPER_COLON:       out << ":";        break;
        case CompilerExpr::OPER_INDEX:       out << "[]";       break;
        case CompilerExpr::OPER_POST_INC:    out << "post++";   break;
        case CompilerExpr::OPER_POST_DEC:    out << "post--";   break;
        case CompilerExpr::OPER_PRE_INC:     out << "++pre";    break;
        case CompilerExpr::OPER_PRE_DEC:     out << "--pre";    break;
        
    }
    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerExpr * const expr )
{
    if ( nullptr == expr )
    {
        out << "(<nullptr>)";
        return out;
    }

    if ( CompilerExpr::OPER_NONE == expr->m_operator )
    {
        out << "(" << expr->lVal() << ")";
    }
    else
    {
        out << "(" << expr->lVal() << " " << expr->m_operator << " " << expr->rVal() << std::string(")");
    }

    out << " {";
    out << expr->location();
    out << "}";

    if ( nullptr != expr->next() )
    {
        out << " | " << expr->next();
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerExpr & expr )
{
    out << &expr;
    return out;
}

void CompilerExpr::serialize ( CompilerSerializer & output ) const
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

void CompilerExpr::deserialize ( CompilerSerializer & input )
{
    m_next = nullptr;
    m_prev = nullptr;

    m_operator =  Operator(input.read_ui16());
    input >> m_lValue >> m_rValue >> m_location;

    SerializationMark mark;
    input >> mark;
    if ( MARK_NEXT == mark )
    {
        m_next = new CompilerExpr(input);
        m_next->m_prev = this;
    }
}
