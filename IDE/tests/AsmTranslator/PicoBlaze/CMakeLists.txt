# ==============================================================================
#
# Unit tests.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsAsmTranslatorPicoBlaze )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# After this command, you can add tests in the project using ADD_TEST command.
enable_testing()


# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Tests for Mediatronix assembler variant.
add_subdirectory ( Mediatronix )

# Tests for openPICIDE assembler variant.
add_subdirectory ( openPICIDE )

# Tests for Xilinx assembler variant.
add_subdirectory ( Xilinx )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Find Boost include directories and libraries.
find_package ( Boost 1.55
               REQUIRED
               COMPONENTS filesystem system )

# Add include directories to the build.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../compiler"
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../utilities/MCUDataFiles"
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../utilities/AsmTranslator" )

# Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS}
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../compiler/core"
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../utilities/MCUDataFiles"
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../utilities/AsmTranslator" )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        ${Boost_LIBRARIES}
                        TAT_TestPicoBlazeXil
                        TAT_TestPicoBlazeMed
                        TAT_TestPicoBlazeOPI
                        AsmTranslator
                        CompilerCore
                        MCUDataFiles
                        cunit )


# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test ( NAME ${PROJECT_NAME}
           COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} )
