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
    m_args = nullptr;
    m_next = nullptr;
    m_prev = nullptr;
    m_branch = nullptr;
}

MScriptStatement::MScriptStatement ( const MScriptSrcLocation & location,
                                     MScriptStmtTypes::Type type,
                                     MScriptExpr * args )
{
    m_location = location;
    m_type = type;
    m_next = nullptr;
    m_prev = nullptr;
    m_branch = nullptr;

    if ( nullptr != args )
    {
        m_args = args->first();
    }
    else
    {
        m_args = nullptr;
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
    if ( nullptr != m_args )
    {
        m_args->completeDelete();
    }
    if ( nullptr != m_branch )
    {
        m_branch->completeDelete();
    }
    if ( nullptr != m_prev )
    {
        m_prev->m_next = m_next;
    }
    if ( nullptr != m_next )
    {
        m_next->m_prev = m_prev;
    }
}

MScriptStatement * MScriptStatement::createBranch ( MScriptStatement * branch )
{
    if ( nullptr == this )
    {
        return nullptr;
    }
    if ( nullptr == branch )
    {
        return this;
    }

    m_branch = new MScriptStatement();
    m_branch->appendLink(branch);
    return this;
}

MScriptStatement * MScriptStatement::first()
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    MScriptStatement * stmt = this;

    while ( nullptr != stmt->m_prev )
    {
        stmt = stmt->m_prev;
    }

    return stmt;
}

MScriptStatement * MScriptStatement::last()
{
    MScriptStatement * result = this;
    while ( nullptr != result->m_next )
    {
        result = result->m_next;
    }
    return result;
}

MScriptStatement * MScriptStatement::insertLink ( MScriptStatement * chainLink )
{
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
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
    if ( nullptr != nextOrig )
    {
        nextOrig->m_prev = chainLinkLast;
    }

    return chainLinkOrig;
}

MScriptStatement * MScriptStatement::appendLink ( MScriptStatement * chainLink )
{
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
    {
        return chainLink;
    }

    chainLink = chainLink->first();

    MScriptStatement * stmt = this;
    while ( nullptr != stmt->next() )
    {
        stmt = stmt->next();
    }
    stmt->m_next = chainLink;
    chainLink->m_prev = stmt;

    return chainLink;
}

MScriptStatement * MScriptStatement::prependLink ( MScriptStatement * chainLink )
{
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
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

MScriptStatement * MScriptStatement::copyEntireChain() const
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    MScriptStatement * result = copyChainLink();

    const MScriptStatement * next = this;
    while ( nullptr != ( next = next->m_next ) )
    {
        result->appendLink(next->copyChainLink());
    }

    const MScriptStatement * prev = this;
    while ( nullptr != ( prev = prev->m_prev ) )
    {
        result->prependLink(prev->copyChainLink());
    }

    return result;
}

MScriptStatement * MScriptStatement::copyChainLink() const
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    MScriptStatement * result = new MScriptStatement(m_location, m_type, m_args->copyEntireChain());
    result->m_branch = m_branch->copyEntireChain();

    return result;
}

MScriptStatement * MScriptStatement::appendArgsLink ( MScriptExpr * chainLink )
{
    if ( nullptr == this )
    {
        return nullptr;
    }
    if ( nullptr == chainLink )
    {
        return this;
    }

    if ( nullptr == m_args )
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
    if ( nullptr == this )
    {
        return;
    }

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

void MScriptStatement::serializeTree ( MScriptSerializer & output ) const
{
    for ( const MScriptStatement * node = this;
          nullptr != node;
          node = node->next() )
    {
        output << node;
    }
}

void MScriptStatement::serialize ( MScriptSerializer & output ) const
{
    output.write ( (uint32_t) m_type );
    output << m_location;

    if ( nullptr == m_args )
    {
        output << MARK_TERMINAL;
    }
    else
    {
        output << MARK_NEXT;
        output << m_args;
    }

    if ( nullptr == m_branch )
    {
        output << MARK_TERMINAL;
    }
    else
    {
        output << MARK_NEXT;
        m_branch->serializeTree(output);
    }

    if ( nullptr == m_next )
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
    m_prev = nullptr;
    m_next = nullptr;
    m_branch = nullptr;
    m_args = nullptr;

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

    if ( nullptr == this )
    {
        out << "<ERROR:nullptr!>" << std::endl;
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

    if ( nullptr == m_prev )
    {
        if ( nullptr == m_next )
        {
            lineString[level] = '0';
        }
        out << "  █─ ";
    }
    else
    {
        if ( nullptr == m_next )
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

    if ( nullptr != m_args )
    {
        out << " [ ";
        out << m_args;
        out << " ]";
    }
    out << std::endl;

    if ( nullptr != m_branch )
    {
        lineString += "1";
        m_branch->print(out, level + 1, lineString);
    }

    if ( nullptr != m_next )
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
