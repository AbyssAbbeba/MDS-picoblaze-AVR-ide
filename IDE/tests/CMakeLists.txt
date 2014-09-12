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
project ( tests )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of additional files that will be cleaned as a part of the "make clean" stage.
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "results" )

## Add a target with no output so it will always be built.
 # Process test results and generate corresponding HTML files.
add_custom_target ( test_analysis
                    COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/results2html.sh" ${TEST_COVERAGE}
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    COMMENT "Processing test results." )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Tests for compiler
add_subdirectory ( compiler )

# Unit tests for Assembly language support library.
if ( MDS_FEATURE_TRANSLATOR )
    add_subdirectory ( AsmTranslator )
endif()

# Tests for disassembler
if ( MDS_FEATURE_DISASSEMBLER )
    add_subdirectory ( disassembler )
endif()

# Tests for simulator
add_subdirectory ( simulator )

# Tests machine code containers.
add_subdirectory ( MCUDataFiles )

# Unit tests for Moravia Script.
# add_subdirectory ( MScript )
