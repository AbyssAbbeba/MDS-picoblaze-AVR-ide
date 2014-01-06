# ==============================================================================
#
# Library hidapi.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( hidapi )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Added for compatibility with Microsoft Windows.
link_directories ( "${CMAKE_CURRENT_SOURCE_DIR}" )
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}" )

if ( CMAKE_HOST_UNIX )
    # GNU/Linux
    set ( SOURCE_FILES "hid-linux.c" )
    set ( LIBRARIES usb-1.0 udev)
elseif ( CMAKE_HOST_WIN32 )
    # Microsoft Windows
    set ( SOURCE_FILES "hid-windows.c" )
    set ( LIBRARIES setupapi )
elseif ( CMAKE_HOST_APPLE )
    # Mac OS X
    set ( SOURCE_FILES "hid-mac.c" )
    set ( LIBRARIES IOKit CoreFoundation )
endif()

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              ${SOURCE_FILES} )

## Disable compiler warnings for this library.
set_target_properties ( ${PROJECT_NAME} PROPERTIES
                                        COMPILE_FLAGS "-w" )

# Link the target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${LIBRARIES} )
