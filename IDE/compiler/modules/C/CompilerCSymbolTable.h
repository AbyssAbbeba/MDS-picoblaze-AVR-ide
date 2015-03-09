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
 * @file CompilerCSymbolTable.h
 */
// =============================================================================

#ifndef COMPILERCSYMBOLTABLE_H
#define COMPILERCSYMBOLTABLE_H

// Standard headers.
#include <map>
#include <string>
#include <vector>

// Forward declarations.
class CompilerCScope;
class CompilerStatement;

// C compiler header files.
#include "CompilerCDeclaration.h"

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCSymbolTable
 */
class CompilerCSymbolTable
{
    ////    Public Datatypes    ////
    public:

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerCSymbolTable();

        /**
         * @brief
         */
        virtual ~CompilerCSymbolTable();

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
            void newSymbol ( CompilerCDeclaration * symbol );

            /**
             * @brief
             * @param[in] dt
             */
            void newDataType ( CompilerCDeclaration * dt );
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
             * @param[in] type
             */
            const CompilerCDeclaration * findDataType ( const std::string & typeName,
                                                        CompilerCDeclaration::Type type );
        //@}

    ////    Private Attributes    ////
    private:
        ///
        CompilerCScope * m_rootScope;

        ///
        std::map<CompilerStatement *, CompilerCScope *> m_scopes;
};

#endif // COMPILERCSYMBOLTABLE_H
