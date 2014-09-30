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
 * @file AsmAdaptableTreeDecoder.h
 */
// =============================================================================

#ifndef ASMADAPTABLETREEDECODER_H
#define ASMADAPTABLETREEDECODER_H

// Forward declarations (Compiler common).
class CompilerOptions;
class CompilerStatement;
class CompilerSemanticInterface;

// Forward declarations (Adaptable Assembler common).
class AsmMacros;
class AsmMemoryPtr;
class AsmDgbFileGen;
class AsmSymbolTable;
class AsmCodeListing;
class AsmMachineCodeGen;

// Adaptable assembler semantic analyzer header files.
#include "CompilerSourceLocation.h"
#include "AsmAdaptableSemanticAnalyzer.h"

// Standard headers.
#include <string>

/**
 * @brief
 * @ingroup AdaptableAsm
 * @class AsmAdaptableTreeDecoder
 */
class AsmAdaptableTreeDecoder
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
         * @param[in,out] device
         * @return
         */
        AsmAdaptableTreeDecoder ( CompilerSemanticInterface * compilerCore,
                                  CompilerOptions           * opts,
                                  AsmDgbFileGen             * dgbFile,
                                  AsmMachineCodeGen         * machineCode,
                                  AsmMacros                 * macros,
                                  AsmMemoryPtr              * memoryPtr,
                                  AsmSymbolTable            * symbolTable,
                                  AsmCodeListing            * codeListing,
                                  AdjSimProcDef             * device );

        /**
         * @brief
         */
        ~AsmAdaptableTreeDecoder();
};

#endif // ASMADAPTABLETREEDECODER_H
