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
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeSemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMPICOBLAZESEMANTICANALYSER_H
#define ASMPICOBLAZESEMANTICANALYSER_H

// Forward declarations.
class AsmMachineCodeGen;
class AsmDgbFileGen;
class AsmPicoBlazeSymbolTable;
class AsmPicoBlazeCodeListing;
class AsmPicoBlazeInstructionSet;
class AsmPicoBlazeMacros;
class AsmPicoBlazeMemoryPtr;
class AsmPicoBlazeSpecialMacros;

// Common compiler header files.
#include "../../CompilerSemanticAnalyzer.h"

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeSemanticAnalyzer
 */
class AsmPicoBlazeSemanticAnalyzer : public CompilerSemanticAnalyzer
{
    ////    Public Static Constants    ////
    public:
        /// @brief Maximum allowed number of iterations in a WHILE loop.
        static const unsigned int MAX_WHILE_ITERATIONS = 1024;

        /// @brief Maximum allowed number of iterations in a REPEAT loop.
        static const unsigned int MAX_REPEAT_ITERATIONS = 1024;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Device
        {
            DEV_UNSPEC, ///<
            DEV_KCPSM2, ///<
            DEV_KCPSM3, ///<
            DEV_KCPSM6, ///<
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
        void process ( CompilerStatement * codeTree );

        /**
         * @brief
         * @param[in] deviceName
         */
        void setDevice ( const std::string & deviceName );

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
         */
        inline void outputHDL();

        /**
         * @brief
         * @param[in,out] dataFile
         * @param[in] fileName
         */
        inline void saveHDL ( DataFile & dataFile,
                              const std::string & fileName );

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
        Device m_device;
};

#endif // ASMPICOBLAZESEMANTICANALYSER_H
