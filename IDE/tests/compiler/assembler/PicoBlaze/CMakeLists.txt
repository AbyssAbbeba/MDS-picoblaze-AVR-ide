# ==============================================================================
#
# Unit tests for PicoBlaze Assembler.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
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
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/utilities/AutoTest/" )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        TAP_TestSuiteSuc
                                        TAP_TestSuiteErr
                                        AutoTest )

add_dependencies ( ${PROJECT_NAME}
                   PicoBlazeAsmIncludeFiles_prc_file_kcpsm1
                   PicoBlazeAsmIncludeFiles_prc_file_kcpsm1cpld
                   PicoBlazeAsmIncludeFiles_prc_file_kcpsm2
                   PicoBlazeAsmIncludeFiles_prc_file_kcpsm3
                   PicoBlazeAsmIncludeFiles_prc_file_kcpsm6 )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test_subject ( "Assembler_PicoBlaze" ${PROJECT_NAME} )
