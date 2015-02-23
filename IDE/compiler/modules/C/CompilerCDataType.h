// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCDataType.h
 */
// =============================================================================

#ifndef COMPILERCDATATYPE_H
#define COMPILERCDATATYPE_H

// Standard headers.
#include <map>
#include <string>
#include <vector>

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCDataType
 */
class CompilerCDataType
{
    ////    Private Datatypes    ////
    private:
        enum Attribute
        {
            A_CONST        = 0x001,
            A_STATIC       = 0x002,
            A_EXTERN       = 0x004,
            A_AUTO         = 0x008,
            A_VOLATILE     = 0x010,
            A_REGISTER     = 0x020,
            A_RESTRICT     = 0x040,
            A_REFERENCE    = 0x080,
            A_REFERS2CONST = 0x100
        };
"enum"          { return DATA_ENUM;         }
"union"         { return DATA_UNION;        }
"struct"        { return DATA_STRUCT;       }

"typedef"       { return DATA_TYPEDEF;      }


        enum BuiltIn
        {
            DT_VOID     = 0x01,

            DT_CHAR     = 0x02,
            DT_SHORT    = 0x04,
            DT_INT      = 0x08,

            DT_FLOAT    = 0x10,
            DT_DOUBLE   = 0x20,

            DT_UNSIGNED = 0x40,
            DT_LONG     = 0x80,
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerCDataType() {}

    ////    Public Operations    ////
    public:

    ////    Private Attributes    ////
    private:
        Attribute m_attribute;
        unsigned int m_array;
        unsigned int m_pointer;
};

#endif // COMPILERCDATATYPE_H
