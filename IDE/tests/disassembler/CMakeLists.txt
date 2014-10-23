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
project ( testsDisassembler )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Tests for PicoBlaze disassembler.
if ( MDS_FEATURE_PICOBLAZE )
    add_subdirectory ( PicoBlaze )
endif()

# Tests for Adaptable disassembler.
if ( MDS_FEATURE_ADAPTABLE_SIMULATOR )
    add_subdirectory ( Adaptable )
endif()
