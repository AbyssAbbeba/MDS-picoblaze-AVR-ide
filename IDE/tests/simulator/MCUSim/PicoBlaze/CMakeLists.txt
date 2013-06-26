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

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# After this command, you can add tests in the project using ADD_TEST command.
enable_testing()


# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# # Test suite: Successful compilation & file generation.
# add_subdirectory ( TestSuiteSuc )

# # Test suite: Error cases.
# add_subdirectory ( TestSuiteErr )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# # Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../../../../simulators/MCUSim/PicoBlaze" )

# # Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../../../../simulators/MCUSim/PicoBlaze" )

## Add switches to compiler command line
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O1         : Optimization, level 1.
add_definitions ( -pipe -std=c++0x -O1 )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
#                         TAP_TestSuiteSuc
#                         TAP_TestSuiteErr
                        cunit )


# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test ( NAME ${PROJECT_NAME}
           COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} )
