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
 * @file AsmKcpsm3InstructionSet.h
 */
// =============================================================================

#ifndef ASMKCPSM3INSTRUCTIONSET_H
#define ASMKCPSM3INSTRUCTIONSET_H

// Forward declarations
class CompilerStatement;
class CompilerOptions;
class AsmKcpsm3SymbolTable;

// Common compiler header files.
#include "../../CompilerSemanticInterface.h"

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3InstructionSet
 */
class AsmKcpsm3InstructionSet
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum OPER_BIT_SIZE
        {
            OBS_AAA = 10, ///< Address
            OBS_SXY =  4, ///< Register
            OBS_KK  =  8, ///< Constant
            OBS_PP  =  8, ///< Port ID
            OBS_SS  =  6  ///< Scratch Pad RAM
        };

        /**
         * @brief
         */
        enum LimitType
        {
            LIM_C, ///<
            LIM_R, ///<
            LIM_D  ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @return
         */
        AsmKcpsm3InstructionSet ( CompilerSemanticInterface * compilerCore,
                                  CompilerOptions * opts,
                                  AsmKcpsm3SymbolTable * symbolTable )
                                : m_compilerCore ( compilerCore ),
                                  m_opts ( opts ),
                                  m_symbolTable ( symbolTable ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] stmt
         * @param[in,out] symbolTable
         * @return
         */
        int resolveOPcode ( CompilerStatement * stmt ) const;

        /**
         * @brief
         * @param[in,out] stmt
         * @param[in] codePointer
         */
        void encapsulate ( CompilerStatement * stmt,
                           int codePointer ) const;

        /**
         * @brief
         * @param[in] stmt
         * @return
         */
        bool isInstruction ( const CompilerStatement * const stmt ) const;

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] type
         * @param[in] location
         * @param[in] value
         * @return
         */
        unsigned int checkLimit ( LimitType type,
                                  const CompilerBase::SourceLocation & location,
                                  unsigned int value ) const;

        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getAAA ( const CompilerStatement * stmt,
                              int index ) const;

        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getSXY ( const CompilerStatement * stmt,
                              int index ) const;

        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getKK ( const CompilerStatement * stmt,
                             int index ) const;

        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getPP ( const CompilerStatement * stmt,
                             int index ) const;

        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getSS ( const CompilerStatement * stmt,
                             int index ) const;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] stmt
         * @param[out] acceptableTypes
         */
        inline void detAccSymTypes ( const CompilerStatement * stmt,
                                     int * acceptableTypes ) const;

        /**
         * @brief
         * @param[in] stmt
         * @return
         */
        inline std::string getInstructionName ( const CompilerStatement * stmt ) const;

        /**
         * @brief
         * @param[in] types
         * @return
         */
        inline std::string getSymbolTypes ( int types ) const;

    ////    Private Attributes    ////
    private:
        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;

        ///
        AsmKcpsm3SymbolTable * const m_symbolTable;
};

#endif // ASMKCPSM3INSTRUCTIONSET_H
