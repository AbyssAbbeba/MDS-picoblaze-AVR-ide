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
 * @file AsmPicoBlazeSpecialMacros.h
 */
// =============================================================================

#ifndef ASMPICOBLAZESPECIALMACROS_H
#define ASMPICOBLAZESPECIALMACROS_H

// Forward declarations.
class CompilerExpr;
class CompilerStatement;
class CompilerSourceLocation;
class AsmCodeListing;
class CompilerSemanticInterface;

// PicoBlaze assembler semantic analyzer header files.
#include "AsmSymbolTable.h"

// Standard headers.
#include <string>
#include <vector>

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeSpecialMacros
 */
class AsmPicoBlazeSpecialMacros
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum JumpCondition
        {
            JC_NONE = 0, ///<
            JC_Z,        ///<
            JC_C,        ///<
            JC_NZ,       ///<
            JC_NC        ///<
        };

        /**
         * @brief
         */
        enum LabelType
        {
            LT_IF = 0, ///<
            LT_FOR,    ///<
            LT_WHILE   ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in,out] symbolTable
         * @param[in,out] codeListing
         * @return
         */
        AsmPicoBlazeSpecialMacros ( CompilerSemanticInterface * compilerCore,
                                    AsmSymbolTable            * symbolTable,
                                    AsmCodeListing            * codeListing );

        /**
         * @brief
         */
        ~AsmPicoBlazeSpecialMacros();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] rtWhile
         * @return
         */
        CompilerStatement * runTimeWhile ( CompilerStatement * rtWhile );

        /**
         * @brief
         * @param[in,out] rtFor
         * @return
         */
        CompilerStatement * runTimeFor ( CompilerStatement * rtFor );

        /**
         * @brief
         */
        void runTimeForLeave();

        /**
         * @brief
         * @param[in,out] rtIfTree
         * @return
         */
        CompilerStatement * runTimeCondition ( CompilerStatement * rtIfTree );

        /**
         * @brief
         * @param[in] node
         * @return
         */
        bool isFromSpecMacro ( const CompilerStatement * node ) const;

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] cnd
         * @param[in] label
         * @param[in] location
         * @return
         */
        CompilerStatement * evaluateCondition ( const CompilerExpr * cnd,
                                                const std::string & label,
                                                const CompilerSourceLocation & location );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[out] label
         * @param[in] labelType
         * @param[in] end
         * @return
         */
        inline const std::string & generateLabel ( std::string & label,
                                                   LabelType labelType,
                                                   bool end = false );

        /**
         * @brief
         * @param[in] label
         * @param[in] location
         * @param[in] cnd
         * @return
         */
        inline CompilerStatement * jump ( const std::string & label,
                                          const CompilerSourceLocation & location,
                                          JumpCondition cnd = JC_NONE ) const;

        /**
         * @brief
         * @param[in] label
         * @param[in] location
         * @return
         */
        inline CompilerStatement * label ( const std::string & label,
                                           const CompilerSourceLocation & location ) const;

        /**
         * @brief
         * @param[in] sx
         * @param[in] sy
         * @param[in] location
         * @return
         */
        inline CompilerStatement * compare_sx_sy ( const CompilerExpr * sx,
                                                   const CompilerExpr * sy,
                                                   const CompilerSourceLocation & location ) const;

        /**
         * @brief
         * @param[in] sx
         * @param[in] kk
         * @param[in] location
         * @return
         */
        inline CompilerStatement * compare_sx_kk ( const CompilerExpr * sx,
                                                   int kk,
                                                   const CompilerSourceLocation & location ) const;

        /**
         * @brief
         * @param[in] sx
         * @param[in] sy
         * @param[in] location
         * @return
         */
        inline CompilerStatement * test_sx_sy ( const CompilerExpr * sx,
                                                const CompilerExpr * sy,
                                                const CompilerSourceLocation & location ) const;

        /**
         * @brief
         * @param[in] sx
         * @param[in] kk
         * @param[in] location
         * @return
         */
        inline CompilerStatement * test_sx_kk ( const CompilerExpr * sx,
                                                int kk,
                                                const CompilerSourceLocation & location ) const;

        /**
         * @brief
         * @param[in] regOrNumber
         * @param[in] type
         * @param[in] location
         */
        inline void checkType ( bool regOrNumber,
                                AsmSymbolTable::SymbolType type,
                                const CompilerSourceLocation & location );

        /**
         * @brief
         * @param[in,out] tree
         * @return
         */
        inline CompilerStatement * markAsFromSpecMacro ( CompilerStatement * tree );

    ////    Private Attributes    ////
    private:
        ///
        unsigned int m_labelNextCounter[3];

        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        AsmSymbolTable * const m_symbolTable;

        ///
        AsmCodeListing * const m_codeListing;

        ///
        std::vector<int> m_forLoopIterRegs;
};

#endif // ASMPICOBLAZESPECIALMACROS_H
