// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3Macros.h
 */
// =============================================================================

#ifndef ASMKCPSM3MACROS_H
#define ASMKCPSM3MACROS_H

// Forward declarations.
class AsmKcpsm3SymbolTable;
class AsmKcpsm3CodeListing;

// Common compiler header files.
#include "../../CompilerSemanticInterface.h"
#include "../../CompilerOptions.h"

// Standard headers.
#include <string>
#include <vector>
#include <ostream>
#include <map>

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3Macros
 */
class AsmKcpsm3Macros
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AsmKcpsm3Macros * macros );

    ////    Public Static Constants    ////
    public:
        /// @brief
        static const char MANGLE_PREFIX = ':';

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Macro
        {
            /**
             * @brief
             */
            Macro();

            /**
             * @brief
             * @param[in] location
             * @param[in] id
             */
            Macro ( const CompilerBase::SourceLocation location,
                    int id )
                  : m_location ( location ),
                    m_id ( id ),
                    m_usageCounter ( 0 ) {}

            ///
            std::vector<std::string> m_parameters;

            ///
            CompilerStatement * m_definition;

            ///
            CompilerBase::SourceLocation m_location;

            /// Macro identifier.
            int m_id;

            ///
            unsigned int m_usageCounter;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @return
         */
        AsmKcpsm3Macros ( CompilerSemanticInterface * compilerCore,
                          CompilerOptions * opts,
                          AsmKcpsm3SymbolTable * symbolTable,
                          AsmKcpsm3CodeListing * codeListing );

        /**
         * @brief
         */
        ~AsmKcpsm3Macros();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @param[in] name
         * @param[in] parameters
         * @param[in,out] macroDef
         */
        void define ( CompilerBase::SourceLocation location,
                      const std::string & name,
                      const CompilerExpr * parameters,
                      CompilerStatement * macroDef );

        /**
         * @brief 
         * @param[in] msgLocation
         * @param[in] location
         * @param[in] name
         * @param[in] arguments
         * @return
         */
        CompilerStatement * expand ( const CompilerBase::SourceLocation & msgLocation,
                                     const CompilerBase::SourceLocation & location,
                                     const std::string & name,
                                     const CompilerExpr * arguments );

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
         * @param[in] enabled
         */
        void setExpEnabled ( bool enabled );

        /**
         * @brief
         * @param[in,out] out
         * @param[in] location
         */
        void printSymLocation ( std::ostream & out,
                                const CompilerBase::SourceLocation & location ) const;

        /**
         * @brief
         * @param[in] node
         * @return
         */
        bool isFromMacro ( const CompilerStatement * node ) const;

        /**
         * @brief
         * @param[in] location
         * @param[in,out] localSymbols
         * @param[in] local
         * @param[in] macroName
         * @param[in,out] node
         * @return
         */
        bool mangleName ( const CompilerBase::SourceLocation & location,
                          std::vector<std::string> * localSymbols,
                          const std::string & local,
                          const std::string & macroName,
                          CompilerStatement * node );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] parameter
         * @param[in] argument
         * @param[in,out] target
         * @return
         */
        bool symbolSubst ( const std::string & parameter,
                           const CompilerExpr * argument,
                           CompilerStatement * target );

        /**
         * @brief
         * @param[in,out] macro
         */
        void incrMacroCounter ( CompilerStatement * macro ) const;

    ////    Private Attributes    ////
    private:
        ///
        unsigned int m_idCounter;

        ///
        unsigned int m_expCounter;

        ///
        std::map<std::string,Macro> m_table;

        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;

        ///
        AsmKcpsm3SymbolTable * const m_symbolTable;

        ///
        AsmKcpsm3CodeListing * const m_codeListing;

        ///
        bool m_expEnabled;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] macros
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const AsmKcpsm3Macros * macros );
//@}

#endif // ASMKCPSM3MACROS_H
