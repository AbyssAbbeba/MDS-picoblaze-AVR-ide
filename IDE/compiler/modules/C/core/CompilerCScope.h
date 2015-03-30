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
 * @file CompilerCScope.h
 */
// =============================================================================

#ifndef COMPILERCSCOPE_H
#define COMPILERCSCOPE_H

// Standard headers.
#include <map>
#include <string>
#include <vector>
#include <ostream>

// Forward declarations.
class CompilerCDeclaration;

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCScope
 */
class CompilerCScope
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const CompilerCScope * scope );

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Type
        {
            TYPE_FILE,           ///<
            TYPE_FUNC_PROTOTYPE, ///<
            TYPE_FUNCTION,       ///<
            TYPE_BLOCK,          ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] parent
         * @param[in] type
         */
        CompilerCScope ( CompilerCScope * parent,
                         Type type = TYPE_BLOCK );

        /**
         * @brief
         */
        virtual ~CompilerCScope();

    ////    Public Operations    ////
    public:

    ////    Public Attributes    ////
    private:
        ///
        Type m_type;

        ///
        CompilerCScope * const m_parent;

        ///
        std::vector<CompilerCScope *> m_members;

        ///
        std::vector<CompilerCDeclaration *> m_symbols;

        ///
        std::vector<CompilerCDeclaration *> m_datatypes;

        ///
        std::map<std::string, int> m_symbolMap;

        ///
        std::multimap<std::string, int> m_typeMap;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] scope
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const CompilerCScope * scope );
//@}

#endif // COMPILERCSCOPE_H
