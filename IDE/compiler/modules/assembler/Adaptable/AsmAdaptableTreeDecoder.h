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
         * @param[in,out] semanticAnalyzer
         * @return
         */
        AsmAdaptableTreeDecoder ( AsmAdaptableSemanticAnalyzer * semanticAnalyzer);

        /**
         * @brief
         */
        ~AsmAdaptableTreeDecoder();

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
         * @param[in] expr
         * @return
         */
        bool isBlank ( const CompilerExpr * expr ) const;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] mnemonic
         * @return
         */
        inline bool isInstruction ( const std::string & mnemonic );

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
        inline void dir_DB_phase2 ( CompilerStatement * node );

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
        inline void dir_DEVICE ( CompilerStatement * node );

        /**
         * @brief
         * @param[in] mnemonic
         * @param[in,out] node
         * @return
         */
        inline CourseOfAction instruction2db ( const std::string & mnemonic,
                                               CompilerStatement * node );

        /**
         * @brief
         * @param[in] node
         * @return
         */
        inline CourseOfAction dir_DB ( const CompilerStatement * node );

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
         * @param[in] mnemonic
         * @param[in] node
         * @return
         */
        inline CompilerStatement * resolveInstruction ( const std::string & mnemonic,
                                                        const CompilerStatement * node );

    ////    Private Attributes    ////
    private:
        ///
        AsmAdaptableSemanticAnalyzer * m_semanticAnalyzer;

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

#endif // ASMADAPTABLETREEDECODER_H
