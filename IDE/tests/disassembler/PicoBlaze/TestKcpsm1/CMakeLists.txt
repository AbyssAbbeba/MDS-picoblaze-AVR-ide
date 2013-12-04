# ==============================================================================
#
# Unit tests for PicoBlaze Simulator, test suite for single instructions.

# TDP_TestKcpsm1: T = Test, D = Disassembler, P = PicoBlaze, _<nameOfTheTestSuite>
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TDP_TestKcpsm1 )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# List of additional files that will be cleaned as a part of the "make clean" stage.
FILE ( GLOB resultFiles results/*.* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Find Boost include dirs and libraries.
find_package ( Boost 1.46
               REQUIRED
               COMPONENTS filesystem system )

# Add include directories to the build.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../../"
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../../utilities" )

# Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS}
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../../../utilities/MCUDataFiles"
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../../../compiler/core"
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../../../disassembler" )

## Add switches to compiler command line
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O1         : Optimization, level 1.
add_definitions ( -pipe -std=c++0x -O1 )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestKcpsm1.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        MCUDataFiles
                        CompilerCore
                        disassembler
                        cunit )
