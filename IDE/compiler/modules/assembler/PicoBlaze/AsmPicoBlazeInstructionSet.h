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
 * @file AsmPicoBlazeInstructionSet.h
 */
// =============================================================================

#ifndef ASMPICOBLAZEINSTRUCTIONSET_H
#define ASMPICOBLAZEINSTRUCTIONSET_H

// Forward declarations
class CompilerStatement;
class CompilerOptions;
class AsmPicoBlazeSymbolTable;
class AsmPicoBlazeInstructionSetN;

// Common compiler header files.
#include "CompilerSemanticInterface.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeSemanticAnalyzer.h"

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeInstructionSet
 */
class AsmPicoBlazeInstructionSet
{
    ////    Public Datatypes    ////
    public:
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
        AsmPicoBlazeInstructionSet ( CompilerSemanticInterface * compilerCore,
                                     CompilerOptions * opts,
                                     AsmPicoBlazeSymbolTable * symbolTable,
                                     AsmPicoBlazeSemanticAnalyzer::Device * device )
                                   : m_compilerCore ( compilerCore ),
                                     m_opts ( opts ),
                                     m_symbolTable ( symbolTable ),
                                     m_strategy ( NULL ),
                                     m_device ( device ) {};

        /**
         * @brief
         */
        ~AsmPicoBlazeInstructionSet();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] stmt
         * @return
         */
        int resolveOPcode ( const CompilerStatement * stmt );

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

        /**
         * @brief
         * @param[in,out] strategy
         */
        void setStrategy ( AsmPicoBlazeInstructionSetN * strategy );

        /**
         * @brief
         * @param[in] type
         * @param[in] location
         * @param[in] value
         * @return
         */
        unsigned int checkLimit ( LimitType type,
                                  const CompilerSourceLocation & location,
                                  unsigned int value ) const;

        /**
         * @brief
         * @param[in] stmt
         * @return
         */
        std::string getInstructionName ( const CompilerStatement * stmt ) const;

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
         * @param[in] types
         * @return
         */
        inline std::string getSymbolTypes ( int types ) const;

        /**
         * @brief
         * @param[in] stmt
         * @return
         */
        inline int oprIdxShift ( const CompilerStatement * stmt ) const;

    ////    Private Attributes    ////
    private:
        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;

        ///
        AsmPicoBlazeSymbolTable * const m_symbolTable;

        ///
        AsmPicoBlazeInstructionSetN * m_strategy;

        ///
        AsmPicoBlazeSemanticAnalyzer::Device * const m_device;
};

#endif // ASMPICOBLAZEINSTRUCTIONSET_H
