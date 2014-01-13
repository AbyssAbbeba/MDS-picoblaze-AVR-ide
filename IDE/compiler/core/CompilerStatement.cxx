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
 * @file CompilerStatement.cxx
 */
// =============================================================================

#include "CompilerStatement.h"

CompilerStatement::~CompilerStatement()
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

CompilerStatement::CompilerStatement()
{
    m_type = CompilerStatementTypes::EMPTY_STATEMENT;

    m_userData     =  0;
    m_serialNumber = -1;

    m_prev      = nullptr;
    m_next      = nullptr;
    m_branch    = nullptr;
    m_args      = nullptr;
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
        newNode = new CompilerStatement();
        input >> newNode;

        currentNode->m_next = newNode;
        newNode->m_prev = currentNode;
        currentNode = newNode;

        input >> mark;
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

    m_prev = nullptr;
    m_next = nullptr;
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

CompilerStatement * CompilerStatement::createBranch ( CompilerStatement * branch )
{
    if ( nullptr == this )
    {
        return nullptr;
    }
    if ( nullptr == branch )
    {
        return this;
    }

    m_branch = branch->first();
    return this;
}

CompilerStatement * CompilerStatement::first()
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    CompilerStatement * stmt = this;

    while ( nullptr != stmt->prev() )
    {
        stmt = stmt->prev();
    }

    return stmt;
}

CompilerStatement * CompilerStatement::last()
{
    CompilerStatement * result = this;
    while ( nullptr != result->next() )
    {
        result = result->next();
    }
    return result;
}

CompilerStatement * CompilerStatement::lastLeaf()
{
    CompilerStatement * result = this;

    while ( nullptr != result->next() )
    {
        while ( nullptr != result->next() )
        {
            result = result->next();
        }

        while ( nullptr != result->branch() )
        {
            result = result->branch();
        }
    }

    return result;
}

CompilerStatement * CompilerStatement::insertLink ( CompilerStatement * chainLink )
{
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
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
    if ( nullptr != nextOrig )
    {
        nextOrig->m_prev = chainLinkLast;
    }

    return chainLinkOrig;
}

CompilerStatement * CompilerStatement::appendLink ( CompilerStatement * chainLink )
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

    CompilerStatement * stmt = this;
    while ( nullptr != stmt->next() )
    {
        stmt = stmt->next();
    }
    stmt->m_next = chainLink;
    chainLink->m_prev = stmt;

    return chainLink;
}

CompilerStatement * CompilerStatement::prependLink ( CompilerStatement * chainLink )
{
    if ( nullptr == chainLink )
    {
        return this;
    }
    if ( nullptr == this )
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

CompilerStatement * CompilerStatement::copyEntireChain() const
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    CompilerStatement * result = copyChainLink();

    const CompilerStatement * next = this;
    while ( nullptr != ( next = next->next() ) )
    {
        result->appendLink(next->copyChainLink());
    }

    const CompilerStatement * prev = this;
    while ( nullptr != ( prev = prev->prev() ) )
    {
        result->prependLink(prev->copyChainLink());
    }

    return result;
}

CompilerStatement * CompilerStatement::copyChainLink() const
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    CompilerStatement * result = new CompilerStatement(m_location, m_type, m_args->copyEntireChain());
    result->m_branch = m_branch->copyEntireChain();
    result->m_userData = m_userData;
    result->m_serialNumber = m_serialNumber;

    return result;
}

CompilerStatement * CompilerStatement::appendArgsLink ( CompilerExpr * chainLink )
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

void CompilerStatement::completeDelete ( CompilerStatement * stmt )
{
    stmt->completeDelete();
}

void CompilerStatement::completeDelete()
{
    if ( nullptr == this )
    {
        return;
    }

    CompilerStatement * tmp;
    CompilerStatement * node;

    node = m_next;
    while ( nullptr != node )
    {
        tmp = node;
        node = node->next();
        delete tmp;
    }

    node = m_prev;
    while ( nullptr != node )
    {
        tmp = node;
        node = node->prev();
        delete tmp;
    }

    delete this;
}

std::ostream & CompilerStatement::print ( std::ostream & out,
                                          int level,
                                          std::string lineString ) const
{
    if ( nullptr == this )
    {
        out << "<nullptr>";
        return out;
    }

    for ( const CompilerStatement * node = this;
          nullptr != node;
          node = node->next() )
    {
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

        if ( nullptr == node->prev() )
        {
            if ( nullptr == node->next() )
            {
                lineString[level] = '0';
            }
            out << "  █─ ";
        }
        else
        {
            if ( nullptr == node->next() )
            {
                out << "  └─ ";
                lineString[level] = '0';
            }
            else
            {
                out << "  ├─ ";
            }
        }

        out << node->type();

        out << " {";
        out << node->location();
        out << "} <";
        out << node->m_userData;
        out << '/';
        out << node->m_serialNumber;
        out << '>';

        if ( nullptr != node->args() )
        {
            out << " [ ";
            out << node->args();
            out << " ]";
        }
        out << '\n';

        if ( nullptr != node->branch() )
        {
            lineString += '1';
            node->branch()->print(out, level + 1, lineString);
        }
    }

    return out;
}

void CompilerStatement::serializeTree ( CompilerSerializer & output ) const
{
    for ( const CompilerStatement * node = this;
          nullptr != node;
          node = node->next() )
    {
        output << node;
    }
}

void CompilerStatement::serialize ( CompilerSerializer & output ) const
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

void CompilerStatement::deserialize ( CompilerSerializer & input )
{
    m_userData = 0;
    m_serialNumber = -1;
    m_prev = nullptr;
    m_next = nullptr;
    m_branch = nullptr;
    m_args = nullptr;

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
