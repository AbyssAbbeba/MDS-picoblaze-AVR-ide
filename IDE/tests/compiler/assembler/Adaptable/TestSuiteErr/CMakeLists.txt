# ==============================================================================
#
# Unit tests for Adaptable Assembler, test suite for successful files generation.
#
# TAA_TestSuiteErr: T = Test, A = Assembler, A = Adaptable, _<nameOfTheTestSuite>
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TAA_TestSuiteErr )

# List of additional files that will be cleaned as a part of the "make clean" stage.
FILE ( GLOB resultFiles results/* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/compiler" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestSuiteErr.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        compiler
                                        CUnit )
