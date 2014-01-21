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

# Add include directories to the build.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/compiler/"
                      "${CMAKE_SOURCE_DIR}/utilities/AutoTest/"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles/"
                      "${CMAKE_SOURCE_DIR}/utilities/AsmTranslator/" )

# Specify directories in which the linker will look for libraries.
link_directories (
                   "${CMAKE_SOURCE_DIR}/compiler/core/"
                   "${CMAKE_SOURCE_DIR}/utilities/AutoTest/"
                   "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles/"
                   "${CMAKE_SOURCE_DIR}/utilities/AsmTranslator/" )

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
                        AutoTest )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

ADD_TEST_SUBJECT ( "AsmTranslator_PicoBlaze" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}" )
