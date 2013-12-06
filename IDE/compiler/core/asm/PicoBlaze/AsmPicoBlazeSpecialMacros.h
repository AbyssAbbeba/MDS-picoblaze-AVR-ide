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
class CompilerSemanticInterface;
class CompilerSourceLocation;
class CompilerStatement;
class CompilerExpr;

class AsmPicoBlazeSymbolTable;
class AsmPicoBlazeCodeListing;

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
                                    AsmPicoBlazeSymbolTable   * symbolTable,
                                    AsmPicoBlazeCodeListing   * codeListing );

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

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] cnd
         * @param[in] label
         * @return
         */
        CompilerStatement * evaluateCondition ( const CompilerExpr * cnd,
                                                const std::string & label );

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
         * @param[in] cnd
         * @return
         */
        inline CompilerStatement * jump ( const std::string & label,
                                          JumpCondition cnd = JC_NONE ) const;

        /**
         * @brief
         * @param[in] label
         * @return
         */
        inline CompilerStatement * label ( const std::string & label ) const;

        /**
         * @brief
         * @param[in] sx
         * @param[in] sy
         * @return
         */
        inline CompilerStatement * compare_sx_sy ( int sx,
                                                   int sy ) const;

        /**
         * @brief
         * @param[in] sx
         * @param[in] kk
         * @return
         */
        inline CompilerStatement * compare_sx_kk ( int sx,
                                                   int kk ) const;

        /**
         * @brief
         * @param[in] sx
         * @param[in] sy
         * @return
         */
        inline CompilerStatement * test_sx_sy ( int sx,
                                                int sy ) const;

        /**
         * @brief
         * @param[in] sx
         * @param[in] kk
         * @return
         */
        inline CompilerStatement * test_sx_kk ( int sx,
                                                int kk ) const;

    ////    Private Attributes    ////
    private:
        ///
        unsigned int m_labelNextCounter[3];

        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        AsmPicoBlazeSymbolTable * const m_symbolTable;

        ///
        AsmPicoBlazeCodeListing * const m_codeListing;

        ///
        std::vector<int> m_forLoopIterRegs;
};

#endif // ASMPICOBLAZESPECIALMACROS_H
