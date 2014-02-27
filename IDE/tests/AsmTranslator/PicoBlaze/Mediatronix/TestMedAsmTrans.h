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
 * @ingroup testsAsmTranslator
 * @file TestMedAsmTrans.h
 */
// =============================================================================

#ifndef TESTMEDASMTRANS_H
#define TESTMEDASMTRANS_H

// Forward declarations.
class Compiler;
class CompilerOptions;
class CompilerMsgInterface;
class AsmTranslator;

// Standard header files.
#include <string>

// The CUnit testing framework.
#include "3rdParty/CUnit/Basic.h"

/**
 * @brief
 * @namespace TestMedAsmTrans
 * @ingroup testsAsmTranslator
 */
namespace TestMedAsmTrans
{
    ////    Operations    ////
        /**
         * @brief
         * @return
         */
        int init();

        /**
         * @brief
         * @return
         */
        int clean();

        /**
         * @brief
         * @param[in,out] suite
         * @return
         */
        bool addTests ( CU_pSuite suite );

        /**
         * @brief
         */
        void testFunction();

        /**
         * @brief
         * @param[in] suffix
         */
        void test ( const std::string & suffix );

        /**
         * @brief
         * @param[in] fileName1
         * @param[in] fileName2
         */
        void fileCompare ( const std::string & fileName1,
                           const std::string & fileName2 );

    ////    Attributes    ////
        /**
         * @brief
         */
        Compiler * m_compiler;

        /**
         * @brief
         */
        CompilerOptions * m_options;

        /**
         * @brief
         */
        CompilerMsgInterface * m_msgInt;

        /**
         * @brief
         */
        AsmTranslator * m_translator;
};

#endif // TESTMEDASMTRANS_H
