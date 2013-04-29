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
    m_type = StatementTypes::EMPTY_STATEMENT;
    m_userData = 0;
    m_serialNumber = -1;
    m_prev = NULL;
    m_next = NULL;
    m_branch = NULL;
    m_args = NULL;
}

CompilerStatement::CompilerStatement ( CompilerBase::SourceLocation location,
                                       StatementTypes::StatementType type,
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

    while ( NULL != stmt->m_prev )
    {
        stmt = stmt->m_prev;
    }

    return stmt;
}

CompilerStatement * CompilerStatement::last()
{
    CompilerStatement * result = this;
    while ( NULL != result->m_next )
    {
        result = result->m_next;
    }
    return result;
}

CompilerStatement * CompilerStatement::appendLink ( CompilerStatement * next )
{
    if ( NULL == next )
    {
        return this;
    }
    if ( NULL == this )
    {
        return next;
    }

    next = next->first();

    CompilerStatement * stmt = this;
    while ( NULL != stmt->m_next )
    {
        stmt = stmt->m_next;
    }
    stmt->m_next = next;
    next->m_prev = stmt;

    return next;
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
    m_next->m_prev = NULL;
    m_next = NULL;
    return this;
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
    }

    if ( NULL != m_prev )
    {
        m_prev->m_next = NULL;
        m_prev->completeDelete();
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

std::ostream & operator << ( std::ostream & out,
                             const CompilerStatement * const stmt )
{
    return stmt->print(out);
}
