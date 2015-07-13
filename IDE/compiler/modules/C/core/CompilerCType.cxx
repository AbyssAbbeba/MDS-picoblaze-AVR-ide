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
 * @file CompilerCType.cxx
 */
// =============================================================================

#include "CompilerCType.h"

void CompilerCType::Type::print ( std::ostream & out ) const
{
    out << "<location>" << m_location << "</location>";
    out << "<size>" << m_size << "</size>";
    out << "<alignment>" << m_alignment << "</alignment>";

    if ( true == m_atomic )
    {
        out << "<atomic/>";
    }
    if ( true == m_constant )
    {
        out << "<constant/>";
    }
    if ( true == m_volatile )
    {
        out << "<volatile/>";
    }
    if ( true == m_static )
    {
        out << "<static/>";
    }
    if ( true == m_extern )
    {
        out << "<extern/>";
    }
    if ( true == m_register )
    {
        out << "<register/>";
    }

    out << "<allocation>";
    out << "<memory>" << m_memory << "</memory>";
    out << "<address>" << m_address << "</address>";
    out << "</allocation>";
}

void CompilerCType::Basic::print ( std::ostream & out ) const
{
    const char * typeTag = nullptr;
    switch ( m_type )
    {
        case VOID:              typeTag = "void";       break;
        case BOOLEAN:           typeTag = "bool";       break;
        case SIGNED_INT:        typeTag = "signed";     break;
        case UNSIGNED_INT:      typeTag = "unsigned";   break;
        case REAL_FLOAT:        typeTag = "real";       break;
        case IMAGINARY_FLOAT:   typeTag = "imginary";   break;
        case COMPLEX_FLOAT:     typeTag = "complex";    break;
        default:                typeTag = "error!";     break;
    }

    out << '<' << typeTag << '>';
    CompilerCType::Type::print(out);
    out << "</" << typeTag << '>';
}

void CompilerCType::BitField::print ( std::ostream & out ) const
{
    out << "<bitfield>";
    out << "<width>" << m_width << "</width>";
    CompilerCType::Type::print(out);
    out << "</bitfield>";
}

void CompilerCType::Function::print ( std::ostream & out ) const
{
    out << "<function>";

    if ( true == m_inline )
    {
        out << "<inline/>";
    }

    if ( true == m_noreturn )
    {
        out << "<noreturn/>";
    }

    out << "<return>" << m_return << "</return>";
    out << "<parameters>";
    for ( const auto p : m_parameters )
    {
        out << "<param>" << p << "</param>";
    }
    out << "</parameters>";

    out << "</function>";
}

void CompilerCType::Array::print ( std::ostream & out ) const
{
    out << "<array>";
    if ( true == m_vla )
    {
        out << "<vla/>";
    }
    out << "<size>" << m_size << "</size>";
    out << "<target>" << m_target << "</target>";
    out << "</array>";
}

void CompilerCType::Pointer::print ( std::ostream & out ) const
{
    out << "<pointer>";
    if ( true == m_restrict )
    {
        out << "<restrict/>";
    }
    out << "<target>" << m_target << "</target>";
    out << "</pointer>";
}

void CompilerCType::Struct::print ( std::ostream & out ) const
{
    out << ( m_union ? "<union>" : "<struct>" );
    if ( false == m_tag.empty() )
    {
        out << "<tag>" << m_tag << "</tag>";
    }
    for ( const auto & m : m_members )
    {
        out << "<member>";
        out << "<type>" << m.second << "</type>";
        out << "<id>" << m.first << "</id>";
        out << "</member>";
    }
    out << ( m_union ? "</union>" : "</struct>" );
}

void CompilerCType::Enum::print ( std::ostream & out ) const
{
    out << "<enum>";
    if ( false == m_tag.empty() )
    {
        out << "<tag>" << m_tag << "</tag>";
    }
    out << "<members>";
    for ( const auto & m : m_members )
    {
        out << "<member>";
        out << "<tag>" << m.first << "</tag>";
        out << "<value>" << m.second << "</value>";
        out << "</member>";
    }
    out << "</members>";
    CompilerCType::Basic::print(out);
    out << "</enum>";
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerCType::Type * type )
{
    if ( nullptr != type )
    {
        type->print(out);
    }
    else
    {
        out << "<empty/>";
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerCType::MemorySpace ms )
{
    using namespace CompilerCType;

    switch ( ms )
    {
        case MS_CODE:  out << "code";
        case MS_DATA:  out << "data";
        case MS_REG:   out << "reg";
        case MS_PORT:  out << "port";
        case MS_STACK: out << "stack";
        default:       out << "error!";
    }

    return out;
}