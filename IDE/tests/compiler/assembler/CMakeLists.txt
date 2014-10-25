# ==============================================================================
#
# Unit tests for Assembler.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsAssembler )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Tests for PicoBlaze assembler.
if ( MDS_FEATURE_PICOBLAZE )
    add_subdirectory ( PicoBlaze )
endif()

# Tests for Adaptable assembler.
if ( MDS_FEATURE_ADAPTABLE_SIMULATOR )
    add_subdirectory ( Adaptable )
endif()

