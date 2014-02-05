# ==============================================================================
#
# Simulator libraries.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( simulators )


# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build control simulator control unit
add_subdirectory ( SimControl )

# Build simulators for microcontrollers
add_subdirectory ( MCUSim )
