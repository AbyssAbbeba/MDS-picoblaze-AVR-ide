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
 * @file MScriptVariable.cxx
 */
// =============================================================================

#include "MScriptVariable.h"

MScriptVariable::MScriptVariable()
{
    m_flags = FLAG_ARRAY;
    m_value.m_array = nullptr;
    m_id = -1;
}

MScriptVariable::MScriptVariable ( const MScriptVariable & obj )
{
    m_location = obj.m_location;
    m_flags = obj.m_flags;
    m_ns = obj.m_ns;
    m_id = m_id;

    if ( m_flags & FLAG_ARRAY )
    {
        if ( m_flags & FLAG_HASH )
        {
            m_value.m_hash = obj.m_value.m_hash;
        }
        else
        {
            m_value.m_array = obj.m_value.m_array;
        }
    }
    else
    {
        m_value.m_scalar = obj.m_value.m_scalar;
    }
}

MScriptVariable::~MScriptVariable()
{
    if ( FLAG_ARRAY & m_flags )
    {
        if ( FLAG_HASH & m_flags )
        {
            if ( nullptr != m_value.m_hash )
            {
                delete m_value.m_hash;
            }
        }
        else
        {
            if ( nullptr != m_value.m_array )
            {
                delete m_value.m_array;
            }
        }
    }
    else
    {
        m_value.m_scalar.completeDelete();
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

    if ( nullptr == variable.m_value.m_hash )
    {
        out << "<UNDEFINDED>" << std::endl;
    }
    else
    {
        if ( MScriptVariable::FLAG_ARRAY & variable.m_flags )
        {
            if ( MScriptVariable::FLAG_HASH & variable.m_flags )
            {
                for ( const auto & cell : *(variable.m_value.m_hash) )
                {
                    out << "  [\"" << cell.first << "\"] : " << cell.second << std::endl;
                }
            }
            else
            {
                size_t size = variable.m_value.m_array->size();
                for ( size_t i = 0; i < size; i++ )
                {
                    out << "  [" << i << "] : " << variable.m_value.m_array->at(i) << std::endl;
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
