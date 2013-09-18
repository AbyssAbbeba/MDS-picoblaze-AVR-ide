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

MScriptFuncTable::Parameter::Parameter()
{
    m_value = NULL;
}

MScriptFuncTable::Parameter::Parameter ( const char * name,
                                         const MScriptValue * defaulValue )
                                       :
                                         m_name ( name )
{
    if ( NULL == defaulValue )
    {
        m_value = NULL;
    }
    else
    {
        m_value = new MScriptValue();
        *m_value = defaulValue->makeCopy();
    }
}

MScriptFuncTable::Parameter::~Parameter()
{
    if ( NULL != m_value )
    {
        delete m_value;
    }
}

MScriptFuncTable::Function::Function ( std::vector<Parameter> * params,
                                       MScriptStatement * code,
                                       const MScriptSrcLocation & location,
                                       unsigned int argsRequired )
{
    m_params       = params;
    m_code         = code;
    m_location     = location;
    m_argsRequired = argsRequired;
    m_deleteCode   = true;
}

MScriptFuncTable::Function::Function ( const Function & obj )
{
    m_deleteCode = false;

    m_code         = obj.m_code;
    m_location     = obj.m_location;
    m_argsRequired = obj.m_argsRequired;
    m_params       = new std::vector<Parameter>(obj.m_params->size());

    for ( size_t i = 0; i < m_params->size(); i++ )
    {
        m_params->at(i) = obj.m_params->at(i);
    }
}

MScriptFuncTable::Function::~Function()
{
    if ( ( true == m_deleteCode ) && ( NULL != m_code ) )
    {
        m_code->completeDelete();
    }
    if ( NULL != m_params )
    {
        delete m_params;
    }
}

bool MScriptFuncTable::define ( const std::string & name,
                                const MScriptSrcLocation & location,
                                std::vector<Parameter> * params,
                                MScriptStatement * code )
{
    unsigned int defaults = 0;

    for ( std::vector<Parameter>::const_iterator p = params->cbegin();
          p != params->cend();
          p++ )
    {
        if ( NULL == p->m_value )
        {
            if ( 0 != defaults )
            {
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "parameter `%1' has to have default value "
                                                                "(because one of preceding parameters has).")
                                                               . arg ( p->m_name.c_str() )
                                                               . toStdString() );
                return false;
            }
        }
        else
        {
            defaults++;
        }
    }

    unsigned int argsRequired = params->size() - defaults;
    MScriptSrcLocation prevDefLoc;

    if ( true == isDefined(name, argsRequired, &prevDefLoc) )
    {
        const std::string prevDefLocStr = prevDefLoc.toString();
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr("ambiguous function overload, overlaps with `%1' declared at %2")
                                                     . arg ( name.c_str() )
                                                     . arg ( prevDefLocStr.c_str() )
                                                     . toStdString() );
        return false;
    }
    else if ( true == isDefined(name, -1, &prevDefLoc) )
    {
        const std::string prevDefLocStr = prevDefLoc.toString();
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_REMARK,
                                          QObject::tr ( "overloading function `%1' declared at %2" )
                                                      . arg ( name.c_str() )
                                                      . arg ( prevDefLocStr.c_str() )
                                                      . toStdString() );
        return false;
    }
    else
    {
        m_funcTable.insert(std::make_pair(name, Function(params, code, location, argsRequired)));
        return true;
    }

}

bool MScriptFuncTable::undefine ( const std::string & name,
                                  const MScriptSrcLocation & location )
{
    FuncMultimap::iterator it = m_funcTable.find(name);

    if ( m_funcTable.end() == it )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "function `%1' has not been defined (cannot delete "
                                                        "nonexistent function)" ) . arg ( name.c_str() )
                                                                                  . toStdString() );
        return false;
    }
    else
    {
        while ( m_funcTable.end() == it )
        {
            m_funcTable.erase(it);
            it = m_funcTable.find(name);
        }
        return true;
    }
}

bool MScriptFuncTable::isDefined ( const std::string & name,
                                   int argc,
                                   MScriptSrcLocation * location ) const
{
    FuncMultimap::const_iterator it = m_funcTable.find(name);

    if ( m_funcTable.end() == it )
    {
        return false;
    }

    if ( -1 == argc )
    {
        if ( NULL != location )
        {
            *location = it->second.m_location;
        }
        return true;
    }
    else
    {
        if ( (unsigned int) argc >= it->second.m_argsRequired && (size_t) argc <= it->second.m_params->size() )
        {
            return true;
        }

        std::pair<FuncMultimap::const_iterator,FuncMultimap::const_iterator> range = m_funcTable.equal_range(name);
        for ( FuncMultimap::const_iterator i = range.first;
              i != range.second;
              i++ )
        {
            if ( (unsigned int) argc >= i->second.m_argsRequired && (size_t) argc <= i->second.m_params->size() )
            {
                return true;
            }
        }

        return false;
    }
}

MScriptFuncTable::Function * MScriptFuncTable::get ( const std::string & name,
                                                     const MScriptSrcLocation & location,
                                                     std::vector<MScriptValue> & arguments )
{
    std::pair<FuncMultimap::const_iterator,FuncMultimap::const_iterator> range = m_funcTable.equal_range(name);
    size_t argc = arguments.size();

    for ( FuncMultimap::const_iterator i = range.first;
          i != range.second;
          i++ )
    {
        if ( argc >= i->second.m_argsRequired && argc <= i->second.m_params->size() )
        {
            Function * result = new Function(i->second);

            for ( size_t a = 0; a < arguments.size(); a++ )
            {
                result->m_params->at(a).m_value = &(arguments[a]);
            }

            return result;
        }
    }

    if ( false == isDefined(name) )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "function `%1' has not been defined" )
                                                      . arg ( name.c_str() )
                                                      . toStdString() );
    }
    else
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "function `%1' has not been defined for the given number of "
                                                        "arguments (%2)" ) . arg ( name.c_str() )
                                                                           . arg ( argc )
                                                                           . toStdString() );
    }

    return NULL;
}

std::ostream & operator << ( std::ostream & out,
                             const std::vector<MScriptFuncTable::Parameter> * parameterList )
{
    bool first = true;

    for ( std::vector<MScriptFuncTable::Parameter>::const_iterator p = parameterList->cbegin();
          p != parameterList->cend();
          p++ )
    {
        if ( false == first )
        {
            out << ", ";
        }
        first = false;

        out << p->m_name;
        if ( NULL != p->m_value )
        {
            out << " = " << *(p->m_value);
        }
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptFuncTable & table )
{
    static const char * const sepLine0 = "==================================================";
    static const char * const sepLine1 = "--------------------------------------------------";

    for ( MScriptFuncTable::FuncMultimap::const_iterator f = table.m_funcTable.cbegin();
          f != table.m_funcTable.cend();
          f++ )
    {
        out << sepLine0 << std::endl;
        out << f->first << "(" << f->second.m_params << ")" << std::endl;
        out << "    declared at: " << f->second.m_location.toString() << std::endl;
        out << "    required arguments: " << f->second.m_argsRequired << std::endl;
        out << sepLine1 << std::endl;
        out << f->second.m_code << std::endl;
        out << sepLine0 << std::endl;
    }

    return out;
}
