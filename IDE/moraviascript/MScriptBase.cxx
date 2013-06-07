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
 * @file MScriptBase.cxx
 */
// =============================================================================

#include "MScriptBase.h"

// Used for i18n only.
#include <QObject>

std::string MScriptBase::msgTypeToStr ( MessageType type ) const
{
    switch ( type )
    {
        case MT_ERROR:   return QObject::tr("error").toStdString();
        case MT_WARNING: return QObject::tr("warning").toStdString();
        case MT_GENERAL: return "";
    }
}
