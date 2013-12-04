// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsMCUSimPicoBlaze
 * @file TestGenericSubsys.h
 */
// =============================================================================

#ifndef TESTGENERICSUBSYS_H
#define TESTGENERICSUBSYS_H

// Forward declarations.
class AVR8Sim;
class HexFile;
class MCUSimTestScript;

// The CUnit testing framework.
#include <CUnit/Basic.h>

/**
 * @brief
 * @namespace TestGenericSubsys
 * @ingroup testsMCUSimPicoBlaze
 */
namespace TestGenericSubsys
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
        AVR8Sim * m_avr8Sim;

        /**
         * @brief
         */
        HexFile * m_programFile;

        /**
         * @brief
         */
        MCUSimTestScript * m_testScript;
};

#endif // TESTGENERICSUBSYS_H
