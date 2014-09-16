# ==============================================================================
#
# Unit tests for MCU Simulator.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
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
add_library ( MCUSimTestScript
              MCUSimTestScript.cxx
              MCUSimTestScriptStrategy.cxx )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Tests for PicoBlaze Simulator.
if ( MDS_FEATURE_PICOBLAZE )
    add_subdirectory ( PicoBlaze )
endif()

# Tests for PIC8 Simulator.
if ( MDS_FEATURE_PIC8 )
    add_subdirectory ( PIC8 )
endif()

# Tests for AVR8 Simulator.
if ( MDS_FEATURE_AVR8 )
    add_subdirectory ( AVR8 )
endif()
