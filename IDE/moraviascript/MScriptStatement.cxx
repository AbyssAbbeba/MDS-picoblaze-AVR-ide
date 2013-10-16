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
 * @file MScriptStatement.cxx
 */
// =============================================================================

#include "MScriptStatement.h"

// MScript language interpreter header files.
#include "MScriptExpr.h"

// Standard header files.
#include <cstdio>

MScriptStatement::MScriptStatement()
{
    m_type = MScriptStmtTypes::STMT_EMPTY;
    m_args = NULL;
    m_next = NULL;
    m_prev = NULL;
    m_branch = NULL;
}

MScriptStatement::MScriptStatement ( const MScriptSrcLocation & location,
                                     MScriptStmtTypes::Type type,
                                     MScriptExpr * args )
{
    m_location = location;
    m_type = type;
    m_next = NULL;
    m_prev = NULL;
    m_branch = NULL;

    if ( NULL != args )
    {
        m_args = args->first();
    }
    else
    {
        m_args = NULL;
    }
}

MScriptStatement::MScriptStatement ( MScriptSerializer & input )
{
    deserialize(input);

    MScriptStatement * currentNode = this;
    MScriptStatement * newNode;
    SerializationMark mark;

    input >> mark;
    while ( MARK_NEXT == mark )
    {
        newNode = new MScriptStatement();
        input >> newNode;

        currentNode->m_next = newNode;
        newNode->m_prev = currentNode;
        currentNode = newNode;

        input >> mark;
    }
}

MScriptStatement::~MScriptStatement()
{
    if ( NULL != m_args )
    {
        m_args->completeDelete();
    }
    if ( NULL != m_branch )
    {
        m_branch->completeDelete();
    }
    if ( NULL != m_prev )
    {
        m_prev->m_next = m_next;
    }
    if ( NULL != m_next )
    {
        m_next->m_prev = m_prev;
    }
}

MScriptStatement * MScriptStatement::createBranch ( MScriptStatement * branch )
{
    if ( NULL == this )
    {
        return NULL;
    }
    if ( NULL == branch )
    {
        return this;
    }

    m_branch = branch->first();
    return this;
}

MScriptStatement * MScriptStatement::first()
{
    if ( NULL == this )
    {
        return NULL;
    }

    MScriptStatement * stmt = this;

    while ( NULL != stmt->m_prev )
    {
        stmt = stmt->m_prev;
    }

    return stmt;
}

MScriptStatement * MScriptStatement::last()
{
    MScriptStatement * result = this;
    while ( NULL != result->m_next )
    {
        result = result->m_next;
    }
    return result;
}

MScriptStatement * MScriptStatement::insertLink ( MScriptStatement * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    MScriptStatement * chainLinkOrig = chainLink;
    MScriptStatement * chainLinkLast = chainLink->last();
    MScriptStatement * nextOrig = m_next;

    chainLink = chainLink->first();

    m_next = chainLink;
    chainLink->m_prev = this;

    chainLinkLast->m_next = nextOrig;
    if ( NULL != nextOrig )
    {
        nextOrig->m_prev = chainLinkLast;
    }

    return chainLinkOrig;
}

MScriptStatement * MScriptStatement::appendLink ( MScriptStatement * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    chainLink = chainLink->first();

    MScriptStatement * stmt = this;
    while ( NULL != stmt->next() )
    {
        stmt = stmt->next();
    }
    stmt->m_next = chainLink;
    chainLink->m_prev = stmt;

    return chainLink;
}

MScriptStatement * MScriptStatement::prependLink ( MScriptStatement * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    MScriptStatement * chainLinkOrig = chainLink;
    MScriptStatement * firstLink = first();

    chainLink = chainLink->last();
    chainLink->m_next = firstLink;
    firstLink->m_prev = chainLink;

    return chainLinkOrig;
}

MScriptStatement * MScriptStatement::unlink()
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

MScriptStatement * MScriptStatement::copyEntireChain() const
{
    if ( NULL == this )
    {
        return NULL;
    }

    MScriptStatement * result = copyChainLink();

    const MScriptStatement * next = this;
    while ( NULL != ( next = next->m_next ) )
    {
        result->appendLink(next->copyChainLink());
    }

    const MScriptStatement * prev = this;
    while ( NULL != ( prev = prev->m_prev ) )
    {
        result->prependLink(prev->copyChainLink());
    }

    return result;
}

MScriptStatement * MScriptStatement::copyChainLink() const
{
    if ( NULL == this )
    {
        return NULL;
    }

    MScriptStatement * result = new MScriptStatement(m_location, m_type, m_args->copyEntireChain());
    result->m_branch = m_branch->copyEntireChain();

    return result;
}

MScriptStatement * MScriptStatement::appendArgsLink ( MScriptExpr * chainLink )
{
    if ( NULL == this )
    {
        return NULL;
    }
    if ( NULL == chainLink )
    {
        return this;
    }

    if ( NULL == m_args )
    {
        m_args = chainLink;
    }
    else
    {
        m_args->appendLink(chainLink);
    }
    return this;
}

void MScriptStatement::completeDelete ( MScriptStatement * stmt )
{
    stmt->completeDelete();
}

void MScriptStatement::completeDelete()
{
    if ( NULL == this )
    {
        return;
    }

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

void MScriptStatement::serializeTree ( MScriptSerializer & output ) const
{
    for ( const MScriptStatement * node = this;
          NULL != node;
          node = node->next() )
    {
        output << node;
    }
}

void MScriptStatement::serialize ( MScriptSerializer & output ) const
{
    output.write ( (uint32_t) m_type );
    output << m_location;

    if ( NULL == m_args )
    {
        output << MARK_TERMINAL;
    }
    else
    {
        output << MARK_NEXT;
        output << m_args;
    }

    if ( NULL == m_branch )
    {
        output << MARK_TERMINAL;
    }
    else
    {
        output << MARK_NEXT;
        m_branch->serializeTree(output);
    }

    if ( NULL == m_next )
    {
        output << MARK_TERMINAL;
    }
    else
    {
        output << MARK_NEXT;
    }
}

void MScriptStatement::deserialize ( MScriptSerializer & input )
{
    m_prev = NULL;
    m_next = NULL;
    m_branch = NULL;
    m_args = NULL;

    m_type = MScriptStmtTypes::Type ( input.read_ui32() );
    input >> m_location;

    SerializationMark mark;

    input >> mark;
    if ( MARK_NEXT == mark )
    {
        m_args = new MScriptExpr(input);
    }

    input >> mark;
    if ( MARK_NEXT == mark )
    {
        m_branch = new MScriptStatement(input);
    }
}

std::ostream & MScriptStatement::print ( std::ostream & out,
                                         int level,
                                         std::string lineString ) const
{
    char addr[19];
    sprintf ( addr,
              "0x%08x%08x",
              (unsigned int) ( ( 0xffffffff00000000 & reinterpret_cast<long long unsigned int>(this) ) >> 32),
              (unsigned int) (   0x00000000ffffffff & reinterpret_cast<long long unsigned int>(this) ) );
    out << addr << " ";

    if ( NULL == this )
    {
        out << "<ERROR:NULL!>" << std::endl;
        return out;
    }

    for ( int i = 0; i < level; i++ )
    {
        if ( '0' == lineString[i] )
        {
            out << "    ";
        }
        else
        {
            out << "  │ ";
        }
    }

    if ( NULL == m_prev )
    {
        if ( NULL == m_next )
        {
            lineString[level] = '0';
        }
        out << "  █─ ";
    }
    else
    {
        if ( NULL == m_next )
        {
            out << "  └─ ";
            lineString[level] = '0';
        }
        else
        {
            out << "  ├─ ";
        }
    }

    out << m_type;

    out << " {";
    out << m_location;
    out << "}";

    if ( NULL != m_args )
    {
        out << " [ ";
        out << m_args;
        out << " ]";
    }
    out << std::endl;

    if ( NULL != m_branch )
    {
        lineString += "1";
        m_branch->print(out, level + 1, lineString);
    }

    if ( NULL != m_next )
    {
        m_next->print(out, level, lineString);
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptStatement * stmt )
{
    return stmt->print(out);
}
