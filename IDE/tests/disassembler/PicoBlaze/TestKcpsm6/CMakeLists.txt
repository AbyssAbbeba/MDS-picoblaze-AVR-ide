# ==============================================================================
#
# Unit tests for PicoBlaze Simulator, test suite for single instructions.

# TDP_TestKcpsm6: T = Test, D = Disassembler, P = PicoBlaze, _<nameOfTheTestSuite>
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TDP_TestKcpsm6 )

# List of additional files that will be cleaned as a part of the "make clean" stage.
FILE ( GLOB resultFiles results/*.* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../../"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestKcpsm6.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        ${Boost_LIBRARIES}
                        MCUDataFiles
                        Compiler
                        disassembler
                        CUnit )
