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
 * @ingroup testsMCUSimPIC8
 * @file TestGenericSubsys.h
 */
// =============================================================================

#ifndef TESTGENERICSUBSYS_H
#define TESTGENERICSUBSYS_H

// Forward declarations.
class PIC8Sim;
class HexFile;
class MCUSimTestScript;

// The CUnit testing framework.
#include <CUnit/Basic.h>

/**
 * @brief
 * @namespace TestGenericSubsys
 * @ingroup testsMCUSimPIC8
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
        PIC8Sim * m_pic8Sim;

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
