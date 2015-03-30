// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCDeclaration.cxx
 */
// =============================================================================

#include "CompilerCDeclaration.h"

// Standard headers.
#include <vector>
#include <utility>

// Compiler header files.
#include "CompilerExpr.h"
#include "CompilerStatement.h"

CompilerCDeclaration::CompilerCDeclaration ( const CompilerSourceLocation & location )
{
    m_type       = F_EMPTY;
    m_location   = location;

    m_value      = nullptr;
    m_array      = nullptr;
    m_overrule   = nullptr;
    m_parent     = nullptr;
    m_members    = nullptr;
    m_next       = nullptr;
    m_prev       = nullptr;
    m_parameters = nullptr;
    m_instances  = nullptr;
}

CompilerCDeclaration::~CompilerCDeclaration()
{
    for ( auto i : { m_array, m_overrule, m_members, m_parameters, m_instances } )
    {
        while ( nullptr != i )
        {
            auto del = i;
            i = i->m_next;
            delete del;
        }
    }

    if ( nullptr != m_parent )
    {
        m_parent->unregisterMember(this);
    }

    m_value->completeDelete();

    unlink();
}

CompilerCDeclaration::Allocation::Allocation()
{
    m_address = -1;
    m_alignment = -1;
    m_bitFieldWidth = -1;
    m_memory = MS_UNSPECIFIED;
}

CompilerCDeclaration::Name::Name()
{
    m_id = nullptr;
    m_type = nullptr;
    m_typedef = nullptr;
}

CompilerCDeclaration::Name::~Name()
{
    for ( auto i : { m_id, m_type, m_typedef } )
    {
        if ( nullptr != i )
        {
            delete i;
        }
    }
}

CompilerCDeclaration * CompilerCDeclaration::copyChainLink ( const CompilerSourceLocation & location ) const
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    auto result = new CompilerCDeclaration(location);

    result->m_type    = m_type;
    result->m_alloc   = m_alloc;
    result->m_pointer = m_pointer;

    if ( nullptr != m_name.m_id )
    {
        result->m_name.m_id = new std::string ( *m_name.m_id );
    }

    if ( nullptr != m_name.m_type )
    {
        result->m_name.m_type = new std::string ( *m_name.m_type );
    }

    if ( nullptr != m_name.m_typedef )
    {
        result->m_name.m_typedef = new std::string ( *m_name.m_typedef );
    }

    result->m_value      = m_value      -> copyEntireChain();
    result->m_array      = m_array      -> copyEntireChain();
    result->m_overrule   = m_overrule   -> copyEntireChain();
    result->m_members    = m_members    -> copyEntireChain();
    result->m_parameters = m_parameters -> copyEntireChain();
    result->m_instances  = m_instances  -> copyEntireChain();

    return result;
}

CompilerCDeclaration * CompilerCDeclaration::copyEntireChain() const
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    auto result = copyChainLink(m_location);

    auto next = this;
    while ( nullptr != ( next = next->m_next ) )
    {
        result->appendLink(next->copyChainLink(next->m_location));
    }

    auto prev = this;
    while ( nullptr != ( prev = prev->m_prev ) )
    {
        result->prependLink(prev->copyChainLink(prev->m_location));
    }

    return result;
}

void CompilerCDeclaration::addMembers ( CompilerCDeclaration * node )
{
    if ( nullptr == m_members )
    {
        m_members = node;
    }
    else
    {
        m_members->appendLink(node);
    }
}

void CompilerCDeclaration::addInstance ( CompilerCDeclaration * node )
{
    if ( nullptr == m_instances )
    {
        m_instances = node;
    }
    else
    {
        m_instances->appendLink(node);
    }
}

void CompilerCDeclaration::addParameter ( CompilerCDeclaration * node )
{
    if ( nullptr == m_parameters )
    {
        m_parameters = node;
    }
    else
    {
        m_parameters->appendLink(node);
    }
}

void CompilerCDeclaration::unregisterMember ( const CompilerCDeclaration * const node )
{
    for ( auto child = m_members; nullptr != child; child = child->m_next)
    {
        if ( node == child )
        {
            if ( nullptr != child->m_next )
            {
                child->m_next->m_prev = child->m_prev;
            }

            if ( nullptr != child->m_next )
            {
                child->m_prev->m_next = child->m_next;
            }

            if ( child == m_members )
            {
                m_members = child->m_next;
            }

            return;
        }
    }
}

void CompilerCDeclaration::appendLink ( CompilerCDeclaration * node )
{
    if ( ( nullptr == node ) || ( nullptr == this ) )
    {
        return;
    }

    auto lastLink = last();
    node = node->first();
    node->m_prev = lastLink;
    lastLink->m_next = node;
}

void CompilerCDeclaration::prependLink ( CompilerCDeclaration * node )
{
    if ( ( nullptr == node ) || ( nullptr == this ) )
    {
        return;
    }

    auto firstLink = first();
    node = node->last();
    node->m_next = firstLink;
    firstLink->m_prev = node;
}

void CompilerCDeclaration::unlink()
{
    if ( nullptr != m_next )
    {
        m_next->m_prev = m_prev;
    }

    if ( nullptr != m_prev )
    {
        m_prev->m_next = m_next;
    }
}

CompilerCDeclaration * CompilerCDeclaration::first()
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    auto result = this;
    while ( nullptr != result->m_prev )
    {
        result = result->m_prev;
    }
    return result;
}

CompilerCDeclaration * CompilerCDeclaration::last()
{
    if ( nullptr == this )
    {
        return nullptr;
    }

    auto result = this;
    while ( nullptr != result->m_next )
    {
        result = result->m_next;
    }
    return result;
}

#ifndef NDEBUG
void CompilerCDeclaration::print ( std::ostream & out,
                                   std::string lineString ) const
{
    if ( nullptr == this )
    {
        out << lineString << "<null-declaration>" << std::endl;
        return;
    }

    std::string lineStringOrig = lineString;

    {
        const char * c = ( ( nullptr == m_prev ) ? "█" : ( ( nullptr == m_next ) ? "└" : "├" ) );
        out << lineString << c << "─ declaration " << std::endl;
    }

    lineString += ( ( nullptr == m_next ) ? " " : "│" );

    if ( nullptr != m_parent )
    {
        out << lineString << "  ├─ parent: true" << std::endl;
    }

    if ( F_EMPTY != m_type )
    {
        out << lineString << "  ├─ type: " << m_type << std::endl;
    }

    if ( nullptr != m_value )
    {
        out << lineString << "  ├─ value: " << m_value << std::endl;
    }

    if ( nullptr != m_name.m_id )
    {
        out << lineString << "  ├─ name.id: \"" << *m_name.m_id << '"' << std::endl;
    }

    if ( nullptr != m_name.m_type )
    {
        out << lineString << "  ├─ name.type: \"" << *m_name.m_type << '"' << std::endl;
    }

    if ( nullptr != m_name.m_typedef )
    {
        out << lineString << "  ├─ name.typedef: \"" << *m_name.m_typedef << '"' << std::endl;
    }

    if ( MS_UNSPECIFIED != m_alloc.m_memory )
    {
        out << lineString << "  ├─ allocation.memory: ";
        switch ( m_alloc.m_memory )
        {
            case MS_CODE: out << "CODE";
            case MS_DATA: out << "DATA";
            case MS_REG:  out << "REG";
            case MS_PORT: out << "PORT";
            default:
                 out << "<error-unknown!>";
        }
        out << std::endl;
    }
    if ( -1 != m_alloc.m_address )
    {
        out << lineString << "  ├─ allocation.address: " << m_alloc.m_address << std::endl;
    }
    if ( -1 != m_alloc.m_alignment )
    {
        out << lineString << "  ├─ allocation.alignment: " << m_alloc.m_alignment << std::endl;
    }
    if ( -1 != m_alloc.m_bitFieldWidth )
    {
        out << lineString << "  ├─ allocation.bitFieldWidth: " << m_alloc.m_bitFieldWidth << std::endl;
    }

    if ( false == m_pointer.empty() )
    {
        out << lineString << "  ├─ pointer" << std::endl;
        for ( unsigned int i = 0; i < m_pointer.size(); i++ )
        {
            const char * c = ( ( i + 1 ) == m_pointer.size() ) ? "└" : "├";
            out << lineString << "  │  " << c << "─ type: " << m_pointer[i] <<'\n';
        }
    }

    const std::pair<std::string, CompilerCDeclaration*> BRANCHES[] =
    {
        { "array",      m_array         },
        { "overrule",   m_overrule      },
        { "instances",  m_instances     },
        { "members",    m_members       },
        { "parameters", m_parameters    },
    };

    for ( const auto & i : BRANCHES )
    {
        if ( nullptr != i.second )
        {
            out << lineString << "  ├─ " << i.first << std::endl;
            i.second->print(out, lineString + "  │  ");
        }
    }

    out << lineString << "  └─ location: " << m_location << std::endl;

    if ( nullptr != m_next )
    {
        m_next->print(out, lineStringOrig);
    }
}
#endif // NDEBUG

#ifndef NDEBUG
std::ostream & operator << ( std::ostream & out,
                             const CompilerCDeclaration * decl )
{
    decl->print(out);
    return out;
}
#endif // NDEBUG

#ifndef NDEBUG
std::ostream & operator << ( std::ostream & out,
                             const CompilerCDeclaration::Type type )
{
    static const std::vector<std::pair<CompilerCDeclaration::Type,std::string>> TYPES =
    {
        { CompilerCDeclaration::TS_VOID,         "void" },
        { CompilerCDeclaration::TS_CHAR,         "char" },
        { CompilerCDeclaration::TS_SHORT,        "short" },
        { CompilerCDeclaration::TS_INT,          "int" },
        { CompilerCDeclaration::TS_LONG,         "long" },
        { CompilerCDeclaration::TS_LONG_LONG,    "long long" },
        { CompilerCDeclaration::TS_FLOAT,        "float" },
        { CompilerCDeclaration::TS_DOUBLE,       "double" },
        { CompilerCDeclaration::TS_LONG_DOUBLE,  "long double" },
        { CompilerCDeclaration::TS_BOOL,         "bool" },
        { CompilerCDeclaration::TS_COMPLEX,      "complex" },
        { CompilerCDeclaration::TS_IMAGINARY,    "imaginary" },
        { CompilerCDeclaration::TS_UNSIGNED,     "unsigned" },
        { CompilerCDeclaration::TS_SIGNED,       "signed" },
        { CompilerCDeclaration::TS_ENUM,         "enum" },
        { CompilerCDeclaration::TS_UNION,        "union" },
        { CompilerCDeclaration::TS_STRUCT,       "struct" },
        { CompilerCDeclaration::TQ_CONST,        "const" },
        { CompilerCDeclaration::TQ_RESTRICT,     "restrict" },
        { CompilerCDeclaration::TQ_VOLATILE,     "volatile" },
        { CompilerCDeclaration::TQ_ATOMIC,       "atomic" },
        { CompilerCDeclaration::SC_STATIC,       "static" },
        { CompilerCDeclaration::SC_EXTERN,       "extern" },
        { CompilerCDeclaration::SC_AUTO,         "auto" },
        { CompilerCDeclaration::SC_REGISTER,     "register" },
        { CompilerCDeclaration::SC_TYPEDEF,      "typedef" },
        { CompilerCDeclaration::SC_THREAD_LOCAL, "thread_local" },
        { CompilerCDeclaration::FS_INLINE,       "inline" },
        { CompilerCDeclaration::FS_NORETURN,     "noreturn" },
        { CompilerCDeclaration::A_FUNCTION,      "FUNCTION" },
    };

    if ( CompilerCDeclaration::F_EMPTY == type )
    {
        out << "(empty)";
        return out;
    }

    bool next = false;
    for ( const auto & i : TYPES )
    {
        if ( i.first & type )
        {
            if ( true == next )
            {
                out << ", ";
            }
            out << i.second;
            next = true;
        }
    }

    return out;
}
#endif // NDEBUG
