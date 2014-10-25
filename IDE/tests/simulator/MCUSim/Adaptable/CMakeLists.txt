# ==============================================================================
#
# Unit tests for Adaptable Simulator.
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsMCUSimAdaptable )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Test strategy for Adaptable simulator.
add_subdirectory ( AdaptableStrategy )

# Test suite: Adaptable.
add_subdirectory ( TestAdaptable )

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
                                        TSA_Adaptable
                                        AutoTest )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test_subject ( "MCU_Simulator_Adaptable" ${PROJECT_NAME} )
