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
        /// @brief
        static const unsigned int MAX_WHILE_ITERATIONS = 10000;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct MemoryPtr
        {
            /// @brief Sets all pointers to zero.
            void clear();

            int m_code; ///< Program memory (CODE).
            int m_reg;  ///< Processor registers (REG).
            int m_data; /// Scratch Pad RAM (DATA).
        };

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
         */
        void phase1 ( CompilerStatement * codeTree,
                      const CompilerBase::SourceLocation * origLocation = NULL,
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
        MemoryPtr m_memoryPtr;
};

#endif // ASMKCPSM3SEMANTICANALYSER_H
