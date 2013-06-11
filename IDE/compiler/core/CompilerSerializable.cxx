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
 * @file CompilerSerializable.cxx
 */
// =============================================================================

#include "CompilerSerializable.h"

CompilerSerializer & operator << ( CompilerSerializer & out,
                                   const CompilerSerializable & val )
{
    val.serialize(out);
    return out;
}

CompilerSerializer & operator << ( CompilerSerializer & out,
                                   const CompilerSerializable * val )
{
    val->serialize(out);
    return out;
}

CompilerSerializer & operator >> ( CompilerSerializer & in,
                                   CompilerSerializable & val )
{
    val.deserialize(in);
    return in;
}


CompilerSerializer & operator >> ( CompilerSerializer & in,
                                   CompilerSerializable * val )
{
    val->deserialize(in);
    return in;
}

CompilerSerializer & operator << ( CompilerSerializer & out,
                                   CompilerSerializable::SerializationMark mark )
{
    out.write ( (uint8_t) mark );
    return out;
}

CompilerSerializer & operator >> ( CompilerSerializer & in,
                                   CompilerSerializable::SerializationMark & mark )
{
    mark = CompilerSerializable::SerializationMark ( in.read_ui8() );
    return in;
}
