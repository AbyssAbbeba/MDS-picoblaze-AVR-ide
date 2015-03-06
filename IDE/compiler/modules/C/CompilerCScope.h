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

// Forward declarations.
class CompilerCDeclaration;

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCScope
 */
class CompilerCScope
{
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
        std::vector<CompilerCScope *> m_chidren;

        ///
        std::multimap<std::string, CompilerCDeclaration *> m_symbols;

        ///
        std::multimap<std::string, CompilerCDeclaration *> m_datatypes;
};

#endif // COMPILERCSCOPE_H
