# ==============================================================================
#
# Unit tests for Adaptable Simulator, test suite for single instructions.

# TDA_TestAdaptable: T = Test, D = Disassembler, A = Adaptable, _<nameOfTheTestSuite>
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsDisasmAdaptable )

# List of additional files that will be cleaned as a part of the "make clean" stage.
FILE ( GLOB resultFiles results/*.* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/utilities"
                      "${CMAKE_SOURCE_DIR}/utilities/AutoTest/" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( TDA_TestAdaptable SHARED
              TestAdaptable.cxx )

# Link a target to given libraries
target_link_libraries ( TDA_TestAdaptable ${Boost_LIBRARIES}
                                          CUnit
                                          compiler
                                          MCUDataFiles
                                          disassembler )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        TDA_TestAdaptable
                                        AutoTest )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test_subject ( "Disassembler_Adaptable" ${PROJECT_NAME} )
