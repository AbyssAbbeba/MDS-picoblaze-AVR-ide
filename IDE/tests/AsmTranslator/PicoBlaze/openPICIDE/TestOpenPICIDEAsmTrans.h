 
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
 * @file TestOpenPICIDEAsmTrans.h
 */
// =============================================================================

#ifndef TESTOPENPICIDEASMTRANS_H
#define TESTOPENPICIDEASMTRANS_H

// Forward declarations.
class Compiler;
class CompilerOptions;
class CompilerMsgInterface;
class AsmTranslator;

// Standard header files.
#include <string>

// The CUnit testing framework.
#include <CUnit/Basic.h>

/**
 * @brief
 * @namespace TestOpenPICIDEAsmTrans
 * @ingroup testsAsmTranslator
 */
namespace TestOpenPICIDEAsmTrans
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

#endif // TESTOPENPICIDEASMTRANS_H
