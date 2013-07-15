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
 * @file AsmKcpsm3SpecialMacros.h
 */
// =============================================================================

#ifndef ASMKCPSM3SPECIALMACROS_H
#define ASMKCPSM3SPECIALMACROS_H

// Forward declarations.
class CompilerSemanticInterface;
class AsmKcpsm3SymbolTable;
class AsmKcpsm3CodeListing;
class CompilerStatement;
class CompilerExpr;

// Standard headers.
#include <string>

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3SpecialMacros
 */
class AsmKcpsm3SpecialMacros
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in,out] symbolTable
         * @param[in,out] codeListing
         * @return
         */
        AsmKcpsm3SpecialMacros ( CompilerSemanticInterface * compilerCore,
                                 AsmKcpsm3SymbolTable * symbolTable,
                                 AsmKcpsm3CodeListing * codeListing );

        /**
         * @brief
         */
        ~AsmKcpsm3SpecialMacros();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] rtIfTree
         * @return
         */
        CompilerStatement * runTimeCondition ( CompilerStatement * rtIfTree );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] cnd
         * @param[in] label
         * @return
         */
        CompilerStatement * evaluateCondition ( const CompilerExpr * cnd,
                                                const std::string & label );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[out] label
         * @param[in] end
         * @return
         */
        inline const std::string & generateLabel ( std::string & label,
                                                   bool end = false );

        /**
         * @brief
         * @param[in] label
         * @return
         */
        inline CompilerStatement * jump ( const std::string & label );

        /**
         * @brief
         * @param[in] label
         * @return
         */
        inline CompilerStatement * label ( const std::string & label );

    ////    Private Attributes    ////
    private:
        ///
        unsigned int m_labelNextCounter;

        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        AsmKcpsm3SymbolTable * const m_symbolTable;

        ///
        AsmKcpsm3CodeListing * const m_codeListing;
};

#endif // ASMKCPSM3SPECIALMACROS_H
