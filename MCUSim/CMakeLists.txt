
# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(MCU_Simulator)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.6)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)


# ------------------------------------------------------------------------------
# BUILD
# ------------------------------------------------------------------------------

## Build the AVR8 Simulator
 #
 # Add a subdirectory "./AVR8" to the build. The subdirectory must contain
 # its own "CmakeLists.txt" file.
add_subdirectory(AVR8)
