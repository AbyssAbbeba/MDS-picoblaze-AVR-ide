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
# CONFIGURE FEATURES
# ------------------------------------------------------------------------------

# List of additional features which can be enabled or disabled for this particular build.
set ( MDS_ADDITONAL_FILES "" )

if ( MDS_FEATURE_AVR8 )
    list ( APPEND MDS_ADDITONAL_FILES McuSimCfgMgrAVR8.cxx
                                      McuDeviceSpecAVR8.cxx )
endif()

if ( MDS_FEATURE_PIC8 )
    list ( APPEND MDS_ADDITONAL_FILES McuSimCfgMgrPIC8.cxx
                                      McuDeviceSpecPIC8.cxx )
endif()

if ( MDS_FEATURE_ADAPTABLE_SIMULATOR )
    list ( APPEND MDS_ADDITONAL_FILES McuDeviceSpecAdaptable.cxx )
endif()

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../"
                      "${CMAKE_SOURCE_DIR}/utilities/" )

## Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME} OBJECT
              ${MDS_ADDITONAL_FILES}
              McuSimCfgMgr.cxx )
