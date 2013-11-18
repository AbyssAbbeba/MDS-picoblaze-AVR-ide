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
 * @file MScriptStrategy.cxx
 */
// =============================================================================

#include "MScriptStrategy.h"

// MScript language interpreter header files.
#include "MScriptVarTable.h"
#include "MScriptFuncTable.h"

int MScriptStrategy::newFunction ( const std::string & ns,
                                   const std::string & name,
                                   const std::vector<std::string> & params,
                                   const std::vector<MScriptValue> * defaults )
{
    std::vector<MScriptFuncTable::Parameter> parameters
    
    m_core->getFuncTbl()->define ( ns + "::" + name, &parameters, 
                      int id );
}

bool MScriptStrategy::deleteFunction ( int id )
{
}

int MScriptStrategy::newVariable ( const std::string & ns,
                                   const std::string & name )
{
}

bool MScriptStrategy::deleteVariable ( int id )
{
}

void MScriptStrategy::message ( MScriptBase::MessageType type,
                                const std::string & text )
{
    
}
