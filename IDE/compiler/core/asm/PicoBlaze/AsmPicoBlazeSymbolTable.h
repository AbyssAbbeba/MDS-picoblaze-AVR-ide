// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeSymbolTable.h
 */
// =============================================================================

#ifndef ASMPICOBLAZESYMBOLTABLE_H
#define ASMPICOBLAZESYMBOLTABLE_H

// Common compiler header files.
#include "../../CompilerSemanticInterface.h"
#include "../../CompilerOptions.h"

// Standard headers.
#include <map>
#include <string>
#include <ostream>

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeSymbolTable
 */
class AsmPicoBlazeSymbolTable
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AsmPicoBlazeSymbolTable * symbolTable );

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
                     const CompilerSourceLocation * location = NULL,
                     SymbolType type = STYPE_UNSPECIFIED,
                     int finalValue = -1,
                     bool redefinable = false );

            /**
             * @brief
             * @param[in] obj
             */
            Symbol ( const Symbol & obj );

            ///
            CompilerExpr * m_value;

            ///
            CompilerSourceLocation m_location;

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
        AsmPicoBlazeSymbolTable ( CompilerSemanticInterface * compilerCore,
                                  CompilerOptions * opts )
                                : m_compilerCore ( compilerCore ),
                                  m_opts ( opts ) {};

        /**
         * @brief
         */
        ~AsmPicoBlazeSymbolTable();

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
                        const CompilerSourceLocation * location = NULL,
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
                          const CompilerSourceLocation * location = NULL,
                          const SymbolType type = STYPE_UNSPECIFIED,
                          bool resolve = true );

        /**
         * @brief
         * @param[in] name
         * @param[in] location
         * @param[in] type
         */
        void removeSymbol ( const std::string & name,
                            const CompilerSourceLocation & location,
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
                                   int bitsMax = 18,
                                   const CompilerSourceLocation * origLocation = NULL );

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
                                const CompilerSourceLocation & location ) const;

        /**
         * @brief
         * @param[in] symbolType
         * @param[in] constLength
         * @return
         */
         static const char * symType2Str ( const SymbolType symbolType,
                                           bool constLength = false );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] side
         * @param[in] expr
         * @param[in] argList
         * @return
         */
        int getExprValue ( ExprValSide side,
                           const CompilerExpr * expr,
                           const CompilerExpr * argList = NULL );

        /**
         * @brief
         * @param[in] expr
         * @param[in] argList
         * @return
         */
        int computeExpr ( const CompilerExpr * expr,
                          const CompilerExpr * argList = NULL );

        /**
         * @brief
         * @param[in,out] expr
         * @param[in] subst
         * @param[in] position
         */
        void substArg ( CompilerExpr * expr,
                        const CompilerExpr * subst,
                        const int position );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] expr
         * @param[in] argList
         * @return
         */
        inline CompilerExpr * substituteArgs ( const CompilerExpr * expr,
                                               const CompilerExpr * argList );

    ////    Private Attributes    ////
    private:
        ///
        std::map<std::string,Symbol> m_table;

        ///
        std::map<std::string,Symbol> m_deletedSymbols;

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
                                 const AsmPicoBlazeSymbolTable * symbolTable );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] symbolType
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const AsmPicoBlazeSymbolTable::SymbolType symbolType );
//@}

#endif // ASMPICOBLAZESYMBOLTABLE_H
