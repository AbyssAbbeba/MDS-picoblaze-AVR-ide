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
#include "MScriptNamespaces.h"

int MScriptStrategy::newFunction ( const std::string & ns,
                                   const std::string & name,
                                   const std::vector<std::string> & params,
                                   const std::vector<MScriptValue> * defaults )
{
    std::vector<MScriptFuncTable::Parameter> * parameters = new std::vector<MScriptFuncTable::Parameter>;

    for ( size_t i = 0; i < params.size(); i ++ )
    {
        if ( ( NULL != defaults ) && ( i < defaults->size() ) )
        {
            parameters->push_back(MScriptFuncTable::Parameter(params[i].c_str(), defaults->at(i)));
        }
        else
        {
            parameters->push_back(params[i].c_str());
        }
    }

    m_core->getFuncTbl()->define ( m_core->getNs()->str2ns(true), name, parameters, m_funcId.acquire() );
}

bool MScriptStrategy::deleteFunction ( int id )
{
    m_funcId.release(id);
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
    m_core->strategyMessage(type, text);
}
