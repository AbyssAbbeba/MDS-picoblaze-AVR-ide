# ==============================================================================
#
# Tests the MDS C compiler.
#
# TCC_TestSuitePreprocessor: T = Test, CC = C Compiler, _<nameOfTheTestSuite>
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TCC_TestSuitePreprocessor )

# List of additional files that will be cleaned as a part of the "make clean" stage.
FILE ( GLOB resultFiles results/* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}" )

## Add a library to the project using the specified source files,
add_library ( ${PROJECT_NAME} SHARED
              TestSuitePreprocessor.cxx )
