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
        enum Type
        {
            DT_VOID             = 1 <<  0, ///<
            DT_CHAR             = 2 <<  0, ///<
            DT_SHORT            = 4 <<  0, ///<
            DT_INT              = 8 <<  0, ///<
            DT_LONG             = 1 <<  4, ///<
            DT_LONG_LONG        = 2 <<  4, ///<
            DT_FLOAT            = 4 <<  4, ///<
            DT_DOUBLE           = 8 <<  4, ///<
            DT_LONG_DOUBLE      = 1 <<  8, ///<
            DT_BOOL             = 2 <<  8, ///<
            DT_COMPLEX          = 4 <<  8, ///<
            DT_IMAGINARY        = 8 <<  8, ///<
            DT_UNSIGNED         = 1 << 12, ///<
            DT_SIGNED           = 2 << 12, ///<
            DT_ENUM             = 4 << 12, ///<
            DT_UNION            = 8 << 12, ///<
            DT_STRUCT           = 1 << 16, ///<
            A_CONST             = 2 << 16, ///<
            A_STATIC            = 4 << 16, ///<
            A_EXTERN            = 8 << 16, ///<
            A_AUTO              = 1 << 20, ///<
            A_VOLATILE          = 2 << 20, ///<
            A_REGISTER          = 4 << 20, ///<
            A_RESTRICT          = 8 << 20, ///<
            A_REFERENCE         = 1 << 24, ///<
            A_REFERS2CONST      = 2 << 24, ///<
            A_FUNCTION          = 4 << 24, ///<
            A_INLINE            = 8 << 24, ///<
            A_TYPEDEF           = 1 << 28, ///<
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
        CompilerSourceLocation m_location;

        ///
        unsigned int m_pointer;

        ///
        std::vector<CompilerCDeclaration *> * m_array;

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
