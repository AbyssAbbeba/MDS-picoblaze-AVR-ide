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

# Build the MCU simulator configuration management
add_subdirectory ( McuSimCfgMgr )

# Build the AVR8 Simulator
if ( MDS_FEATURE_AVR8 )
    add_subdirectory ( AVR8 )
endif()

# Build the PIC8 Simulator
if ( MDS_FEATURE_PIC8 )
    add_subdirectory ( PIC8 )
endif()

# Build the KCPSMx (PicoBlaze) Simulator
if ( MDS_FEATURE_PICOBLAZE )
    add_subdirectory ( PicoBlaze )
endif()

# Build the Adaptable Simulator
if ( MDS_FEATURE_ADAPTABLE_SIMULATOR )
    add_subdirectory ( AdaptableSim )
endif()
