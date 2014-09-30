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
 * @ingroup AdaptableAsm
 * @file AsmAdaptableSemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMADAPTABLESEMANTICANALYZER_H
#define ASMADAPTABLESEMANTICANALYZER_H

// Forward declarations.
class AsmMacros;
class AsmMemoryPtr;
class AdjSimProcDef;
class AsmDgbFileGen;
class AsmSymbolTable;
class AsmCodeListing;
class AsmMachineCodeGen;
class AsmAdaptableTreeDecoder;
class AsmAdaptableCodeGenerator;

// Common compiler header files.
#include "CompilerSemanticAnalyzer.h"
#include "AsmMachineCodeGen.h"

/**
 * @brief
 * @ingroup AdaptableAsm
 * @class AsmAdaptableSemanticAnalyzer
 */
class AsmAdaptableSemanticAnalyzer : public CompilerSemanticAnalyzer
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        AsmAdaptableSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                       CompilerOptions * opts );

        /**
         * @brief
         */
        virtual ~AsmAdaptableSemanticAnalyzer();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        virtual void process ( CompilerStatement * codeTree ) override;

        /**
         * @brief
         * @param[in] deviceName
         */
        virtual void setDevice ( const std::string & deviceName ) override;

    ////    Protected Attributes    ////
    protected:
        ///
        AsmMacros * m_macros;

        ///
        AsmDgbFileGen * m_dgbFile;

        ///
        AsmMemoryPtr * m_memoryPtr;

        ///
        AsmSymbolTable * m_symbolTable;

        ///
        AsmCodeListing * m_codeListing;

        ///
        AsmMachineCodeGen * m_machineCode;

        ///
        AsmAdaptableTreeDecoder * m_treeDecoder;

        ///
        AsmAdaptableCodeGenerator * m_codeGenerator;

        ///
        AdjSimProcDef * m_device;
};

#endif // ASMADAPTABLESEMANTICANALYZER_H
