# ==============================================================================
#
# Unit tests for PicoBlaze Simulator, test suite for single instructions.

# TSP_TestSingleInst: T = Test, S = Simulator, P = PicoBlaze, _<nameOfTheTestSuite>
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TSP_TestSingleInst )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# After this command, you can add tests in the project using ADD_TEST command.
enable_testing()


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Find Boost include directories and libraries.
find_package ( Boost 1.46
               REQUIRED
               COMPONENTS filesystem system )

# Add include directories to the build.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../../../compiler"
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../../../utilities/MCUDataFiles"
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../../../utilities/DbgFile" )

# Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS}
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../../../compiler/core"
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../../../utilities/MCUDataFiles"
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../../../utilities/DbgFile" )

## Add switches to compiler command line
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O1         : Optimization, level 1.
add_definitions ( -pipe -std=c++0x -O1 )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestSingleInst.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        ${Boost_LIBRARIES}
                        cunit )
