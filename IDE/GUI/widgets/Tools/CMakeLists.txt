##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(Tools)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build the special development tools

if ( MDS_FEATURE_8_SEGMENT_EDITOR )
    add_subdirectory(Display)
endif()

if ( MDS_FEATURE_CONVERTER_TOOL )
    add_subdirectory(Convertor)
endif()

# Build the simulator widgets

if ( MDS_FEATURE_SIM_LED_PANEL )
    add_subdirectory(SimLed)
endif()

if ( MDS_FEATURE_SIM_7_SEGMENT )
    add_subdirectory(Sim7Seg)
endif()

