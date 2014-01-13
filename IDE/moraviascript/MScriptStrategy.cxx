// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
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
        if ( ( nullptr != defaults ) && ( i < defaults->size() ) )
        {
            parameters->push_back(MScriptFuncTable::Parameter(params[i].c_str(), defaults->at(i)));
        }
        else
        {
            parameters->push_back(params[i].c_str());
        }
    }

    int id = m_funcId.acquire();
    if ( true == m_core->getFuncTbl()->define ( m_core->getNs()->str2ns(ns, true), name, parameters, id ) )
    {
        return id;
    }
    else
    {
        m_funcId.release(id);
        return -1;
    }
}

bool MScriptStrategy::deleteFunction ( int id )
{
    if ( false == m_core->getFuncTbl()->undefine(id) )
    {
        return false;
    }

    m_funcId.release(id);
    return true;
}

int MScriptStrategy::newVariable ( const std::string & ns,
                                   const std::string & name )
{
    int id = m_varId.acquire();
    if ( true == m_core->getVarTbl()->declare ( m_core->getNs()->str2ns(ns, true), name, id ) )
    {
        return id;
    }
    else
    {
        m_varId.release(id);
        return -1;
    }
}

bool MScriptStrategy::deleteVariable ( int id )
{
    if ( false == m_core->getVarTbl()->remove(id) )
    {
        return false;
    }

    m_varId.release(id);
    return true;
}

void MScriptStrategy::message ( MScriptBase::MessageType type,
                                const std::string & text )
{
    m_core->strategyMessage(type, text);
}
