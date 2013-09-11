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


MScriptFuncTable::Parameter::Parameter ( const char * name,
                                         const MScriptValue * defaulValue )
                                       :
                                         m_name ( name )
{
    if ( NULL == defaulValue )
    {
        m_defaulValue = NULL;
    }
    else
    {
        m_defaulValue = new MScriptValue();
        *m_defaulValue = defaulValue->makeCopy();
    }
}

MScriptFuncTable::Parameter::~Parameter()
{
    delete m_defaulValue;
}

MScriptFuncTable::Function::Function()
{
    m_code   = NULL;
    m_params = NULL;
}

MScriptFuncTable::Function::~Function()
{
    if ( NULL != m_code )
    {
        m_code->completeDelete();
    }
    if ( NULL != m_params )
    {
        delete m_params;
    }
}

bool MScriptFuncTable::define ( const std::string & name,
                                const std::vector<Parameter> * params,
                                const MScriptStatement * code )
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