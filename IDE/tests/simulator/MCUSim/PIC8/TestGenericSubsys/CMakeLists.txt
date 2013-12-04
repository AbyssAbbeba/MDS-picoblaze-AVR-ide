# ==============================================================================
#
# Unit tests for PIC8 Simulator, test suite for single instructions.

# TSC_TestGenericSubsys: T = Test, S = Simulator, C = PIC8, _<nameOfTheTestSuite>
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TSC_TestGenericSubsys )

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

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../../../../../simulators/MCUSim/PIC8"
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../../../utilities/MCUDataFiles"
                      "${CMAKE_CURRENT_BINARY_DIR}/../.." )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../../../../../simulators/MCUSim/PIC8"
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../../../utilities/MCUDataFiles"
                   "${CMAKE_CURRENT_BINARY_DIR}/../.." )

## Add switches to compiler command line
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O1         : Optimization, level 1.
add_definitions ( -pipe -std=c++0x -O1 )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestGenericSubsys.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        MCUSimTestScript
                        PIC8Sim
                        cunit )

## Add a target with no output so it will always be built.
 # Compile .asm files for all testcases in the `testcases' directory
add_custom_target ( "${PROJECT_NAME}_compile" ALL ${CMAKE_CURRENT_BINARY_DIR}/testcases/compileAll.sh
                    DEPENDS compiler
                    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/testcases"
                    COMMENT "Compiling .asm files for all testcases in the `testcases' directory." )
