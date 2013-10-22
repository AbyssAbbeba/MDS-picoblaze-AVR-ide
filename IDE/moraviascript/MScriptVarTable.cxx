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
#include "MScriptNamespaces.h"
#include "MScriptInterpretInterface.h"

// Standard header files.
#include <cstdio>
#include <utility>
#include <cstring>
#include <cstdlib>

// Used for i18n only
#include <QObject>


MScriptVarTable::MScriptVarTable ( MScriptInterpretInterface * interpret,
                                   MScriptNamespaces * namespaces )
                                 :
                                   m_interpret ( interpret ),
                                   m_namespaces ( namespaces )
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
                MScriptValue * arrayElement = access ( variable, index, &location );
                if ( NULL != arrayElement )
                {
                    arrayElement->completeDelete();
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

void MScriptVarTable::declare ( const std::string & variable,
                                const MScriptSrcLocation & location,
                                MScriptVariable::Flags flags,
                                unsigned int dimensions )
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
                var->second.m_value.m_hash = new std::map<std::string,MScriptVariable>();
            }
            else
            {
                var->second.m_value.m_array = new std::vector<MScriptVariable>;
            }

            return;
        }
        else
        {
            var->second.m_value.m_scalar = MScriptValue();
        }
    }
    else if ( var->second.m_flags != flags )
    {
        /*
         * Variable has already been declared.
         */

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
    }
}

void MScriptVarTable::assign ( const std::string & variable,
                               const MScriptSrcLocation & location,
                               const MScriptValue & value,
                               const Index * index )
{
    if ( true == declared ( variable, index) )
    {
        // Assign value to an exiting variable, or constant.
        MScriptValue * var = access ( variable, index, &location );

        if ( MScriptVariable::FLAG_CONST & getFlags ( variable ) )
        {
            // Assigning value to a constant.
            if ( MScriptValue::TYPE_EMPTY != var->m_type )
            {
                const std::string varLocation = getLocation(variable)->toString();
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "attempting to reassign a constant, `%1' has been "
                                                                "declared as constant at %2" )
                                                               . arg ( variable.c_str() )
                                                               . arg ( varLocation.c_str() )
                                                               . toStdString() );
                return;
            }
        }
        else
        {
            // Assigning value to a variable.
            var->completeDelete();
        }

        value.makeCopy(*var);
        return;
    }

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

        declare ( variable, location, flags, (unsigned int) dim );
    }

    MScriptValue * var;
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
        value.makeCopy(*var);
    }
}

bool MScriptVarTable::defined ( const std::string & variable,
                                const Index * index )
{
    return ( NULL != access ( variable, index ) );
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

inline MScriptValue * MScriptVarTable::newArrayElement ( const std::string & variable,
                                                         const Index * index,
                                                         const MScriptSrcLocation * location )
{
    MScriptVariable * var = rawAccess ( variable );

    if ( true == index->m_index.empty() )
    {
        return newArrayElementKey ( variable, index, location, var );
    }
    else
    {
        return newArrayElementIdx ( variable, index, location, var );
    }
}

inline MScriptValue * MScriptVarTable::newArrayElementIdx ( const std::string & variable,
                                                            const Index * index,
                                                            const MScriptSrcLocation * location,
                                                            MScriptVariable * var )
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
                MScriptVariable * var0 = &( var->m_value.m_array->at(j) );
                var0->m_location = *location;

                if ( index->m_index.cend() == ( i + 1 ) )
                {
                    var0->m_flags = MScriptVariable::FLAG_NO_FLAGS;
                    var0->m_value.m_scalar = MScriptValue();
                }
                else
                {
                    var0->m_flags = MScriptVariable::FLAG_ARRAY;
                    var0->m_value.m_array = new std::vector<MScriptVariable>();
                }
            }
        }

        var = &( var->m_value.m_array->at(*i) );
        int flags = ( ( MScriptVariable::FLAG_ARRAY | MScriptVariable::FLAG_HASH ) & var->m_flags );

        if (
               ( index->m_index.cend() == ( i + 1 ) )
                   ?
               ( MScriptVariable::FLAG_NO_FLAGS != flags )
                   :
               ( MScriptVariable::FLAG_ARRAY    != flags )
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

inline MScriptValue * MScriptVarTable::newArrayElementKey ( const std::string & variable,
                                                            const Index * index,
                                                            const MScriptSrcLocation * location,
                                                            MScriptVariable * var )
{
    for ( std::vector<std::string>::const_iterator i = index->m_key.cbegin();
          i != index->m_key.cend();
          i++ )
    {
        std::map<std::string,MScriptVariable>::iterator it = var->m_value.m_hash->find(*i);

        if ( var->m_value.m_hash->end() != it )
        {
            var = &( it->second );
            int flags = ( ( MScriptVariable::FLAG_ARRAY | MScriptVariable::FLAG_HASH ) & var->m_flags );

            if (
                   ( index->m_key.cend() == ( i + 1 ) )
                       ?
                   ( MScriptVariable::FLAG_NO_FLAGS != flags )
                       :
                   ( ( MScriptVariable::FLAG_ARRAY | MScriptVariable::FLAG_HASH ) != flags )
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
            var->m_value.m_hash->insert ( std::make_pair ( *i, MScriptVariable() ) );

            it = var->m_value.m_hash->find(*i);
            var = &( it->second );

            var->m_location = *location;
            if ( index->m_key.cend() != ( i + 1 ) )
            {
                var->m_flags = MScriptVariable::Flags ( MScriptVariable::FLAG_ARRAY | MScriptVariable::FLAG_HASH );
                var->m_value.m_hash = new std::map<std::string,MScriptVariable>();
            }
            else
            {
                var->m_flags = MScriptVariable::FLAG_NO_FLAGS;
                var->m_value.m_scalar = MScriptValue();
            }
        }
    }

    return &( var->m_value.m_scalar );
}

inline MScriptVariable * MScriptVarTable::rawAccess ( const std::string & variable,
                                                      int * level )
{
    int enteredLevel = 0;

    for ( std::vector<VarTable>::reverse_iterator varTab = m_varTables.rbegin();
          varTab != m_varTables.rend();
          varTab++, enteredLevel++ )
    {
        VarTable::iterator var = varTab->find(variable);

        if ( varTab->cend() == var )
        {
            continue;
        }

        if ( NULL != level )
        {
            if ( -1 != *level )
            {
                if ( enteredLevel != *level )
                {
                    continue;
                }
            }
            *level = enteredLevel;
        }

        return &(var->second);
    }

    return NULL;
}

MScriptValue * MScriptVarTable::access ( const std::string & variable,
                                         const Index * index,
                                         const MScriptSrcLocation * location,
                                         int * level )
{
    MScriptVariable * cell = rawAccess ( variable, level );

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
        return reaccess ( variable, cell->m_value.m_scalar, location, index );
    }

    if ( NULL == index )
    {
        if ( NULL != location )
        {
            m_interpret->interpreterMessage ( *location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "variable `%1' is array (it cannot be accessed as scalar)" )
                                                           . arg ( variable.c_str() )
                                                           . toStdString() );
        }
        return NULL;
    }

    if ( MScriptVariable::FLAG_HASH & cell->m_flags )
    {
        return accessHash ( variable, index, location, cell );
    }
    else
    {
        return accessArray ( variable, index, location, cell );
    }
}

inline MScriptValue * MScriptVarTable::accessArray ( const std::string & variable,
                                                     const Index * index,
                                                     const MScriptSrcLocation * location,
                                                     MScriptVariable * cell )
{
    unsigned int dimensionNo = 0;

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
            cell = &( cell->m_value.m_array->at(*idx) );
        }
    }

    const int flags = ( ( MScriptVariable::FLAG_HASH | MScriptVariable::FLAG_ARRAY ) & cell->m_flags );
    if ( MScriptVariable::FLAG_ARRAY == flags )
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
        return reaccess ( variable, cell->m_value.m_scalar, location );
    }
}

inline MScriptValue * MScriptVarTable::accessHash ( const std::string & variable,
                                                    const Index * index,
                                                    const MScriptSrcLocation * location,
                                                    MScriptVariable * cell )
{
    unsigned int dimensionNo = 0;

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

        std::map<std::string,MScriptVariable>::iterator it = cell->m_value.m_hash->find(*key);

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
            cell = &( it->second );
        }
    }

    const int flags = ( ( MScriptVariable::FLAG_HASH | MScriptVariable::FLAG_ARRAY ) & cell->m_flags );
    if ( ( MScriptVariable::FLAG_HASH | MScriptVariable::FLAG_ARRAY ) == flags )
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
        return reaccess ( variable, cell->m_value.m_scalar, location );
    }
}

inline void MScriptVarTable::derefer ( const char * reference,
                                       int * level,
                                       std::string * variable,
                                       Index * index ) const
{
    char * origStr = new char [ strlen(reference) + 1 ];
    char * str = origStr;
    char * next;

    strcpy(origStr, reference);

    next = strchr ( str, '.' );
    next[0] = '\0';
    sscanf ( str, "%d", level );
    str = ( next + 1 );

    next = strchr ( str, '.' );
    next[0] = '\0';
    *variable = str;
    str = ( next + 1 );

    if ( '\0' == str[0] )
    {
        // No array element specifier.
        return;
    }

    next = strchr ( str, '.' );
    next[0] = '\0';
    str = ( next + 1 );

    if ( 'A' == str[0] )
    {
        // Index vector.
        unsigned int number;
        while ( NULL != ( next = strchr ( str, '\n' ) ) )
        {
            next[0] = '\0';
            sscanf ( str, "%d", &number );
            str = ( next + 1 );
            index->m_index.push_back(number);
        }
    }
    else
    {
        // Key vector.
        while ( NULL != ( next = strchr ( str, '\n' ) ) )
        {
            next[0] = '\0';
            index->m_key.push_back(str);
            str = ( next + 1 );
        }
    }
}

inline MScriptValue * MScriptVarTable::reaccess ( const std::string & variable,
                                                  MScriptValue & input,
                                                  const MScriptSrcLocation * location,
                                                  const Index * index )
{
    if ( MScriptValue::TYPE_SYMBOL == input.m_type )
    {
        int level;
        std::string targetVar;
        Index targetIndex;

        derefer ( input.m_data.m_symbol, &level, &targetVar, &targetIndex );

        if ( 0 != index->dimensions() && 0 != targetIndex.dimensions() )
        {
            m_interpret->interpreterMessage ( *location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "variable referenced by `%1' is an array element, i.e. it "
                                                            "is scalar (it cannot be accessed as array)" )
                                                           . arg ( variable.c_str() )
                                                           . toStdString() );
            return NULL;
        }

        const Index * accessIndex = index;
        if ( 0 != targetIndex.dimensions() )
        {
            accessIndex = &targetIndex;
        }
        if ( 0 == accessIndex->dimensions() )
        {
            accessIndex = NULL;
        }

        return access ( input.m_data.m_symbol, accessIndex, location, &level );
    }
    else if ( NULL != index )
    {
        if ( NULL != location )
        {
            m_interpret->interpreterMessage ( *location,
                                              MScriptBase::MT_ERROR,
                                              QObject::tr ( "variable `%1' is scalar (it cannot be accessed as array)" )
                                                           . arg ( variable.c_str() )
                                                           . toStdString() );
        }

        return NULL;
    }
    else
    {
        return &input;
    }
}

void MScriptVarTable::refer ( const std::string & refName,
                              const std::string & refTarget,
                              const MScriptSrcLocation & location,
                              const Index * refIndex,
                              const Index * targetIndex )
{
    int level = -1;
    MScriptValue * target = access ( refTarget, targetIndex, &location, &level );
    if ( NULL == target )
    {
        return;
    }

    std::string refString;

    {
        char buffer[10];
        sprintf(buffer, "%d", level);
        refString = buffer;
        refString += ".";
    }

    refString += refTarget;
    refString += ".";

    if ( 0 != targetIndex->dimensions() )
    {
        refString += ".";

        if ( 0 > targetIndex->dimensions() )
        {
            // Lower than zero -> associative array.
            refString += "B\n";
            for ( std::vector<std::string>::const_iterator it = targetIndex->m_key.cbegin();
                  it != targetIndex->m_key.cend();
                  it++ )
            {
                refString += *it;
                refString += '\n';
            }
        }
        else
        {
            // Higher than zero -> indexed array.
            refString += "A\n";
            for ( std::vector<unsigned int>::const_iterator it = targetIndex->m_index.cbegin();
                  it != targetIndex->m_index.cend();
                  it++ )
            {
                char buffer[10];
                sprintf(buffer, "%d\n", *it);
                refString = buffer;
            }
        }
    }

    MScriptValue value;
    value.m_type = MScriptValue::TYPE_SYMBOL;
    value.m_data.m_symbol = (char*) malloc( refString.size() + 1 );
    strcpy(value.m_data.m_symbol, refString.c_str());
    assign ( refName, location, value, refIndex );
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
    if ( NULL == this )
    {
        return 0;
    }

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

void MScriptVarTable::Index::clear()
{
    m_index.clear();
    m_key.clear();
}

std::string MScriptVarTable::flags2Str ( MScriptVariable::Flags flags ) const
{
    if ( MScriptVariable::FLAG_INVALID == flags )
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
