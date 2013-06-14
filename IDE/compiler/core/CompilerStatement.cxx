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
 * @ingroup Compiler
 * @file CompilerStatement.cxx
 */
// =============================================================================

#include "CompilerStatement.h"

CompilerStatement::~CompilerStatement()
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

CompilerStatement::CompilerStatement()
{
    m_type = CompilerStatementTypes::EMPTY_STATEMENT;
    m_userData = 0;
    m_serialNumber = -1;
    m_prev = NULL;
    m_next = NULL;
    m_branch = NULL;
    m_args = NULL;
}

CompilerStatement::CompilerStatement ( CompilerSerializer & input )
{
    deserialize(input);

    CompilerStatement * currentNode = this;
    CompilerStatement * newNode;
    SerializationMark mark;

    input >> mark;
    while ( MARK_NEXT == mark )
    {
        input >> mark;

        newNode = new CompilerStatement();
        input >> newNode;

        currentNode->m_next = newNode;
        newNode->m_prev = currentNode;
        currentNode = newNode;
    }
}

CompilerStatement::CompilerStatement ( CompilerSourceLocation location,
                                       CompilerStatementTypes::StatementType type,
                                       CompilerExpr * args )
{
    m_type = type;
    m_location = location;

    m_userData = 0;
    m_serialNumber = -1;

    m_prev = NULL;
    m_next = NULL;
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

CompilerStatement * CompilerStatement::createBranch ( CompilerStatement * branch )
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

CompilerStatement * CompilerStatement::first()
{
    if ( NULL == this )
    {
        return NULL;
    }

    CompilerStatement * stmt = this;

    while ( NULL != stmt->prev() )
    {
        stmt = stmt->prev();
    }

    return stmt;
}

CompilerStatement * CompilerStatement::last()
{
    CompilerStatement * result = this;
    while ( NULL != result->next() )
    {
        result = result->next();
    }
    return result;
}

CompilerStatement * CompilerStatement::lastLeaf()
{
    CompilerStatement * result = this;

    while ( NULL != result->next() )
    {
        while ( NULL != result->next() )
        {
            result = result->next();
        }

        while ( NULL != result->branch() )
        {
            result = result->branch();
        }
    }

    return result;
}

CompilerStatement * CompilerStatement::insertLink ( CompilerStatement * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    CompilerStatement * chainLinkOrig = chainLink;
    CompilerStatement * chainLinkLast = chainLink->last();
    CompilerStatement * nextOrig = m_next;

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

CompilerStatement * CompilerStatement::appendLink ( CompilerStatement * chainLink )
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

    CompilerStatement * stmt = this;
    while ( NULL != stmt->next() )
    {
        stmt = stmt->next();
    }
    stmt->m_next = chainLink;
    chainLink->m_prev = stmt;

    return chainLink;
}

CompilerStatement * CompilerStatement::prependLink ( CompilerStatement * chainLink )
{
    if ( NULL == chainLink )
    {
        return this;
    }
    if ( NULL == this )
    {
        return chainLink;
    }

    CompilerStatement * chainLinkOrig = chainLink;
    CompilerStatement * firstLink = first();

    chainLink = chainLink->last();
    chainLink->m_next = firstLink;
    firstLink->m_prev = chainLink;

    return chainLinkOrig;
}

CompilerStatement * CompilerStatement::unlink()
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

CompilerStatement * CompilerStatement::copyEntireChain() const
{
    if ( NULL == this )
    {
        return NULL;
    }

    CompilerStatement * result = copyChainLink();

    const CompilerStatement * next = this;
    while ( NULL != ( next = next->next() ) )
    {
        result->appendLink(next->copyChainLink());
    }

    const CompilerStatement * prev = this;
    while ( NULL != ( prev = prev->prev() ) )
    {
        result->prependLink(prev->copyChainLink());
    }

    return result;
}

CompilerStatement * CompilerStatement::copyChainLink() const
{
    if ( NULL == this )
    {
        return NULL;
    }

    CompilerStatement * result = new CompilerStatement(m_location, m_type, m_args->copyEntireChain());
    result->m_branch = m_branch->copyEntireChain();
    result->m_userData = m_userData;
    result->m_serialNumber = m_serialNumber;

    return result;
}

CompilerStatement * CompilerStatement::appendArgsLink ( CompilerExpr * chainLink )
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

void CompilerStatement::completeDelete ( CompilerStatement * stmt )
{
    stmt->completeDelete();
}

void CompilerStatement::completeDelete()
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

std::ostream & CompilerStatement::print ( std::ostream & out,
                                          int level,
                                          std::string lineString ) const
{
    if ( NULL == this )
    {
        out << "<ERROR:NULL!>";
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
    out << "} <";
    out << m_userData;
    out << "/";
    out << m_serialNumber;
    out << ">";

    if ( NULL != m_args )
    {
        out << " [ ";
        out << m_args;
        out << " ]";
    }
    out << "\n";

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

void CompilerStatement::serializeTree ( CompilerSerializer & output ) const
{
    for ( const CompilerStatement * node = this;
          NULL != node;
          node = node->next() )
    {
        output << node;
    }
}

void CompilerStatement::serialize ( CompilerSerializer & output ) const
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

void CompilerStatement::deserialize ( CompilerSerializer & input )
{
    m_userData = 0;
    m_serialNumber = -1;
    m_prev = NULL;
    m_next = NULL;
    m_branch = NULL;
    m_args = NULL;

    m_type = CompilerStatementTypes::StatementType ( input.read_ui32() );
    input >> m_location;

    SerializationMark mark;

    input >> mark;
    if ( MARK_NEXT == mark )
    {
        m_args = new CompilerExpr(input);
    }

    input >> mark;
    if ( MARK_NEXT == mark )
    {
        m_branch = new CompilerStatement(input);
    }
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerStatement * stmt )
{
    return stmt->print(out);
}
