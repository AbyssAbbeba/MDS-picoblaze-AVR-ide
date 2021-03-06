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
 * @file AsmPicoBlazeSemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMPICOBLAZESEMANTICANALYSER_H
#define ASMPICOBLAZESEMANTICANALYSER_H

// Forward declarations.
class AsmMacros;
class AsmMemoryPtr;
class AsmDgbFileGen;
class AsmStringTable;
class AsmSymbolTable;
class AsmCodeListing;
class AsmPicoBlazeTreeDecoder;
class AsmPicoBlazeSpecialMacros;
class AsmPicoBlazeCodeGenerator;
class AsmPicoBlazeInstructionSet;

// Common compiler header files.
#include "CompilerSemanticAnalyzer.h"
#include "AsmMachineCodeGen.h"

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeSemanticAnalyzer
 */
class AsmPicoBlazeSemanticAnalyzer : public CompilerSemanticAnalyzer
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Device
        {
            DEV_UNSPEC,      ///<
            DEV_KCPSM1CPLD,  ///<
            DEV_KCPSM1,      ///<
            DEV_KCPSM2,      ///<
            DEV_KCPSM3,      ///<
            DEV_KCPSM6,      ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        AsmPicoBlazeSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                       CompilerOptions * opts );

        /**
         * @brief
         */
        virtual ~AsmPicoBlazeSemanticAnalyzer();

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

        /**
         * @brief
         */
        inline void genSummary();

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
        AsmPicoBlazeTreeDecoder * m_treeDecoder;

        ///
        AsmPicoBlazeSpecialMacros * m_specialMacros;

        ///
        AsmPicoBlazeCodeGenerator * m_codeGenerator;

        ///
        AsmPicoBlazeInstructionSet * m_instructionSet;

        ///
        AsmStringTable * m_stringTable;

        ///
        Device m_device;
};

#endif // ASMPICOBLAZESEMANTICANALYSER_H
