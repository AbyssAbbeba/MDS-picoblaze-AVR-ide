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
 * @file CompilerCType.h
 */
// =============================================================================

#ifndef COMPILERCTYPE_H
#define COMPILERCTYPE_H

// Compiler header files.
#include "CompilerSourceLocation.h"

// Standard header files.
#include <string>
#include <vector>
#include <utility>
#include <ostream>

namespace CompilerCType
{
    struct Type
    {
        // TODO: protected constructor
        virtual void print ( std::ostream & out ) const;

        bool m_atomic;
        bool m_constant;
        bool m_volatile;

        bool m_static;
        bool m_extern;
        bool m_register;

        int m_size;
        int m_alignment;

        CompilerSourceLocation m_location;
    };

    struct Basic : public Type
    {
        virtual void print ( std::ostream & out ) const;

        enum Type
        {
            VOID,
            BOOLEAN,
            SIGNED_INT,
            UNSIGNED_INT,
            REAL_FLOAT,
            IMAGINARY_FLOAT,
            COMPLEX_FLOAT,
        } m_type;
    };

    struct BitField : public Type
    {
        virtual void print ( std::ostream & out ) const;

        int m_width;
    };

    struct Function : public Type
    {
        virtual void print ( std::ostream & out ) const;

        bool m_inline;
        bool m_noreturn;
        const Type * m_return;
        std::vector<const Type *> m_parameters;
    };

    struct Array : public Type
    {
        virtual void print ( std::ostream & out ) const;

        bool m_vla;
        int m_size;
    };

    struct Pointer : public Type
    {
        virtual void print ( std::ostream & out ) const;

        bool m_restrict;
        const Type * m_target;
    };

    struct Struct : public Type
    {
        virtual void print ( std::ostream & out ) const;

        bool m_union;
        std::string m_tag;
        std::vector<std::pair<std::string,const Type*>> m_members;
    };

    struct Enum : public Basic
    {
        virtual void print ( std::ostream & out ) const;

        std::string m_tag;
        std::vector<std::pair<std::string,int>> m_members;
    };
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] type
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerCType::Type * type );
//@}

#endif // COMPILERCTYPE_H
