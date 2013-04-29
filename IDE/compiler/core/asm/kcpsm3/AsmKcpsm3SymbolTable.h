// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3SymbolTable.h
 */
// =============================================================================

#ifndef ASMKCPSM3SYMBOLTABLE_H
#define ASMKCPSM3SYMBOLTABLE_H

#include "../../CompilerCore.h"
#include "../../CompilerOptions.h"
#include "../../CompilerExpr.h"
#include "../../StatementTypes.h"

#include <vector>
#include <string>
#include <ostream>

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3SymbolTable
 */
class AsmKcpsm3SymbolTable
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AsmKcpsm3SymbolTable * symbolTable );

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum SymbolType
        {
            STYPE_INVALID = -1, ///<

            STYPE_NUMBER,       ///<
            STYPE_LABEL         ///<
        };

        /**
         * @brief
         */
        struct Symbol
        {
            Symbol ( const char * name,
                     SymbolType type,
                     const CompilerExpr * value = NULL )
            {
                m_name.assign ( name );
                m_type = type;
                if ( NULL != value )
                {
                    m_value = *value;
                }
            }

            std::string m_name;
            CompilerExpr m_value;
            SymbolType m_type;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @return
         */
        AsmKcpsm3SymbolTable ( CompilerCore * compilerCore,
                               CompilerOptions * opts )
                             : m_compilerCore ( compilerCore ),
                               m_opts ( opts ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] name
         * @param[in] value
         */
        void addSymbol ( CompilerExpr * const name,
                         StatementTypes::StatementType type,
                         CompilerExpr * const value = NULL );

        /**
         * @brief
         * @param[in,out] expr
         * @param[in] bitsMax
         */
        unsigned int resolveExpr ( CompilerExpr * expr,
                                   int bitsMax = -1 );

    ////    Private Attributes    ////
    private:
        ///
        std::vector<Symbol> m_table;

        ///
        CompilerCore * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] symbolTable
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const AsmKcpsm3SymbolTable * symbolTable );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] symbolType
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const AsmKcpsm3SymbolTable::SymbolType symbolType );
//@}

#endif // ASMKCPSM3SYMBOLTABLE_H
