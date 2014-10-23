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
 * @ingroup testsMCUSimAdaptable
 * @file TestAdaptable.h
 */
// =============================================================================

#ifndef TESTADAPTABLE_H
#define TESTADAPTABLE_H

// Forward declarations.
class HexFile;
class AdaptableSim;
class MCUSimTestScript;
class AdaptableStrategy;

// The CUnit testing framework.
#include "3rdParty/CUnit/Basic.h"

/**
 * @brief
 * @namespace TestAdaptable
 * @ingroup testsMCUSimAdaptable
 */
namespace TestAdaptable
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
        AdaptableSim * m_adaptableSim;

        /**
         * @brief
         */
        HexFile * m_programFile;

        /**
         * @brief
         */
        MCUSimTestScript * m_testScript;

        /**
         * @brief
         */
        AdaptableStrategy * m_strategy;
};

#endif // TESTADAPTABLE_H
