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
 * @file AsmPicoBlazeMacros.h
 */
// =============================================================================

#ifndef ASMPICOBLAZEMACROS_H
#define ASMPICOBLAZEMACROS_H

// Forward declarations.
class AsmPicoBlazeSymbolTable;
class AsmPicoBlazeCodeListing;

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
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeMacros
 */
class AsmPicoBlazeMacros
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AsmPicoBlazeMacros * macros );

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
            Macro ( const CompilerSourceLocation location,
                    int id,
                    CompilerStatement * definition )
                  : m_location ( location ),
                    m_id ( id ),
                    m_definition ( definition ),
                    m_usageCounter ( 0 ) {}

            ///
            CompilerSourceLocation m_location;

            /// Macro identifier.
            int m_id;

            ///
            CompilerStatement * m_definition;

            ///
            unsigned int m_usageCounter;

            ///
            std::vector<std::string> m_parameters;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @return
         */
        AsmPicoBlazeMacros ( CompilerSemanticInterface * compilerCore,
                             CompilerOptions * opts,
                             AsmPicoBlazeSymbolTable * symbolTable,
                             AsmPicoBlazeCodeListing * codeListing );

        /**
         * @brief
         */
        ~AsmPicoBlazeMacros();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @param[in] name
         * @param[in] parameters
         * @param[in,out] macroDef
         */
        void define ( CompilerSourceLocation location,
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
        CompilerStatement * expand ( const CompilerSourceLocation & msgLocation,
                                     const CompilerSourceLocation & location,
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
                                const CompilerSourceLocation & location ) const;

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
        bool mangleName ( const CompilerSourceLocation & location,
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
        AsmPicoBlazeSymbolTable * const m_symbolTable;

        ///
        AsmPicoBlazeCodeListing * const m_codeListing;

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
                                 const AsmPicoBlazeMacros * macros );
//@}

#endif // ASMPICOBLAZEMACROS_H
