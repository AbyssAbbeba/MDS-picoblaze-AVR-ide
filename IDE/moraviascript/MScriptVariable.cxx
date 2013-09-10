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
 * @file MScriptVariable.cxx
 */
// =============================================================================

#include "MScriptVariable.h"

// MScript language interpreter header files.
#include "MScriptValue.h"

MScriptVariable::~MScriptVariable()
{
    if ( FLAG_ARRAY & m_flags )
    {
        if ( FLAG_HASH & m_flags )
        {
            for ( std::map<std::string,MScriptVariable*>::const_iterator var = m_value.m_hash->cbegin();
                  var != m_value.m_hash->cend();
                  var++ )
            {
                if ( NULL != var->second )
                {
                    delete var->second;
                }
            }
            delete m_value.m_hash;
        }
        else
        {
            for ( std::vector<MScriptVariable*>::const_iterator var = m_value.m_array->cbegin();
                  var != m_value.m_array->cend();
                  var++ )
            {
                if ( NULL != *var )
                {
                    delete *var;
                }
            }
            delete m_value.m_array;
        }
    }
    else if ( NULL != m_value.m_scalar )
    {
        delete m_value.m_scalar;
    }
}

std::ostream & operator << ( std::ostream & out,
                             MScriptVariable::Flags flags )
{
    bool comma = false;

    if ( MScriptVariable::FLAG_CONST & flags )
    {
        out << "CONST";
        comma = true;
    }

    if ( MScriptVariable::FLAG_ARRAY & flags )
    {
        if ( true == comma )
        {
            out << ", ";
        }
        out << "ARRAY";
        comma = true;
    }

    if ( MScriptVariable::FLAG_HASH & flags )
    {
        if ( true == comma )
        {
            out << ", ";
        }
        out << "HASH";
        comma = true;
    }

    if ( false == comma )
    {
        out << "<NONE>";
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptVariable & variable )
{
    out << "flags: " << variable.m_flags << std::endl;
    out << "declared at: " << variable.m_location << std::endl;
    out << "value: ";

    if ( NULL == variable.m_value.m_hash )
    {
        out << "<UNDEFINDED>" << std::endl;
    }
    else
    {
        if ( MScriptVariable::FLAG_ARRAY & variable.m_flags )
        {
            if ( MScriptVariable::FLAG_HASH & variable.m_flags )
            {
                const std::map<std::string,MScriptVariable*> * hashTable = variable.m_value.m_hash;
                for ( std::map<std::string,MScriptVariable*>::const_iterator cell = hashTable->cbegin();
                      cell != hashTable->cend();
                      cell++ )
                {
                    out << "  [\"" << cell->first << "\"] : ";
                    if ( NULL == cell->second )
                    {
                        out << "<UNDEFINDED>";
                    }
                    else
                    {
                        out << *(cell->second);
                    }
                    out << std::endl;
                }
            }
            else
            {
                size_t size = variable.m_value.m_array->size();
                for ( size_t i = 0; i < size; i++ )
                {
                    const MScriptVariable * value = variable.m_value.m_array->at(i);

                    out << "  [" << i << "] : ";
                    if ( NULL == value )
                    {
                        out << "<UNDEFINDED>";
                    }
                    else
                    {
                        out << *value;
                    }
                    out << std::endl;
                }
            }
        }
        else
        {
            out << variable.m_value.m_scalar << std::endl;
        }
    }

    return out;
}
