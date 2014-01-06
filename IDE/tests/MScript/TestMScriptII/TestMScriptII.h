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
 * @ingroup testsMScript
 * @file TestMScriptII.h
 */
// =============================================================================

#ifndef TESTMSCRIPTII_H
#define TESTMSCRIPTII_H

// Forward declarations.
class MScript;
class MScriptStrategy;

// The CUnit testing framework.
#include "3rdParty/CUnit/Basic.h"

/**
 * @brief
 * @namespace TestMScriptII
 * @ingroup testsMScript
 */
namespace TestMScriptII
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
        MScript * m_core;

        /**
         * @brief
         */
        MScriptStrategy * m_testStrategy;
};

#endif // TESTMSCRIPTII_H
