/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */

#include "AVR8/AVR8Sim.h"
#include "AVR8/AVR8ProgramMemory.h"
#include "AVR8/AVR8DataMemory.h"
#include "AVR8/AVR8DataEEPROM.h"
#include "AVR8/AVR8InstructionSet.h"
#include "MCUDataFiles/HexFile.h"
#include "McuSimCfgMgr/McuSimCfgMgr.h"

#include <cstdlib>
#include <iostream>
#include <CUnit/Basic.h>

AVR8Sim * g_AVR8Sim;
HexFile * g_HexFile;
McuSimCfgMgr * g_McuSimCfgMgr;

const char * MCUSPECFILE = "../../simulators/MCUSim/McuSimCfgMgr/mcuspecfile.xml";

int init_suite1()
{
	g_AVR8Sim = new AVR8Sim();
	g_HexFile = new HexFile();
	g_McuSimCfgMgr = new McuSimCfgMgr();

	if (
		( NULL == g_AVR8Sim )
			||
		( NULL == g_HexFile )
			||
		( NULL == g_McuSimCfgMgr )
	) {
		return -1;
	} else {
		return 0;
	}
}

int clean_suite1()
{
	if ( NULL != g_AVR8Sim ) {
		delete g_AVR8Sim;
	}
	if ( NULL != g_HexFile ) {
		delete g_HexFile;
	}
	if ( NULL != g_McuSimCfgMgr ) {
		delete g_McuSimCfgMgr;
	}
	return 0;
}

void test_suite1_openConfigFile()
{
      CU_ASSERT ( true == g_McuSimCfgMgr->openConfigFile(MCUSPECFILE) );
}

void test_suite1_clearAndLoad()
{
	const char * filename = "avr8_test0.hex";

	try {
	      g_HexFile->clearAndLoad(filename);
	} catch ( DataFile::DataFileException e ) {
		CU_FAIL("Failed to load program memory from the given IHEX file.")
	}
}

void test_suite1_setupSimulator()
{
	try {
		g_McuSimCfgMgr->setupSimulator("ATmega8A", g_AVR8Sim->getConfig());

		g_AVR8Sim->reset(MCUSim::RSTMD_NEW_CONFIG);
		g_AVR8Sim->reset(MCUSim::RSTMD_INITIAL_VALUES);
		g_AVR8Sim->reset(MCUSim::RSTMD_MCU_RESET);
	} catch ( ... ) {
		CU_FAIL("Failed to setup and reset simulator.")
	}
}

void test_suite1_loadAndCopyProgramFile()
{
	const char * filename = "_avr8_test0.hex";

	dynamic_cast<AVR8ProgramMemory*>(g_AVR8Sim->getSubsys(MCUSim::Subsys::ID_MEM_CODE))->loadDataFile(g_HexFile);
	dynamic_cast<AVR8ProgramMemory*>(g_AVR8Sim->getSubsys(MCUSim::Subsys::ID_MEM_CODE))->storeInDataFile(g_HexFile);
	g_HexFile->save(filename);

	#ifdef __linux__ // Code specific for Linux
		if ( 0 != system(NULL) ) {
			CU_ASSERT ( 0 == system("diff _avr8_test0.hex avr8_test0.hex > /dev/null") );
		}
	#endif // __linux__
}

void test_suite1_startSimulator()
{
	for ( int i = 0; i < 3; i++ ) {
		CU_ASSERT ( 2 == g_AVR8Sim->executeInstruction() );
		CU_ASSERT ( 0 == dynamic_cast<AVR8InstructionSet*>(g_AVR8Sim->getSubsys(MCUSim::Subsys::ID_CPU))->getProgramCounter() );
	}
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main(int argc, char ** argv)
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry()) {
		return CU_get_error();
	}

	/* add a suite to the registry */
	pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (
		(NULL == CU_add_test(pSuite, "test of openConfigFile()", test_suite1_openConfigFile))
			||
		(NULL == CU_add_test(pSuite, "test of clearAndLoad()", test_suite1_clearAndLoad))
			||
		(NULL == CU_add_test(pSuite, "test of setupSimulator()", test_suite1_setupSimulator))
			||
		(NULL == CU_add_test(pSuite, "test of loadAndCopyProgramFile()", test_suite1_loadAndCopyProgramFile))
			||
		(NULL == CU_add_test(pSuite, "test of startSimulator()", test_suite1_startSimulator))
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
