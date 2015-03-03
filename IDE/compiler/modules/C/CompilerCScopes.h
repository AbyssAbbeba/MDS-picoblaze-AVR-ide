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
 * @file CompilerCScopes.h
 */
// =============================================================================

#ifndef COMPILERCSCOPES_H
#define COMPILERCSCOPES_H

// Standard headers.
#include <map>
#include <string>
#include <vector>

// Compiler header files.
#include "CompilerStatement.h"

// C compiler header files.
#include "CompilerCDeclaration.h"

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCScopes
 */
class CompilerCScopes
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Scope
        {
            Scope ( Scope * parent ) : m_parent(parent) {};

            Scope * const m_parent;
            std::vector<Scope*> m_chidren;

            std::map<std::string, const CompilerCDeclaration *> m_symbols;
            std::map<std::string, const CompilerCDeclaration *> m_datatypes;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerCScopes();

        /**
         * @brief
         */
        virtual ~CompilerCScopes();

    ////    Public Operations    ////
    public:
        /// @name Declaration phase.
        //@{
            /**
             * @brief
             * @param[in] scope
             */
            void enterScopeDeclaration ( const CompilerStatement * scope );

            /**
             * @brief
             */
            void leaveScopeDeclaration();

            /**
             * @brief
             * @param[in] symbol
             */
            void newSymbol ( const CompilerCDeclaration * symbol );

            /**
             * @brief
             * @param[in] dt
             */
            void newDataType ( const CompilerCDeclaration * dt );
        //@}

        /// @name Translation phase.
        //@{
            /**
             * @brief
             * @param[in] scope
             */
            void enterScope ( const CompilerStatement * scope );

            /**
             * @brief
             */
            void leaveScope();

            /**
             * @brief
             * @param[in] id
             */
            const CompilerCDeclaration * findSymbol ( const std::string & id );

            /**
             * @brief
             * @param[in] typeName
             */
            const CompilerCDeclaration * findDataType ( const std::string & typeName );
        //@}

    ////    Private Attributes    ////
    private:
        ///
        Scope * m_rootScope;

        ///
        std::map<const CompilerStatement *, Scope *> m_scopes;
};

#endif // COMPILERCSCOPES_H
