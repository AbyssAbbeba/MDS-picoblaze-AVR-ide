# ==============================================================================
#
# Unit tests for PicoBlaze Disassembler.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsDisasmPicoBlaze )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Test suite: KCPSM1CPLD.
add_subdirectory ( TestKcpsm1CPLD )

# Test suite: KCPSM1.
add_subdirectory ( TestKcpsm1 )

# Test suite: KCPSM2.
add_subdirectory ( TestKcpsm2 )

# Test suite: KCPSM3.
add_subdirectory ( TestKcpsm3 )

# Test suite: KCPSM6.
add_subdirectory ( TestKcpsm6 )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/utilities/AutoTest/" )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        TDP_TestKcpsm1CPLD
                                        TDP_TestKcpsm1
                                        TDP_TestKcpsm2
                                        TDP_TestKcpsm3
                                        TDP_TestKcpsm6
                                        AutoTest )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test_subject ( "Disassembler_PicoBlaze" ${PROJECT_NAME} )
