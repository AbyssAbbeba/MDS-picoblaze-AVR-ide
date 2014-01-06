# ==============================================================================
#
# Multi-target multi-language cross compiler.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( assemblerIncludeFiles )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build additional file of the compiler for PicoBlaze assembler.
add_subdirectory ( PicoBlaze )
