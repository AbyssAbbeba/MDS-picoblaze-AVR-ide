# ==============================================================================
#
# Unit tests for PicoBlaze Simulator.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsMCUSimPicoBlaze )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Test strategy for PicoBlaze simulator.
add_subdirectory ( PicoBlazeStrategy )

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
                      "${CMAKE_SOURCE_DIR}/utilities/AutoTest/"  )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        TSP_TestKcpsm1CPLD
                                        TSP_TestKcpsm1
                                        TSP_TestKcpsm2
                                        TSP_TestKcpsm3
                                        TSP_TestKcpsm6
                                        AutoTest )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

ADD_TEST_SUBJECT ( "MCU_Simulator_PicoBlaze" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}" )
