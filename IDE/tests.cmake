# ==============================================================================
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# author: Martin Ošmera <martin.osmera@moravia-microsystems.com>
#
# ==============================================================================

macro ( ADD_TEST_SUBJECT TestName TestBinaryTarget )
    set ( testDir "${CMAKE_SOURCE_DIR}/tests" )

    if ( TEST_MEMCHECK )
        # Run test subject with Memcheck.
        if ( TEST_COVERAGE )
            # Run test subject with Memcheck, and with Coverage.
            add_test ( NAME "${TestName}"
                       WORKING_DIRECTORY "${testDir}"
                       COMMAND bash "${testDir}/coverage.sh" "${TestName}" "${coverageFlag}"
                               valgrind --leak-check=full
                                        --track-origins=yes
                                        "--log-file=${testDir}/results/${TestName}-Valgring.log"
                               "$<TARGET_FILE:${TestBinaryTarget}>"
                                        --dir "${CMAKE_CURRENT_SOURCE_DIR}"
                                        --xml "${testDir}/results/${TestName}" )
        else()
            # Run test subject with Memcheck, and without Coverage.
            add_test ( NAME "${TestName}"
                       WORKING_DIRECTORY "${testDir}"
                       COMMAND valgrind --leak-check=full
                                        --track-origins=yes
                                        "--log-file=${testDir}/results/${TestName}-Valgring.log"
                               "$<TARGET_FILE:${TestBinaryTarget}>"
                                        --dir "${CMAKE_CURRENT_SOURCE_DIR}"
                                        --xml "${testDir}/results/${TestName}" )
        endif()
    else()
        # Run test subject without Memcheck.
        if ( TEST_COVERAGE )
            # Run test subject without Memcheck, and with Coverage.
            add_test ( NAME "${TestName}"
                       WORKING_DIRECTORY "${testDir}"
                       COMMAND bash "${testDir}/coverage.sh" "${TestName}"
                               "$<TARGET_FILE:${TestBinaryTarget}>"
                                    --dir "${CMAKE_CURRENT_SOURCE_DIR}"
                                    --xml "${testDir}/results/${TestName}" )
        else()
            # Run test subject without Memcheck, and without Coverage.
            add_test ( NAME "${TestName}"
                       WORKING_DIRECTORY "${testDir}"
                       COMMAND "$<TARGET_FILE:${TestBinaryTarget}>"
                                    --dir "${CMAKE_CURRENT_SOURCE_DIR}"
                                    --xml "${testDir}/results/${TestName}" )
        endif()
    endif()
endmacro()

# tests.cmake ends here
