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
 * @file AsmPicoBlazeTreeDecoder.h
 */
// =============================================================================

#ifndef ASMPICOBLAZETREEDECODER_H
#define ASMPICOBLAZETREEDECODER_H

// Forward declarations (Compiler common).
class CompilerOptions;
class CompilerStatement;
class CompilerSemanticInterface;

// Forward declarations (PicoBlaze Assembler common).
class AsmMacros;
class AsmMemoryPtr;
class AsmDgbFileGen;
class AsmSymbolTable;
class AsmCodeListing;
class AsmMachineCodeGen;
class AsmPicoBlazeSpecialMacros;
class AsmPicoBlazeInstructionSet;

// PicoBlaze assembler semantic analyzer header files.
#include "CompilerSourceLocation.h"
#include "AsmPicoBlazeSemanticAnalyzer.h"

// Standard headers.
#include <string>

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeTreeDecoder
 */
class AsmPicoBlazeTreeDecoder
{
    ////    Private Static Constants    ////
    private:
        /// @brief Maximum allowed number of iterations in a WHILE loop.
        static const unsigned int MAX_WHILE_ITERATIONS = 1024;

        /// @brief Maximum allowed number of iterations in a REPEAT loop.
        static const unsigned int MAX_REPEAT_ITERATIONS = 1024;

    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        struct SprInit
        {
            ///
            CompilerSourceLocation m_location;

            ///
            unsigned int m_address;

            ///
            unsigned char m_octet;
        };

    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum CourseOfAction
        {
            CA_NO_ACTION,
            CA_CONTINUE,
            CA_RETURN_TRUE,
            CA_RETURN_FALSE
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in,out] opts
         * @param[in,out] dgbFile
         * @param[in,out] machineCode
         * @param[in,out] macros
         * @param[in,out] memoryPtr
         * @param[in,out] symbolTable
         * @param[in,out] codeListing
         * @param[in,out] specialMacros
         * @param[in,out] instructionSet
         * @param[in,out] device
         * @return
         */
        AsmPicoBlazeTreeDecoder ( CompilerSemanticInterface            * compilerCore,
                                  CompilerOptions                      * opts,
                                  AsmDgbFileGen                        * dgbFile,
                                  AsmMachineCodeGen                    * machineCode,
                                  AsmMacros                            * macros,
                                  AsmMemoryPtr                         * memoryPtr,
                                  AsmSymbolTable                       * symbolTable,
                                  AsmCodeListing                       * codeListing,
                                  AsmPicoBlazeSpecialMacros            * specialMacros,
                                  AsmPicoBlazeInstructionSet           * instructionSet,
                                  AsmPicoBlazeSemanticAnalyzer::Device & device );

        /**
         * @brief
         */
        ~AsmPicoBlazeTreeDecoder();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         * @param[in] macroName
         * @return False, if critical error occured; true otherwise.
         */
        bool phase1 ( CompilerStatement * codeTree,
                      const std::string * macroName = nullptr );

        /**
         * @brief
         * @param[in,out] codeTree
         */
        void phase2 ( CompilerStatement * codeTree );

        /**
         * @brief
         */
        void phase3();

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] location
         * @param[in] functionality
         * @return
         */
        bool checkKcpsm3AndHigher ( const CompilerSourceLocation & location,
                                    const char * functionality );

        /**
         * @brief
         * @param[in] expr
         * @return
         */
        bool isBlank ( const CompilerExpr * expr ) const;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_LIST ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_NOLIST ( CompilerStatement * node );

        /**
         * @brief
         */
        inline void dir_EXPAND();

        /**
         * @brief
         */
        inline void dir_NOEXPAND();

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_IF ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_AUTOxxx ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_DB ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_MACRO ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_TITLE ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_MESSG ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_WARNING ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_ERROR ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_INCLUDE ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         * @param[in] macroName
         * @param[in,out] localSymbols
         */
        inline void dir_LOCAL ( CompilerStatement * node,
                                const std::string * macroName,
                                std::vector<std::string> & localSymbols );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_SET ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_DEFINE ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_UNDEFINE ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void label ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_ORG ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_SKIP ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_LIMIT ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_FAILJMP ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_ORGSPR ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_INITSPR ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_MERGESPR ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         */
        inline void dir_EQU_etc ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         * @return
         */
        inline CourseOfAction dir_EXITM ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         * @return
         */
        inline CourseOfAction dir_REPT ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         * @return
         */
        inline CourseOfAction dir_WHILE ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         * @return
         */
        inline CourseOfAction macro ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         * @return
         */
        inline CourseOfAction dir_RTCOND ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         * @return
         */
        inline CourseOfAction dir_RTWHILE ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         * @return
         */
        inline CourseOfAction dir_RTFOR ( CompilerStatement * node );

        /**
         * @brief
         * @param[in,out] node
         * @return
         */
        inline CourseOfAction dir_DEVICE ( CompilerStatement * node );

        /**
         * @brief
         * @return
         */
        inline bool isInstWord3B() const;

    ////    Private Attributes    ////
    private:
        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;

        ///
        AsmDgbFileGen * const m_dgbFile;

        ///
        AsmMachineCodeGen * const m_machineCode;

        ///
        AsmMacros * const m_macros;

        ///
        AsmMemoryPtr * const m_memoryPtr;

        ///
        AsmSymbolTable * const m_symbolTable;

        ///
        AsmCodeListing * const m_codeListing;

        ///
        AsmPicoBlazeSpecialMacros * const m_specialMacros;

        ///
        AsmPicoBlazeInstructionSet * const m_instructionSet;

        ///
        AsmPicoBlazeSemanticAnalyzer::Device & m_device;

        ///
        CompilerStatement * m_forceNext;

        ///
        CompilerStatement * m_failjmp;

        ///
        std::vector<SprInit> m_sprInit;

        ///
        int m_mergeAddr;

        ///
        CompilerSourceLocation m_mergeSprLoc;
};

#endif // ASMPICOBLAZETREEDECODER_H
