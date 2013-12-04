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
 * @file MScriptSerializable.cxx
 */
// =============================================================================

#include "MScriptSerializable.h"

MScriptSerializer & operator << ( MScriptSerializer & out,
                                  const MScriptSerializable & val )
{
    val.serialize(out);
    return out;
}

MScriptSerializer & operator << ( MScriptSerializer & out,
                                  const MScriptSerializable * val )
{
    val->serialize(out);
    return out;
}

MScriptSerializer & operator >> ( MScriptSerializer & in,
                                  MScriptSerializable & val )
{
    val.deserialize(in);
    return in;
}

MScriptSerializer & operator >> ( MScriptSerializer & in,
                                  MScriptSerializable * val )
{
    val->deserialize(in);
    return in;
}

MScriptSerializer & operator << ( MScriptSerializer & out,
                                  MScriptSerializable::SerializationMark mark )
{
    out.write ( (uint8_t) mark );
    return out;
}

MScriptSerializer & operator >> ( MScriptSerializer & in,
                                   MScriptSerializable::SerializationMark & mark )
{
    mark = MScriptSerializable::SerializationMark ( in.read_ui8() );
    return in;
}
