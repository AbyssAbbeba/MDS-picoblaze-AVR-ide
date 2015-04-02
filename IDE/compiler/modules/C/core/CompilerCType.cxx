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
    out << "<type>";

    out << "</type>";
}

void CompilerCType::Basic::print ( std::ostream & out ) const
{
}

void CompilerCType::BitField::print ( std::ostream & out ) const
{
}

void CompilerCType::Function::print ( std::ostream & out ) const
{
}

void CompilerCType::Array::print ( std::ostream & out ) const
{
}

void CompilerCType::Pointer::print ( std::ostream & out ) const
{
}

void CompilerCType::Struct::print ( std::ostream & out ) const
{
}

void CompilerCType::Enum::print ( std::ostream & out ) const
{
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
        out << "<null/>";
    }

    return out;
}
