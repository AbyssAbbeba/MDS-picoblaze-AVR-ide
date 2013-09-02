# ==============================================================================
#
# Unit tests for PicoBlaze Disassembler.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsDisasmPicoBlaze )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# After this command, you can add tests in the project using ADD_TEST command.
enable_testing()


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

# Find Boost include dirs and libraries.
find_package ( Boost 1.46
               REQUIRED
               COMPONENTS filesystem system )

# # Add include directories to the build.
include_directories ( ${Boost_INCLUDE_DIRS} )

# # Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS} )

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
                        ${Boost_LIBRARIES}
                        TDP_TestKcpsm1CPLD
                        TDP_TestKcpsm1
                        TDP_TestKcpsm2
                        TDP_TestKcpsm3
                        TDP_TestKcpsm6
                        cunit )


# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test ( NAME ${PROJECT_NAME}
           COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} )
