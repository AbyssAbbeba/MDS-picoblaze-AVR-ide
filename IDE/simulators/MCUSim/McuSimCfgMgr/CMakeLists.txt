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

# List of additional simulator modules which can be enabled or disabled for this particular build.
set ( MDS_ADDITONAL_FILES "" )

if ( MDS_FEATURE_ADAPTABLE_SIMULATOR )
    list ( APPEND MDS_ADDITONAL_FILES "McuDeviceSpecAdaptable.cxx" )
endif()

## Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME} OBJECT
              ${MDS_ADDITONAL_FILES}
              McuSimCfgMgr.cxx
              McuSimCfgMgrAVR8.cxx
              McuSimCfgMgrPIC8.cxx
              McuDeviceSpecAVR8.cxx
              McuDeviceSpecPIC8.cxx )
