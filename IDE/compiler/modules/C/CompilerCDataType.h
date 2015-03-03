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
            A_REFERS2CONST = 0x100,
            A_FUNCTION     = 0x200
        };

        enum Type
        {
            DT_VOID     = 1,
            DT_CHAR     = 2,
            DT_SHORT    = 4,
            DT_INT      = 5,
            DT_FLOAT    = 6,
            DT_DOUBLE   = 7,

            DT_UNSIGNED = 1 << 8,
            DT_LONG     = 2 << 8,

            DT_ENUM     = 1 << 16,
            DT_UNION    = 2 << 16,
            DT_STRUCT   = 3 << 16,
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
        Type m_type;
        Attribute m_attribute;

        unsigned int m_pointer;
        std::vector<unsigned int> * m_array;

        std::string * m_id;
        std::string * m_typename;

        CompilerCDataType * m_owner;
        std::vector<CompilerCDataType*> * m_members;
};

#endif // COMPILERCDATATYPE_H
