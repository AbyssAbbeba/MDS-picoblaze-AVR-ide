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
#include "MScriptNamespaces.h"
#include "MScriptInterpretInterface.h"

// Used for i18n only.
#include <QObject>

MScriptFuncTable::Parameter::Parameter()
{
    m_value = NULL;
}

MScriptFuncTable::Parameter::Parameter ( const char * name )
                                       :
                                         m_name ( name ),
                                         m_value ( NULL ),
                                         m_reference ( false ) {}

MScriptFuncTable::Parameter::Parameter ( const char * name,
                                         const MScriptValue & defaulValue )
                                       :
                                         m_name ( name ),
                                         m_value ( new MScriptValue(defaulValue) ),
                                         m_reference ( false ) {}

MScriptFuncTable::Parameter::Parameter ( const char * name,
                                         bool reference )
                                       :
                                         m_name ( name ),
                                         m_value ( NULL ),
                                         m_reference ( reference ) {}

MScriptFuncTable::Function::Function ( std::vector<Parameter> * params,
                                       MScriptStatement * code,
                                       const MScriptSrcLocation & location,
                                       unsigned int argsRequired,
                                       MScriptNamespaces::NsDesc * ns )
{
    m_params       = params;
    m_code         = code;
    m_location     = location;
    m_ns           = ns;
    m_argsRequired = argsRequired;
    m_id           = -1;
}

MScriptFuncTable::Function::Function ( const Function & obj )
{
    m_code         = obj.m_code;
    m_location     = obj.m_location;
    m_argsRequired = obj.m_argsRequired;
    m_id           = obj.m_id;
    m_params       = new std::vector<Parameter>(obj.m_params->size());

    for ( size_t i = 0; i < m_params->size(); i++ )
    {
        m_params->at(i) = obj.m_params->at(i);
    }
}

MScriptFuncTable::Function::~Function()
{
    if ( NULL != m_params )
    {
        delete m_params;
    }
}

MScriptFuncTable::~MScriptFuncTable()
{
    clear();
}

void MScriptFuncTable::define ( const std::string & name,
                                const MScriptSrcLocation & location,
                                std::vector<Parameter> * params,
                                MScriptStatement * code )
{
    unsigned int defaults = 0;
    std::string bareId;
    MScriptNamespaces::NsDesc * ns = m_namespaces->analyseId ( name, bareId, &location );

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
                                                                "(because some of the preceding parameters have).")
                                                               . arg ( p->m_name.c_str() )
                                                               . toStdString() );
                return;
            }
        }
        else
        {
            defaults++;
        }
    }

    unsigned int argsRequired = params->size() - defaults;
    MScriptSrcLocation prevDefLoc;

    if ( true == isDefined(name, location, argsRequired, &prevDefLoc, true) )
    {
        if ( location == prevDefLoc )
        {
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "exactly this function has already been defined" )
                                                          . toStdString() );
        }
        else
        {
            const std::string prevDefLocStr = prevDefLoc.toString(m_interpret->getCoreBase());
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "ambiguous function overload, overlaps with `%1' declared "
                                                            "at %3" )
                                                          . arg ( bareId.c_str() )
                                                          . arg ( prevDefLocStr.c_str() )
                                                          . toStdString() );
        }
        return;
    }

    if ( true == isDefined(name, location) )
    {
        if ( true == isDefined(name, location, -1, &prevDefLoc, true) )
        {
            const std::string prevDefLocStr = prevDefLoc.toString(m_interpret->getCoreBase());
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_REMARK,
                                              QObject::tr ( "overloading function `%1' declared at %3" )
                                                          . arg ( bareId.c_str() )
                                                          . arg ( prevDefLocStr.c_str() )
                                                          . toStdString() );
        }
        else if ( true == isDefined(name, location, argsRequired, &prevDefLoc) )
        {
            const std::string prevDefLocStr = prevDefLoc.toString(m_interpret->getCoreBase());
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_WARNING,
                                              QObject::tr ( "redefining function `%1' for namespace `%2', function "
                                                            " declared at %3 will be eclipsed in this namespace" )
                                                          . arg ( bareId.c_str() )
                                                          . arg ( ns->toString().c_str() )
                                                          . arg ( prevDefLocStr.c_str() )
                                                          . toStdString() );
        }
        else if ( true == isDefined(name, location, -1, &prevDefLoc) )
        {
            const std::string prevDefLocStr = prevDefLoc.toString(m_interpret->getCoreBase());
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_REMARK,
                                              QObject::tr ( "this function definition overloads another function "
                                                            "declared in antother namespace, orignal function "
                                                            "declaration at `%1'" )
                                                          . arg ( prevDefLocStr.c_str() )
                                                          . toStdString() );
        }
    }

    m_funcTable.insert ( std::make_pair ( name, Function ( params, code, location, argsRequired, ns ) ) );
}

void MScriptFuncTable::define ( MScriptNamespaces::NsDesc * ns,
                                const std::string & name,
                                std::vector<Parameter> * params,
                                int id )
{
    
}

bool MScriptFuncTable::undefine ( const std::string & name,
                                  int id )
{
    
}

bool MScriptFuncTable::undefine ( const std::string & name,
                                  const MScriptSrcLocation & location )
{
    std::string bareId;
    const MScriptNamespaces::NsDesc * ns = m_namespaces->analyseId ( name, bareId, &location );
    std::pair<FuncMultimap::iterator,FuncMultimap::iterator> range = m_funcTable.equal_range(bareId);
    bool result = false;

    for ( FuncMultimap::iterator i = range.first;
          i != range.second;
          i++ )
    {
        if ( ns == i->second.m_ns )
        {
            result = true;
            m_funcTable.erase(i);
        }
    }

    if ( false == result  )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "function `%1' in namespace `%2' has not been defined "
                                                        "(cannot delete nonexistent function)" )
                                                      . arg ( name.c_str() )
                                                      . arg ( ns->toString().c_str() )
                                                      . toStdString() );
    }
    return result;
}

bool MScriptFuncTable::isDefined ( const std::string & name,
                                   const MScriptSrcLocation & location,
                                   int argc,
                                   MScriptSrcLocation * defLocation,
                                   bool exactNsMatch ) const
{
    std::string bareId;
    const MScriptNamespaces::NsDesc * ns = m_namespaces->analyseId ( name, bareId, &location );

    if ( m_funcTable.cend() == m_funcTable.find(bareId) )
    {
        return false;
    }

    std::pair<FuncMultimap::const_iterator,FuncMultimap::const_iterator> range = m_funcTable.equal_range(bareId);
    for ( FuncMultimap::const_iterator i = range.first;
          i != range.second;
          i++ )
    {
        if ( true == exactNsMatch )
        {
            if ( ns != i->second.m_ns )
            {
                continue;
            }
        }
        else if ( -1 == ns->inheritsFrom ( i->second.m_ns ) )
        {
            continue;
        }

        if (
               ( -1 == argc )
                   ||
               (
                   ( (unsigned int) argc >= i->second.m_argsRequired )
                       &&
                   ( (size_t) argc <= i->second.m_params->size() )
               )
           )
        {
            if ( NULL != defLocation )
            {
                *defLocation = i->second.m_location;
            }

            return true;
        }
    }

    return false;
}

MScriptFuncTable::Function * MScriptFuncTable::get ( const std::string & name,
                                                     const MScriptSrcLocation & location,
                                                     std::vector<MScriptValue> & arguments )
{
    std::string bareId;
    const MScriptNamespaces::NsDesc * ns = m_namespaces->analyseId ( name, bareId, &location );
    std::pair<FuncMultimap::const_iterator,FuncMultimap::const_iterator> range = m_funcTable.equal_range(bareId);
    std::pair<int,FuncMultimap::const_iterator> availability = std::make_pair(-1, range.first);
    size_t argc = arguments.size();

    for ( FuncMultimap::const_iterator i = range.first;
          i != range.second;
          i++ )
    {
        if ( argc >= i->second.m_argsRequired && argc <= i->second.m_params->size() )
        {
            int avb = ns->inheritsFrom ( i->second.m_ns );
            if (
                   ( -1 == availability.first )
                       ||
                   ( -1 != avb && avb < availability.first )
               )
            {
                availability.first = avb;
                availability.second = i;
            }
        }
    }

    if ( -1 != availability.first )
    {
        Function * result = new Function(availability.second->second);

        for ( size_t a = 0; a < arguments.size(); a++ )
        {
            result->m_params->at(a).m_value = &(arguments[a]);
        }

        return result;
    }

    if ( false == isDefined(name, location) )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "function `%1' in namespace `%2' has not been defined" )
                                                      . arg ( bareId.c_str() )
                                                      . arg ( ns->toString().c_str() )
                                                      . toStdString() );
    }
    else
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "function `%1' in namespace `%2' has not been defined for "
                                                        "the given number of arguments (%2)" )
                                                      . arg ( bareId.c_str() )
                                                      . arg ( ns->toString().c_str() )
                                                      . arg ( argc )
                                                      . toStdString() );
    }

    return NULL;
}

void MScriptFuncTable::clear()
{
    for ( FuncMultimap::iterator f = m_funcTable.begin();
          f != m_funcTable.end();
          f++ )
    {
        for ( std::vector<Parameter>::iterator p = f->second.m_params->begin();
              p != f->second.m_params->end();
              p++ )
        {
            if ( NULL != p->m_value )
            {
                p->m_value->completeDelete();
                delete p->m_value;
            }
        }

        f->second.m_code->completeDelete();
    }

    m_funcTable.clear();
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
        out << f->second.m_ns << f->first << "(" << f->second.m_params << ")" << std::endl;
        out << "    defined at: " << f->second.m_location.toString(table.m_interpret->getCoreBase()) << std::endl;
        out << "    required arguments: " << f->second.m_argsRequired << std::endl;
        out << sepLine1 << std::endl;
        out << f->second.m_code << std::endl;
        out << sepLine0 << std::endl;
    }

    return out;
}
