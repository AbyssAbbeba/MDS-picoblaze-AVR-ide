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

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# After this command, you can add tests in the project using ADD_TEST command.
enable_testing()


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../../../simulators/MCUSim" )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
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
