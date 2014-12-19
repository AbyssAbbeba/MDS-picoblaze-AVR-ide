# ==============================================================================
#
# Unit tests for compiler.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsCompiler )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Tests for assembler.
add_subdirectory ( assembler )

# Tests for C compiler.
if ( MDS_FEATURE_C_COMPILER )
    add_subdirectory ( C )
endif()
