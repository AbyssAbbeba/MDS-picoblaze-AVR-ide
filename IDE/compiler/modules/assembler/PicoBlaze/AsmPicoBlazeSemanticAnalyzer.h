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
class AsmDgbFileGen;
class AsmPicoBlazeMacros;
class AsmPicoBlazeMemoryPtr;
class AsmPicoBlazeSymbolTable;
class AsmPicoBlazeTreeDecoder;
class AsmPicoBlazeCodeListing;
class AsmPicoBlazeSpecialMacros;
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

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] codeTree
         */
        void printCodeTree ( const CompilerStatement * codeTree );

    ////    Inline Private Operations    ////
    private:
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
        AsmMachineCodeGen * m_machineCode;

        ///
        AsmDgbFileGen * m_dgbFile;

        ///
        AsmPicoBlazeSymbolTable * m_symbolTable;

        ///
        AsmPicoBlazeCodeListing * m_codeListing;

        ///
        AsmPicoBlazeInstructionSet * m_instructionSet;

        ///
        AsmPicoBlazeMacros * m_macros;

        ///
        AsmPicoBlazeMemoryPtr * m_memoryPtr;

        ///
        AsmPicoBlazeSpecialMacros * m_specialMacros;

        ///
        AsmPicoBlazeTreeDecoder * m_treeDecoder;

        ///
        Device m_device;
};

#endif // ASMPICOBLAZESEMANTICANALYSER_H
