// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Assembler
 * @file AsmCommons.h
 */
// =============================================================================

#ifndef ASMCOMMONS_H
#define ASMCOMMONS_H

/**
 * @brief
 * @ingroup Assembler
 * @namespace AsmCommons
 */
namespace AsmCommons
{
    ////    Datatypes    ////
    enum UserData
    {
        UD_SPEC_MACRO    = 0x10000,
        UD_MACRO_COUNTER = 0x0ffff
    };
};

#endif // ASMCOMMONS_H
