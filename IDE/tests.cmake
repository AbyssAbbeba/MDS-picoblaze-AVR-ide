# ==============================================================================
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# author: Martin Ošmera <martin.osmera@moravia-microsystems.com>
#
# ==============================================================================

macro ( ADD_TEST_SUBJECT testName testBinary )
    set ( testDir "${CMAKE_SOURCE_DIR}/tests" )

    if ( TEST_COVERAGE )
        set ( coverageFlag 1 )
    else()
        set ( coverageFlag 0 )
    endif()

    if ( TEST_MEMCHECK )
        add_test ( NAME "${testName}"
                   WORKING_DIRECTORY "${testDir}"
                   COMMAND "${testDir}/coverage.sh" "${testName}" "${coverageFlag}"
                            valgrind --leak-check=full
                                     --track-origins=yes
                                     "--log-file=${testDir}/results/${testName}-Valgring.log"
                           "${testBinary}" -x "${testDir}/results/${testName}" )
    else()
        add_test ( NAME "${testName}"
                   WORKING_DIRECTORY "${testDir}"
                   COMMAND "${testDir}/coverage.sh" "${testName}" "${coverageFlag}"
                           "${testBinary}" -x "${testDir}/results/${testName}" )
    endif()
endmacro()

# tests.cmake ends here
