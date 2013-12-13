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
 * @file TestXilAsmTrans.h
 */
// =============================================================================

#ifndef TESTXILASMTRANS_H
#define TESTXILASMTRANS_H

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
 * @namespace TestXilAsmTrans
 * @ingroup testsAsmTranslator
 */
namespace TestXilAsmTrans
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

#endif // TESTXILASMTRANS_H
