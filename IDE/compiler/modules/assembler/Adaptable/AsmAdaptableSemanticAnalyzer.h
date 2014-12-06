// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
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
class AsmDgbFileGen;
class AsmSymbolTable;
class AsmStringTable;
class AsmCodeListing;
class AsmMachineCodeGen;
class AsmAdaptableTreeDecoder;
class AsmAdaptableCodeGenerator;

// Common compiler header files.
#include "CompilerSemanticAnalyzer.h"
#include "AsmMachineCodeGen.h"

// Support for processor definition files used Adaptable Simulator
#include "AdjSimProcDef.h"

/**
 * @brief
 * @ingroup AdaptableAsm
 * @class AsmAdaptableSemanticAnalyzer
 */
class AsmAdaptableSemanticAnalyzer : public CompilerSemanticAnalyzer
{
    friend class AsmAdaptableTreeDecoder;

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
        virtual void setDevice ( const std::string & deviceDefFile ) override;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] codeTree
         */
        inline void printCodeTree ( const CompilerStatement * codeTree );

        /**
         * @brief
         */
        inline void genMachineCode();

        /**
         * @brief
         * @param[in] wordSize
         * @param[in,out] dataFile
         * @param[in] fileName
         */
        inline void saveHDL ( AsmMachineCodeGen::WordSize wordSize,
                              DataFile & dataFile,
                              const std::string & fileName );

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
        AsmStringTable * m_stringTable;

        ///
        AdjSimProcDef m_device;

        ///
        bool m_deviceSet;
};

#endif // ASMADAPTABLESEMANTICANALYZER_H
