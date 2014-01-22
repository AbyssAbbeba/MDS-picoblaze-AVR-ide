# ==============================================================================
#
# Unit tests for PicoBlaze Assembler.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsAssemblerPicoBlaze )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Test suite: Successful compilation & file generation.
add_subdirectory ( TestSuiteSuc )

# Test suite: Error cases.
add_subdirectory ( TestSuiteErr )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities/AutoTest/" )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/TestSuiteSuc/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/TestSuiteErr/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities/AutoTest/" )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        ${Boost_LIBRARIES}
                        TAP_TestSuiteSuc
                        TAP_TestSuiteErr
                        AutoTest )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

ADD_TEST_SUBJECT ( "Assembler_PicoBlaze" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}" )
