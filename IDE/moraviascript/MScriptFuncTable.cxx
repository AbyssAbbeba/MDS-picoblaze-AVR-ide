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
 * @file MScriptFuncTable.cxx
 */
// =============================================================================

#include "MScriptFuncTable.h"

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptStatement.h"
#include "MScriptInterpretInterface.h"

// Used for i18n only
#include <QObject>

bool MScriptFuncTable::define ( const std::string & name,
                                const std::vector<Parameter> * params )
{
}

bool MScriptFuncTable::undefine ( const std::string & name )
{
}

bool MScriptFuncTable::isDefined ( const std::string & name ) const
{
}

const MScriptFuncTable::Function & MScriptFuncTable::get ( const std::string & name,
                                                           const std::vector<MScriptValue> & arguments )
{
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptFuncTable & table )
{
}