# ==============================================================================
#
# Unit tests.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
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
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/compiler/"
                      "${CMAKE_SOURCE_DIR}/utilities/AutoTest/"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles/"
                      "${CMAKE_SOURCE_DIR}/utilities/AsmTranslator/" )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME} main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        TAT_TestPicoBlazeXil
                                        TAT_TestPicoBlazeMed
                                        TAT_TestPicoBlazeOPI
                                        AsmTranslator
                                        compiler
                                        MCUDataFiles
                                        AutoTest )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test_subject ( "AsmTranslator_PicoBlaze" ${PROJECT_NAME} )

set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "Mediatronix/results;openPICIDE/results;Xilinx/results" )
