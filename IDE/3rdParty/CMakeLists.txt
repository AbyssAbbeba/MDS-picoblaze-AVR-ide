# ==============================================================================
#
# 3rd party software.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( 3rdParty )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build modified CUnit library.
if ( BUILD_TESTING )
    add_subdirectory ( CUnit )
endif()

# Build hidapi library.
# add_subdirectory ( hidapi )
