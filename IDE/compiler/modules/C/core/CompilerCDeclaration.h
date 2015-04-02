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

// Forward declarations.
class CompilerExpr;
class CompilerStatement;

// Standard headers.
#include <string>
#include <vector>
#include <cstdint>
#include <ostream>

// C compiler header files.
#include "CompilerCType.h"

// Compiler header files.
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
            F_EMPTY             = 0x00000000, ///<

            TS_VOID             = 0x00000001, ///<
            TS_CHAR             = 0x00000002, ///<
            TS_SHORT            = 0x00000004, ///<
            TS_INT              = 0x00000008, ///<
            TS_LONG             = 0x00000010, ///<
            TS_LONG_LONG        = 0x00000020, ///<
            TS_FLOAT            = 0x00000040, ///<
            TS_DOUBLE           = 0x00000080, ///<
            TS_LONG_DOUBLE      = 0x00000100, ///<
            TS_BOOL             = 0x00000200, ///<
            TS_COMPLEX          = 0x00000400, ///<
            TS_IMAGINARY        = 0x00000800, ///<
            TS_UNSIGNED         = 0x00001000, ///<
            TS_SIGNED           = 0x00002000, ///<
            TS_ENUM             = 0x00004000, ///<
            TS_UNION            = 0x00008000, ///<
            TS_STRUCT           = 0x00010000, ///<

            TQ_CONST            = 0x00020000, ///<
            TQ_RESTRICT         = 0x00040000, ///<
            TQ_VOLATILE         = 0x00080000, ///<
            TQ_ATOMIC           = 0x00100000, ///<

            SC_STATIC           = 0x00200000, ///<
            SC_EXTERN           = 0x00400000, ///<
            SC_AUTO             = 0x00800000, ///<
            SC_REGISTER         = 0x01000000, ///<
            SC_TYPEDEF          = 0x02000000, ///<
            SC_THREAD_LOCAL     = 0x04000000, ///<

            FS_INLINE           = 0x08000000, ///<
            FS_NORETURN         = 0x10000000, ///<

            A_FUNCTION          = 0x20000000, ///<
//                       = 0x40000000, ///<
//                       = 0x80000000, ///<
        };

        /**
         * @brief
         */
        struct Allocation
        {
            ///
            Allocation();

            ///
            CompilerCType::MemorySpace m_memory;

            ///
            int m_address;

            ///
            int m_alignment;

            ///
            int m_bitFieldWidth;
        };

        /**
         * @brief
         */
        struct Name
        {
            ///
            Name();

            ///
            ~Name();

            ///
            std::string * m_id;

            ///
            std::string * m_type;

            ///
            std::string * m_typedef;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerCDeclaration ( const CompilerSourceLocation & location );

        /**
         * @brief
         */
        ~CompilerCDeclaration();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @return
         */
        CompilerCDeclaration * copyChainLink ( const CompilerSourceLocation & location ) const;

        /**
         * @brief
         * @return
         */
        CompilerCDeclaration * copyEntireChain() const;

        /**
         * @brief
         * @param[in,out] node
         */
        void addMembers ( CompilerCDeclaration * node );

        /**
         * @brief
         * @param[in,out] node
         */
        void addInstance ( CompilerCDeclaration * node );

        /**
         * @brief
         * @param[in,out] node
         */
        void addParameter ( CompilerCDeclaration * node );

        /**
         * @brief
         * @param[in,out] node
         */
        void unregisterMember ( const CompilerCDeclaration * const node );

        /**
         * @brief
         * @param[in,out] node
         */
        void appendLink ( CompilerCDeclaration * node );

        /**
         * @brief
         * @param[in,out] node
         */
        void prependLink ( CompilerCDeclaration * node );

        /**
         * @brief
         */
        void unlink();

        /**
         * @brief
         * @return
         */
        CompilerCDeclaration * first();

        /**
         * @brief
         * @return
         */
        CompilerCDeclaration * last();

    #ifndef NDEBUG
        /**
         * @brief
         * @param[in,out] out
         * @param[in] lineString
         */
        void print ( std::ostream & out,
                     std::string lineString = "" ) const;
    #endif // NDEBUG

    ////    Public Attributes    ////
    public:
        ///
        CompilerSourceLocation m_location;

        ///
        Type m_type;

        ///
        Allocation m_alloc;

        ///
        Name m_name;

        ///
        CompilerExpr * m_value;

        ///
        std::vector<Type> m_pointer;

        ///
        CompilerCDeclaration * m_array;

        ///
        CompilerCDeclaration * m_parameters;

        ///
        CompilerCDeclaration * m_instances;

        ///
        CompilerCDeclaration * m_overrule;

        ///
        CompilerCDeclaration * m_parent;

        ///
        CompilerCDeclaration * m_members;

        ///
        CompilerCDeclaration * m_next;

        ///
        CompilerCDeclaration * m_prev;
};

#ifndef NDEBUG
/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] decl
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerCDeclaration * decl );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] type
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerCDeclaration::Type type );
//@}
#endif // NDEBUG

#endif // COMPILERCDECLARATION_H
