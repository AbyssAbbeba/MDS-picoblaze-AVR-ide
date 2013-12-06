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
 * @ingroup testsMCUSimPicoBlaze
 * @file TestKcpsm2.h
 */
// =============================================================================

#ifndef TESTKCPSM2_H
#define TESTKCPSM2_H

// Forward declarations.
class HexFile;
class PicoBlazeSim;
class MCUSimTestScript;
class PicoBlazeStrategy;

// The CUnit testing framework.
#include <CUnit/Basic.h>

/**
 * @brief
 * @namespace TestKcpsm2
 * @ingroup testsMCUSimPicoBlaze
 */
namespace TestKcpsm2
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
        PicoBlazeSim * m_picoBlazeSim;

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
        PicoBlazeStrategy * m_strategy;
};

#endif // TESTKCPSM2_H
