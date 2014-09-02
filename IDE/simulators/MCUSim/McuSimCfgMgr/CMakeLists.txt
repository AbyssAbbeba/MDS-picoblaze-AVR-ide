# ==============================================================================
#
# MCU simulator configuration management library.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( McuSimCfgMgr )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/.."
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../utilities/" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} OBJECT
              McuSimCfgMgr.cxx
              McuSimCfgMgrAVR8.cxx
              McuSimCfgMgrPIC8.cxx
              McuDeviceSpecAVR8.cxx
              McuDeviceSpecPIC8.cxx
              McuDeviceSpecAdaptable.cxx )
