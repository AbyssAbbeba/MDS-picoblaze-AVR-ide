// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup McuSimCfgMgr
 * @file McuSimCfgMgrMacros.h
 */
// =============================================================================

#ifndef MCUSIMCFGMGRMACROS_H
#define MCUSIMCFGMGRMACROS_H

#define CHECK_NO_OF_ATTRS(number) \
    if ( number != numberOf ) \
    { \
        qDebug() << "Invalid number of attributes, parser line: " << __LINE__ \
                 << ", tag: " << localName; \
        return false; \
    }

#define INVALID_ATTRIBUTE() \
    qDebug() << "Invalid attribute, parser line: " << __LINE__ \
             << ", tag: " << localName \
             << ", attr: " << atts.localName(i); \
    return false;

#define INVALID_ATTR_VALUE() \
    qDebug() << " Invalid attribute value, parser line: " << __LINE__ \
             << ", tag: " << localName \
             << ", attr: " << atts.localName(i) \
             << ", val: " << atts.value(i); \
    return false;

#endif // MCUSIMCFGMGRMACROS_H
