# ==============================================================================
#
# Utility for working with USB devices in HID class.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( hid_util )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Added for compatibility with Microsoft Windows OS
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}" )

## Add switches to compiler command line
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -O1         : Optimization, level 1.
add_definitions(-pipe -O1)

if ( CMAKE_HOST_UNIX )
    # GNU/Linux
    set ( HID "hid-linux.c" )
elseif ( CMAKE_HOST_WIN32 )
    # Microsoft Windows
    set ( HID "hid-windows.c" )
elseif ( CMAKE_HOST_APPLE )
    # Mac OS X
    set ( HID "hid-mac.c" )
endif()

add_executable ( ${PROJECT_NAME}
                 ${HID}
                 main.cxx )

if ( CMAKE_HOST_UNIX )
    # GNU/Linux
    target_link_libraries ( ${PROJECT_NAME} usb-1.0 udev )
elseif ( CMAKE_HOST_WIN32 )
    # Microsoft Windows
    target_link_libraries ( ${PROJECT_NAME} setupapi )
elseif ( CMAKE_HOST_APPLE )
    # Mac OS X
    target_link_libraries ( ${PROJECT_NAME} IOKit CoreFoundation )
endif()
