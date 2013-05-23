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

// Common compiler header files.
#include "../../CompilerSemanticInterface.h"
#include "../../CompilerOptions.h"

// Standard headers.
#include <map>
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
        enum ExprValSide
        {
            LEFT,  ///<
            RIGHT  ///<
        };

        /**
         * @brief
         */
        enum SymbolType
        {
            STYPE_UNSPECIFIED = 0x00, ///<

            STYPE_NUMBER      = 0x01, ///<
            STYPE_REGISTER    = 0x02, ///<
            STYPE_LABEL       = 0x04, ///<
            STYPE_PORT        = 0x08, ///<
            STYPE_DATA        = 0x10, ///<
            STYPE_EXPRESSION  = 0x20  ///<
        };

        /**
         * @brief
         */
        struct Symbol
        {
            /**
             * @brief
             * @param[in] value
             * @param[in] type
             */
            Symbol ( const CompilerExpr * value = NULL,
                     const CompilerBase::SourceLocation * location = NULL,
                     SymbolType type = STYPE_UNSPECIFIED,
                     int finalValue = -1,
                     bool redefinable = false );

            /**
             * @brief
             */
            ~Symbol();

            ///
            CompilerExpr * m_value;

            ///
            CompilerBase::SourceLocation m_location;

            ///
            SymbolType m_type;

            ///
            int m_finalValue;

            ///
            bool m_used;

            ///
            bool m_constant;

            ///
            bool m_masked;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @return
         */
        AsmKcpsm3SymbolTable ( CompilerSemanticInterface * compilerCore,
                               CompilerOptions * opts )
                             : m_compilerCore ( compilerCore ),
                               m_opts ( opts ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] name
         * @param[in] value
         * @param[in] location
         * @param[in] type
         * @param[in] resolve
         * @param[in] redefinable
         * @return
         */
        int addSymbol ( const std::string & name,
                        const CompilerExpr * value = NULL,
                        const CompilerBase::SourceLocation * location = NULL,
                        const SymbolType type = STYPE_UNSPECIFIED,
                        bool resolve = false,
                        bool redefinable = false );

        /**
         * @brief
         * @param[in,out] expr
         * @param[in] codePointer
         */
        void resolveSymbols ( CompilerExpr * expr,
                              int codePointer );

        /**
         * @brief
         * @param[in] expr
         * @return
         */
        SymbolType getType ( const CompilerExpr * expr );

        /**
         * @brief
         * @param[in] name
         * @return
         */
        SymbolType getType ( const std::string & name );

        /**
         * @brief
         */
        void maskNonLabels();

        /**
         * @brief
         * @param[in] name
         * @param[in] value
         * @param[in] location
         * @param[in] type
         * @param[in] resolve
         * @return
         */
        int assignValue ( const std::string & name,
                          const CompilerExpr * value,
                          const CompilerBase::SourceLocation * location = NULL,
                          const SymbolType type = STYPE_UNSPECIFIED,
                          bool resolve = false );

        /**
         * @brief
         * @param[in] name
         * @param[in] location
         * @param[in] type
         */
        void removeSymbol ( const std::string & name,
                            const CompilerBase::SourceLocation & location,
                            const SymbolType type = STYPE_UNSPECIFIED );

        /**
         * @brief
         * @param[in] name
         * @param[in] type
         * @return
         */
        bool isDefined ( const std::string & name,
                         const SymbolType type = STYPE_UNSPECIFIED ) const;

        /**
         * @brief
         * @param[in] name
         * @param[in] type
         * @return
         */
        const CompilerExpr * getValue ( const std::string & name,
                                        const SymbolType type = STYPE_UNSPECIFIED );

        /**
         * @brief
         * @param[in] expr
         * @param[in] bitsMax
         * @param[in] origLocation
         * @return
         */
        unsigned int resolveExpr ( const CompilerExpr * expr,
                                   int bitsMax = -1,
                                   const CompilerBase::SourceLocation * origLocation = NULL );

        /**
         * @brief
         * @param[in] origSymbol
         * @param[in] newSymbol
         * @param[in,out] expr
         * @return
         */
        unsigned int substitute ( const std::string & origSymbol,
                                  const CompilerExpr * newSymbol,
                                  CompilerExpr * expr );

        /**
         * @brief
         */
        void output();

        /**
         * @brief
         */
        void clear();

        /**
         * @brief
         * @param[in,out] out
         * @param[in] location
         */
        void printSymLocation ( std::ostream & out,
                                const CompilerBase::SourceLocation & location ) const;

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] side
         * @param[in] expr
         * @return
         */
        int getExprValue ( ExprValSide side,
                           const CompilerExpr * expr );

        /**
         * @brief
         * @param[in] expr
         * @return
         */
        int computeExpr ( const CompilerExpr * expr );

    ////    Private Attributes    ////
    private:
        ///
        std::multimap<std::string,Symbol> m_table;

        ///
        std::multimap<std::string,Symbol> m_deletedSymbols;

        ///
        CompilerSemanticInterface * const m_compilerCore;

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
