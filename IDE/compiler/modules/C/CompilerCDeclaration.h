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
 * @file CompilerCDeclaration.h
 */
// =============================================================================

#ifndef COMPILERCDECLARATION_H
#define COMPILERCDECLARATION_H

// Standard headers.
#include <map>
#include <string>
#include <vector>
#include <utility>

// Compiler header files.
#include "CompilerExpr.h"
#include "CompilerSourceLocation.h"

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCDeclaration
 */
class CompilerCDeclaration
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Attribute
        {
            A_CONST        = 0x001, ///<
            A_STATIC       = 0x002, ///<
            A_EXTERN       = 0x004, ///<
            A_AUTO         = 0x008, ///<
            A_VOLATILE     = 0x010, ///<
            A_REGISTER     = 0x020, ///<
            A_RESTRICT     = 0x040, ///<
            A_REFERENCE    = 0x080, ///<
            A_REFERS2CONST = 0x100, ///<
            A_FUNCTION     = 0x200, ///<
            A_INLINE       = 0x400, ///<
        };

        /**
         * @brief
         */
        enum Type
        {
            DT_VOID      = 1,       ///<
            DT_CHAR      = 2,       ///<
            DT_SHORT     = 4,       ///<
            DT_INT       = 5,       ///<
            DT_FLOAT     = 6,       ///<
            DT_DOUBLE    = 7,       ///<
            DT_BOOL      = 8,       ///<

            DT_ENUM      = 1 << 8,  ///<
            DT_UNION     = 2 << 8,  ///<
            DT_STRUCT    = 3 << 8,  ///<

            DT_UNSIGNED  = 1 << 16, ///<
            DT_LONG      = 2 << 16, ///<
            DT_LONG_LONG = 4 << 16, ///<
            DT_COMPLEX   = 8 << 16, ///<
        };

        /**
         * @brief
         */
        enum MemorySpace
        {
            MS_NONE, ///<
            MS_CODE, ///<
            MS_REG,  ///<
            MS_PORT, ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerCDeclaration();

        /**
         * @brief
         */
        ~CompilerCDeclaration();

    ////    Public Operations    ////
    public:

    ////    Private Attributes    ////
    private:
        ///
        Type m_type;

        ///
        Attribute m_attribute;

        ///
        CompilerSourceLocation m_location;

        ///
        unsigned int m_pointer;

        ///
        std::vector<unsigned int> * m_array;

        ///
        std::string * m_id;

        ///
        std::string * m_typename;

        ///
        CompilerExpr * m_value;

        ///
        CompilerCDeclaration * m_owner;

        ///
        std::vector<CompilerCDeclaration *> * m_members;

        ///
        std::pair<MemorySpace, int> m_allocator;
};

#endif // COMPILERCDECLARATION_H
