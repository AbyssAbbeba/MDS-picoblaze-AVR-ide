# ==============================================================================
#
# libraries of microcontroller simulators.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( MCUSimulator )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )


# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build the AVR8 Simulator
add_subdirectory ( AVR8 )

# Build the PIC8 Simulator
add_subdirectory ( PIC8 )

# Build the KCPSMx (PicoBlaze) Simulator
add_subdirectory ( PicoBlaze )

# Build the MCU simulator configuration management
add_subdirectory ( McuSimCfgMgr )
