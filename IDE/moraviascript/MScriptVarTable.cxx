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
 * @file MScriptVarTable.cxx
 */
// =============================================================================

#include "MScriptVarTable.h"

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptValue.h"
#include "MScriptInterpretInterface.h"

// Standard header files.
#include <utility>

// Used for i18n only
#include <QObject>


MScriptVarTable::MScriptVarTable ( MScriptInterpretInterface * interpret )
                                 : m_interpret ( interpret )
{
    pushScope();
}

MScriptVarTable::~MScriptVarTable()
{
    clear();
}

void MScriptVarTable::pushScope()
{
    m_varTables.push_back(VarTable());
}

void MScriptVarTable::popScope()
{
    m_varTables.pop_back();
}

bool MScriptVarTable::remove ( const std::string & variable,
                               const MScriptSrcLocation & location,
                               const Index * index )
{
    bool result = false;

    for ( std::vector<VarTable>::reverse_iterator varTab = m_varTables.rbegin();
          varTab != m_varTables.rend();
          varTab++ )
    {
        VarTable::iterator var = varTab->find(variable);

        if ( varTab->end() == var )
        {
            continue;
        }

        if ( false == result )
        {
            if ( NULL == index )
            {
                varTab->erase(var);
            }
            else
            {
                MScriptValue ** arrayElement = access ( variable, index, &location );
                if ( NULL != arrayElement )
                {
                    *arrayElement = NULL;
                }
            }

            result = true;
        }
        else
        {
            const std::string eclipsedVarLoc = var->second.m_location.toString();
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_WARNING,
                                              QObject::tr ( "variable with the same name (`%1') has also been "
                                                            "declared in upper scope at %2 (this variable is no "
                                                            "longer eclipsed)" ) . arg ( variable.c_str() )
                                                                                 . arg ( eclipsedVarLoc.c_str() )
                                                                                 . toStdString() );
            break;
        }
    }

    if ( false == result )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "variable `%1' has not been declared (cannot delete "
                                                        "nonexistent variable)" ) . arg ( variable.c_str() )
                                                                                  . toStdString() );
    }

    return result;
}

bool MScriptVarTable::declare ( const std::string & variable,
                                const MScriptSrcLocation & location,
                                MScriptVariable::Flags flags,
                                unsigned int dimensions,
                                bool constant )
{
    VarTable::iterator var = m_varTables.back().find(variable);

    if ( m_varTables.back().end() == var )
    {
        /*
         * Variable has not yet been declared.
         */

        const MScriptSrcLocation * varLoc = getLocation(variable);
        if ( NULL != varLoc )
        {
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_WARNING,
                                              QObject::tr ( "this declaration eclipses variable `%1' declated at %2" )
                                                          . arg ( variable.c_str() )
                                                          . arg ( varLoc->toString().c_str() )
                                                          . toStdString() );
        }

        m_varTables.back().insert(std::make_pair(variable, MScriptVariable()));

        var = m_varTables.back().find(variable);
        var->second.m_flags = flags;
        var->second.m_location = location;

        if ( MScriptVariable::FLAG_ARRAY & flags )
        {
            if ( MScriptVariable::FLAG_HASH & flags )
            {
                var->second.m_value.m_hash = new std::map<std::string,MScriptVariable*>();
            }
            else
            {
                var->second.m_value.m_array = new std::vector<MScriptVariable*>;
            }
        }
        else
        {
            var->second.m_value.m_scalar = NULL;
        }

        return true;
    }
    else
    {
        /*
         * Variable has already been declared.
         */
        if ( var->second.m_flags != flags )
        {
            const std::string varLocation = var->second.m_location.toString();
            m_interpret->interpreterMessage ( location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "variable `%1' has been already be declared at %2 as %3, "
                                                            "cannot redeclare as %4" )
                                                          . arg ( variable.c_str() )
                                                          . arg ( varLocation.c_str() )
                                                          . arg ( flags2Str(var->second.m_flags).c_str() )
                                                          . arg ( flags2Str(flags).c_str() )
                                                          . toStdString() );
            return false;
        }
        else
        {
            return true;
        }
    }
}

bool MScriptVarTable::assign ( const std::string & variable,
                               const MScriptSrcLocation & location,
                               MScriptValue * value,
                               const Index * index )
{
    if ( true == declared ( variable, index) )
    {
        // Assign value to an exiting variable, or constant.
        MScriptValue ** var = access ( variable, index, &location );

        if ( MScriptVariable::FLAG_CONST & getFlags ( variable ) )
        {
            // Assigning value to a constant.
            if ( NULL != *var )
            {
                const std::string varLocation = getLocation(variable)->toString();
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "attempting to reassign a constant, `%1' has been "
                                                                "declared as constant at %2" )
                                                               . arg ( variable.c_str() )
                                                               . arg ( varLocation.c_str() )
                                                               . toStdString() );
                return false;
            }
        }
        else
        {
            // Assigning value to a variable.
            if ( NULL != *var )
            {
                (*var)->completeDelete();
                delete *var;
            }
        }

        *var = value;
        return true;
    }
    else
    {
        // Define a new variable.
        if ( false == declared ( variable ) )
        {
            MScriptVariable::Flags flags = MScriptVariable::FLAG_NO_FLAGS;
            int dim = 0;

            if ( NULL != index )
            {
                flags = MScriptVariable::FLAG_ARRAY;
                dim = index->dimensions();
            }

            if ( dim < 0 )
            {
                dim = -dim;
                flags = MScriptVariable::Flags ( flags | MScriptVariable::FLAG_HASH );
            }

            bool result = declare ( variable, location, flags, (unsigned int) dim );
            if ( false == result )
            {
                return false;
            }
        }

        MScriptValue ** var;
        if ( NULL == index )
        {
            var = access ( variable, index, &location );
        }
        else
        {
            var = newArrayElement ( variable, index, &location );
        }

        if ( NULL != var )
        {
            *var = value;
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool MScriptVarTable::defined ( const std::string & variable,
                                const Index * index )
{
    MScriptValue ** result = access ( variable, index );
    return ( ( NULL != result ) && ( NULL != *result ) );
}

bool MScriptVarTable::declared ( const std::string & variable,
                                 const Index * index )
{
    if ( NULL == index )
    {
        return ( MScriptVariable::FLAG_INVALID != getFlags ( variable ) );
    }
    else
    {
        return ( NULL != access ( variable, index ) );
    }
}

MScriptVariable::Flags MScriptVarTable::getFlags ( const std::string & variable )
{
    MScriptVariable * var = rawAccess ( variable );

    if ( NULL != var )
    {
        return var->m_flags;
    }
    else
    {
        return MScriptVariable::FLAG_INVALID;
    }
}

const MScriptSrcLocation * MScriptVarTable::getLocation ( const std::string & variable )
{
    MScriptVariable * var = rawAccess ( variable );

    if ( NULL != var )
    {
        return &( var->m_location );
    }
    else
    {
        return NULL;
    }
}

inline MScriptValue ** MScriptVarTable::newArrayElement ( const std::string & variable,
                                                          const Index * index,
                                                          const MScriptSrcLocation * location )
{
    MScriptVariable * var = rawAccess ( variable );

    if ( false == index->m_index.empty() )
    {
        for ( std::vector<unsigned int>::const_iterator i = index->m_index.cbegin();
              i != index->m_index.cend();
              i++ )
        {
            size_t size = var->m_value.m_array->size();
            if ( *i >= size )
            {
                var->m_value.m_array->resize ( 1 + *i );
                for ( size_t j = size; j < *i; j++ )
                {
                    var->m_value.m_array->at(j) = NULL;
                }
            }

            var = var->m_value.m_array->at(*i);
            if ( NULL == var )
            {
                var->m_value.m_array->at(*i) = new MScriptVariable();
                var = var->m_value.m_array->at(*i);

                var->m_location = *location;
                if ( index->m_index.cend() != ( i + 1 ) )
                {
                    var->m_flags = MScriptVariable::FLAG_ARRAY;
                    var->m_value.m_array = new std::vector<MScriptVariable*>();
                }
                else
                {
                    var->m_flags = MScriptVariable::FLAG_NO_FLAGS;
                    var->m_value.m_scalar = NULL;
                }
            }
            else if (
                        (
                            ( index->m_index.cend() != ( i + 1 ) )
                                &&
                            (
                               ( MScriptVariable::FLAG_ARRAY )
                                   !=
                               ( ( MScriptVariable::FLAG_ARRAY | MScriptVariable::FLAG_HASH ) & var->m_flags )
                            )
                        )
                            ||
                        (
                            ( index->m_index.cend() == ( i + 1 ) )
                                &&
                            (
                               ( MScriptVariable::FLAG_NO_FLAGS )
                                   !=
                               ( ( MScriptVariable::FLAG_ARRAY | MScriptVariable::FLAG_HASH ) & var->m_flags )
                            )
                        )
                    )
            {
                m_interpret->interpreterMessage ( *location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "cannot create new array element at the specified "
                                                                "index, number of dimensions and/or type of the array "
                                                                "is/are not compatible with the array declaration" )
                                                              . arg ( variable.c_str() )
                                                              . toStdString() );
                return NULL;
            }
        }

        return &( var->m_value.m_scalar );
    }
    else
    {
        for ( std::vector<std::string>::const_iterator i = index->m_key.cbegin();
              i != index->m_key.cend();
              i++ )
        {
            std::map<std::string,MScriptVariable*>::iterator it = var->m_value.m_hash->find(*i);

            if ( var->m_value.m_hash->end() != it )
            {
                var = it->second;

                if (
                       (
                           ( index->m_key.cend() != ( i + 1 ) )
                               &&
                           (
                               ( MScriptVariable::FLAG_ARRAY | MScriptVariable::FLAG_HASH )
                                   !=
                               ( ( MScriptVariable::FLAG_ARRAY | MScriptVariable::FLAG_HASH ) & var->m_flags )
                           )
                       )
                           ||
                       (
                           ( index->m_key.cend() == ( i + 1 ) )
                               &&
                           (
                               ( MScriptVariable::FLAG_NO_FLAGS )
                                   !=
                               ( ( MScriptVariable::FLAG_ARRAY | MScriptVariable::FLAG_HASH ) & var->m_flags )
                           )
                       )
                   )
                {
                    m_interpret->interpreterMessage ( *location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "cannot create new array element at the specified "
                                                                    "key vector, number of dimensions and/or type of "
                                                                    "the array is/are not compatible with the array "
                                                                    "declaration" )
                                                                  . arg ( variable.c_str() )
                                                                  . toStdString() );
                    return NULL;
                }
            }
            else
            {
                var->m_value.m_hash->insert ( std::make_pair ( *i, new MScriptVariable() ) );

                it = var->m_value.m_hash->find(*i);
                var = it->second;

                var->m_location = *location;
                if ( index->m_key.cend() != ( i + 1 ) )
                {
                    var->m_flags = MScriptVariable::Flags ( MScriptVariable::FLAG_ARRAY | MScriptVariable::FLAG_HASH );
                    var->m_value.m_hash = new std::map<std::string,MScriptVariable*>();
                }
                else
                {
                    var->m_flags = MScriptVariable::FLAG_NO_FLAGS;
                    var->m_value.m_scalar = NULL;
                }
            }
        }

        return &( var->m_value.m_scalar );
    }
}

inline MScriptVariable * MScriptVarTable::rawAccess ( const std::string & variable )
{
    for ( std::vector<VarTable>::reverse_iterator varTab = m_varTables.rbegin();
          varTab != m_varTables.rend();
          varTab++ )
    {
        VarTable::iterator var = varTab->find(variable);

        if ( varTab->cend() != var )
        {
            return &(var->second);
        }
    }

    return NULL;
}

MScriptValue ** MScriptVarTable::access ( const std::string & variable,
                                          const Index * index,
                                          const MScriptSrcLocation * location )
{
    MScriptVariable * cell = rawAccess ( variable );

    if ( NULL == cell )
    {
        if ( NULL != location )
        {
            m_interpret->interpreterMessage ( *location,
                                            MScriptBase::MT_ERROR,
                                            QObject::tr ( "variable `%1' has not been declared (cannot access "
                                                          "nonexistent variable)" ) . arg ( variable.c_str() )
                                                                                    . toStdString() );
        }
        return NULL;
    }


    if ( 0 == ( MScriptVariable::FLAG_ARRAY & cell->m_flags ) )
    {
        return &( cell->m_value.m_scalar );
    }

    if ( NULL == index )
    {
        if ( NULL != location )
        {
            m_interpret->interpreterMessage ( *location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "variable `%1' is an array (it cannot be accessed "
                                                            "as scalar)" ) . arg ( variable.c_str() )
                                                                           . toStdString() );
        }
        return NULL;
    }

    unsigned int dimensionNo = 0;

    if ( MScriptVariable::FLAG_HASH & cell->m_flags )
    {
        for ( std::vector<std::string>::const_iterator key = index->m_key.cbegin();
              key != index->m_key.cend();
              key++, dimensionNo++ )
        {
            int flags = ( ( MScriptVariable::FLAG_HASH | MScriptVariable::FLAG_ARRAY ) & cell->m_flags );

            if ( ( MScriptVariable::FLAG_HASH | MScriptVariable::FLAG_ARRAY ) != flags )
            {
                if ( NULL != location )
                {
                    m_interpret->interpreterMessage ( *location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "associative array `%1' has been declared with "
                                                                    "more dimensions than provided in the key vector" )
                                                                  . arg ( variable.c_str() )
                                                                  . toStdString() );
                }
                return NULL;
            }

            std::map<std::string,MScriptVariable*>::const_iterator it = cell->m_value.m_hash->find(*key);

            if ( cell->m_value.m_hash->cend() == it )
            {
                if ( NULL != location )
                {
                    m_interpret->interpreterMessage ( *location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "associative array `%1' does not contain an "
                                                                    "element at the specified key vector (%2), "
                                                                    "dimension #%3" )
                                                                  . arg ( variable.c_str() )
                                                                  . arg ( index->toString().c_str() )
                                                                  . arg ( dimensionNo )
                                                                  . toStdString() );
                }
                return NULL;
            }
            else
            {
                cell = it->second;

                if ( NULL == cell )
                {
                    break;
                }
            }
        }

        if (
               ( NULL == cell )
                   ||
               (
                   ( MScriptVariable::FLAG_HASH | MScriptVariable::FLAG_ARRAY )
                       ==
                   ( ( MScriptVariable::FLAG_HASH | MScriptVariable::FLAG_ARRAY ) & cell->m_flags )
               )
           )
        {
            if ( NULL != location )
            {
                m_interpret->interpreterMessage ( *location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "associative array `%1' has been declared with "
                                                                "less dimensions than provided in the index" )
                                                              . arg ( variable.c_str() )
                                                              . toStdString() );
            }
            return NULL;
        }
        else
        {
            return &( cell->m_value.m_scalar );
        }
    }
    else
    {
        for ( std::vector<unsigned int>::const_iterator idx = index->m_index.cbegin();
              idx != index->m_index.cend();
              idx++, dimensionNo++ )
        {
            int flags = ( ( MScriptVariable::FLAG_HASH | MScriptVariable::FLAG_ARRAY ) & cell->m_flags );

            if ( MScriptVariable::FLAG_ARRAY != flags )
            {
                if ( NULL != location )
                {
                    m_interpret->interpreterMessage ( *location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "indexed array `%1' has been declared with "
                                                                    "more dimensions than provided in the index" )
                                                                  . arg ( variable.c_str() )
                                                                  . toStdString() );
                }
                return NULL;
            }
            else if ( *idx >= cell->m_value.m_array->size() )
            {
                if ( NULL != location )
                {
                    m_interpret->interpreterMessage ( *location,
                                                      MScriptBase::MT_ERROR,
                                                      QObject::tr ( "indexed array `%1' does not contain an "
                                                                    "element at the specified index (%2), "
                                                                    "dimension #%3" )
                                                                  . arg ( variable.c_str() )
                                                                  . arg ( index->toString().c_str() )
                                                                  . arg ( dimensionNo )
                                                                  . toStdString() );
                }
                return NULL;
            }
            else
            {
                cell = cell->m_value.m_array->at(*idx);

                if ( NULL == cell )
                {
                    break;
                }
            }
        }

        if (
               ( NULL == cell )
                  ||
               (
                   MScriptVariable::FLAG_ARRAY
                       ==
                   ( ( MScriptVariable::FLAG_HASH | MScriptVariable::FLAG_ARRAY ) & cell->m_flags )
               )
           )
        {
            if ( NULL != location )
            {
                m_interpret->interpreterMessage ( *location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "indexed array `%1' has been declared with less "
                                                                "dimensions than provided in the index" )
                                                              . arg ( variable.c_str() )
                                                              . toStdString() );
            }
            return NULL;
        }
        else
        {
            return &( cell->m_value.m_scalar );
        }
    }
}

void MScriptVarTable::clear()
{
    m_varTables.clear();
}

std::string MScriptVarTable::Index::toString() const
{
    std::string result;

    if ( false == m_index.empty() )
    {
        for ( std::vector<unsigned int>::const_iterator i = m_index.cbegin();
              i != m_index.cend();
              i++ )
        {
            result += "[";
            result += *i;
            result += "]";
        }
    }
    else
    {
        for ( std::vector<std::string>::const_iterator i = m_key.cbegin();
              i != m_key.cend();
              i++ )
        {
            result += "[\"";
            result += *i;
            result += "\"]";
        }
    }

    return result;
}

int MScriptVarTable::Index::dimensions() const
{
    if ( false == m_index.empty() )
    {
        return (int) m_index.size();
    }
    else if ( false == m_key.empty() )
    {
        return (int) -(m_key.size());
    }
    else
    {
        return 0;
    }
}

std::string MScriptVarTable::flags2Str ( MScriptVariable::Flags flags ) const
{
    if ( flags & MScriptVariable::FLAG_INVALID )
    {
        return "<error>";
    }

    if ( flags & MScriptVariable::FLAG_ARRAY )
    {
        return std::string ( ( flags & MScriptVariable::FLAG_CONST ) ? "constant "   : ""        )
                         + ( ( flags & MScriptVariable::FLAG_HASH  ) ? "associative" : "indexed" )
                         + " array ";
    }
    else
    {
        if ( flags & MScriptVariable::FLAG_CONST )
        {
            return "scalar constant";
        }
        else
        {
            return "scalar variable";
        }
    }
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptVarTable & table )
{
    static const char * const sepLine0 = "==================================================";
    static const char * const sepLine1 = "--------------------------------------------------";

    int stackFrameNo = -1;
    for ( std::vector<MScriptVarTable::VarTable>::const_iterator varTab = table.m_varTables.cbegin();
          varTab != table.m_varTables.cend();
          varTab++ )
    {
        stackFrameNo++;
        out << sepLine0 << std::endl;
        out << "Scope level #" << stackFrameNo << ":" << std::endl;
        out << sepLine0 << std::endl;

        for ( MScriptVarTable::VarTable::const_iterator var = varTab->cbegin();
              var != varTab->cend();
              var++ )
        {
            out << "name: " << var->first << std::endl;
            out << "value(s): " << var->second << std::endl;
            out << sepLine1 << std::endl;
        }

        out << std::endl;
    }

    return out;
}
