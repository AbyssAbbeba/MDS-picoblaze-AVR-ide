# ==============================================================================
#
# Unit tests for MCU Simulator.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsMCUSimulator )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../../simulators/MCUSim" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( MCUSimTestScript SHARED
              MCUSimTestScript.cxx
              MCUSimTestScriptStrategy.cxx )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Tests for PicoBlaze Simulator.
add_subdirectory ( PicoBlaze )

# Tests for PIC8 Simulator.
add_subdirectory ( PIC8 )

# Tests for AVR8 Simulator.
add_subdirectory ( AVR8 )
