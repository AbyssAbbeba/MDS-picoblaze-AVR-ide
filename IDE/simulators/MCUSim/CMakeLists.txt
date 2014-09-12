# ==============================================================================
#
# libraries of microcontroller simulators.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( MCUSimulator )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build the AVR8 Simulator
add_subdirectory ( AVR8 )

# Build the PIC8 Simulator
add_subdirectory ( PIC8 )

# Build the KCPSMx (PicoBlaze) Simulator
add_subdirectory ( PicoBlaze )

# Build the Adaptable Simulator
if ( MDS_FEATURE_ADAPTABLE_SIMULATOR )
    add_subdirectory ( AdaptableSim )
endif()

# Build the MCU simulator configuration management
add_subdirectory ( McuSimCfgMgr )
