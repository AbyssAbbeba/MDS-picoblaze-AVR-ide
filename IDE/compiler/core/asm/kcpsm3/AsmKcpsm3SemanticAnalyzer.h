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
 * @file AsmKcpsm3SemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMKCPSM3SEMANTICANALYSER_H
#define ASMKCPSM3SEMANTICANALYSER_H

// Forward declarations.
class AsmMachineCodeGen;
class AsmDgbFileGen;
class AsmKcpsm3SymbolTable;
class AsmKcpsm3CodeListing;
class AsmKcpsm3InstructionSet;
class AsmKcpsm3Macros;
class AsmKcpsm3MemoryPtr;
class AsmKcpsm3SpecialMacros;

// Common compiler header files.
#include "../../CompilerSemanticAnalyzer.h"

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3SemanticAnalyzer
 */
class AsmKcpsm3SemanticAnalyzer : public CompilerSemanticAnalyzer
{
    ////    Public Static Constants    ////
    public:
        /// @brief Maximum allowed number of iterations in a WHILE loop.
        static const unsigned int MAX_WHILE_ITERATIONS = 1024;

        /// @brief Maximum allowed number of iterations in a REPEAT loop.
        static const unsigned int MAX_REPEAT_ITERATIONS = 1024;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        AsmKcpsm3SemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                    CompilerOptions * opts );

        /**
         * @brief
         */
        virtual ~AsmKcpsm3SemanticAnalyzer();

    protected:
        /**
         * @brief Forbidden constructor
         */
        AsmKcpsm3SemanticAnalyzer() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        void process ( CompilerStatement * codeTree );


    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] codeTree
         */
        void printCodeTree ( const CompilerStatement * codeTree );

        /**
         * @brief
         * @param[in,out] codeTree
         * @param[in] origLocation
         * @param[in] macroName
         * @return False, if critical error occured; true otherwise.
         */
        bool phase1 ( CompilerStatement * codeTree,
                      const CompilerSourceLocation * origLocation = NULL,
                      const std::string * macroName = NULL );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        inline void phase2 ( CompilerStatement * codeTree );

        /**
         * @brief
         * @param[in,out] ifTree
         * @return
         */
        inline CompilerStatement * conditionalCompilation ( CompilerStatement * ifTree );

    ////    Protected Attributes    ////
    protected:
        ///
        AsmMachineCodeGen * m_machineCode;

        ///
        AsmDgbFileGen * m_dgbFile;

        ///
        AsmKcpsm3SymbolTable * m_symbolTable;

        ///
        AsmKcpsm3CodeListing * m_codeListing;

        ///
        AsmKcpsm3InstructionSet * m_instructionSet;

        ///
        AsmKcpsm3Macros * m_macros;

        ///
        AsmKcpsm3MemoryPtr * m_memoryPtr;

        ///
        AsmKcpsm3SpecialMacros * m_specialMacros;
};

#endif // ASMKCPSM3SEMANTICANALYSER_H
