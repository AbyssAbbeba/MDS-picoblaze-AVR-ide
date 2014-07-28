# ==============================================================================
#
# Documentation.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( docs )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build manual.
add_subdirectory ( manual )

# Build API documentation.
add_subdirectory ( doxygen )

# Unix/Linux style manual pages.
if ( NOT TARGET_OS STREQUAL "Windows" )
    add_subdirectory ( PosixManPages )
endif()
