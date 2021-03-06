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
 * @file TestMScriptIII.h
 */
// =============================================================================

#ifndef TESTMSCRIPTIII_H
#define TESTMSCRIPTIII_H

// Forward declarations.
class MScript;
class MScriptStrategy;

// The CUnit testing framework.
#include "3rdParty/CUnit/Basic.h"

/**
 * @brief
 * @namespace TestMScriptIII
 * @ingroup testsMScript
 */
namespace TestMScriptIII
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

#endif // TESTMSCRIPTIII_H
