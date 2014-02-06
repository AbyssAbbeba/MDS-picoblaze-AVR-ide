# ==============================================================================
#
# Unit tests for PicoBlaze Simulator, test suite for single instructions.

# TDP_TestKcpsm1: T = Test, D = Disassembler, P = PicoBlaze, _<nameOfTheTestSuite>
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TDP_TestKcpsm1 )

# List of additional files that will be cleaned as a part of the "make clean" stage.
FILE ( GLOB resultFiles results/*.* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/utilities" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestKcpsm1.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        MCUDataFiles
                                        compiler
                                        disassembler
                                        CUnit )
