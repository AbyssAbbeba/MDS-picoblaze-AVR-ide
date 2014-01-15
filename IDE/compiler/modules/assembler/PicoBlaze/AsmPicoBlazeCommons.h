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
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeCommons.h
 */
// =============================================================================

#ifndef ASMPICOBLAZECOMMONS_H
#define ASMPICOBLAZECOMMONS_H

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @namespace AsmPicoBlazeCommons
 */
namespace AsmPicoBlazeCommons
{
    ////    Datatypes    ////
    enum UserData
    {
        UD_SPEC_MACRO    = 0x10000,
        UD_MACRO_COUNTER = 0x0ffff
    };
};

#endif // ASMPICOBLAZECOMMONS_H
