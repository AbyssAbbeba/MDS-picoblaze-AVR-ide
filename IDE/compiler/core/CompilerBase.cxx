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
 * @file CompilerBase.cxx
 */
// =============================================================================

#include "CompilerBase.h"

// Initialize static constants.
const char * const CompilerBase::PRECOMPILED_CODE_EXTENSION = ".prc";

std::ostream & operator << ( std::ostream & out,
                             CompilerBase::MessageType type )
{
    switch ( type )
    {
        case CompilerBase::MT_INVALID:  out << "MT_INVALID";    break;
        case CompilerBase::MT_GENERAL:  out << "MT_GENERAL";    break;
        case CompilerBase::MT_ERROR:    out << "MT_ERROR";      break;
        case CompilerBase::MT_WARNING:  out << "MT_WARNING";    break;
        case CompilerBase::MT_REMARK:   out << "MT_REMARK";     break;
    }
    return out;
}
