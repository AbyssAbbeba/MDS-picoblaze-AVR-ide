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
                    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/results2html.sh
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    COMMENT "Processing test results." )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Unit tests for Assembly language support library.
add_subdirectory ( AsmTranslator )

# Tests for compiler
add_subdirectory ( compiler )

# Tests for disassembler
add_subdirectory ( disassembler )

# Tests machine code containers.
add_subdirectory ( MCUDataFiles )

# Tests for simulator
add_subdirectory ( simulator )

# Unit tests for Moravia Script.
add_subdirectory ( MScript )
