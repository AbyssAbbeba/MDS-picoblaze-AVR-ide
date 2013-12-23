# ==============================================================================
#
# Unit tests for PicoBlaze Assembler, test suite for successful files generation.
#
# TAP_TestSuiteSuc: T = Test, A = Assembler, P = PicoBlaze, _<nameOfTheTestSuite>
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TAP_TestSuiteErr )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# List of additional files that will be cleaned as a part of the "make clean" stage.
FILE ( GLOB resultFiles results/* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Find Boost include dirs and libraries.
find_package ( Boost 1.55
               REQUIRED
               COMPONENTS filesystem system )

# Add include directories to the build.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../../../compiler" )

# Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS}
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../../../compiler/core" )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestSuiteErr.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        ${Boost_LIBRARIES}
                        CompilerCore
                        cunit )
