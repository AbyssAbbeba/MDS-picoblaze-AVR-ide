# ==============================================================================
#
# The entire Multitarget Development System.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( mmIDE )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# Set default build type to Debug
if ( NOT CMAKE_BUILD_TYPE )
  set ( CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE )
endif ( NOT CMAKE_BUILD_TYPE )

# set ( CMAKE_INSTALL_PREFIX "/some/path" )

# After this command, you can add tests in the project using ADD_TEST command.
enable_testing()

# ------------------------------------------------------------------------------
# GENERATE API DOCUMENTATION WITH DOXYGEN
# ------------------------------------------------------------------------------

find_package ( Doxygen )
if ( DOXYGEN_FOUND )
    configure_file ( ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY )
    add_custom_target ( doc
                        ${DOXYGEN_EXECUTABLE}
                        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Generating API documentation with Doxygen" VERBATIM )
endif ( DOXYGEN_FOUND )


# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build utility libraries
add_subdirectory ( utilities )

# Build GUI
add_subdirectory ( GUI )

# Build HW (not for now due to "absence" of libUSB on MS Windows OS)
# add_subdirectory ( HW )

# Build simulators
add_subdirectory ( simulators )

# Build Moravia Script language support
add_subdirectory ( moraviascript )

# Build compiler collection
add_subdirectory ( compiler )

# Build disassemblers
add_subdirectory ( disassembler )

# Add some data files to installation packages
add_subdirectory ( pkgs )

# Build automated tests
if ( NOT CMAKE_HOST_WIN32 )
    add_subdirectory ( tests )
endif ( NOT CMAKE_HOST_WIN32 )


# ------------------------------------------------------------------------------
# BUILD INSTALLATION PACKAGES
# ------------------------------------------------------------------------------

# http://www.cmake.org/Wiki/CMake:CPackPackageGenerator

# Set which packages we want to build
if ( CMAKE_HOST_APPLE )
    # Apple MAC OS X
    set ( CPACK_GENERATOR "" )
elseif ( CMAKE_HOST_WIN32 )
    # Microsoft Windows
    set ( CPACK_GENERATOR "NSIS" )
else ( CMAKE_HOST_WIN32 )
    # GNU/Linux and possibly other POSIX systems
    set ( CPACK_GENERATOR "DEB;RPM;STGZ" )
endif ( CMAKE_HOST_APPLE )

# General options
set ( CPACK_PACKAGE_VENDOR "Moravia Microsystems" )
set ( CPACK_PACKAGE_VERSION_MAJOR 0 )
set ( CPACK_PACKAGE_VERSION_MINOR 1 )
set ( CPACK_PACKAGE_VERSION_PATCH 0 )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Advanced development environment for small embedded systems." )
set ( MY_CPACK_PACKAGE_DETAILED_DESC "Some detailed description, like:
   * Advanced simulation capabilities including simulation of devices peripheral to the microcontroller.
   *  Multiplatform development tools allowing thousands, or tens of thousands, of engineers, scientist, and students, all around the world to use this kind of software and hardware tools also on GNU/Linux and Mac OS X operating systems.
   *  etc. etc.
" )

# Resource file (see the CPack manual for details)
set ( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_BINARY_DIR}/pkgs/pkgdesc.txt" )
set ( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/pkgs/pkglicense.txt" )
set ( CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_BINARY_DIR}/pkgs/pkgreadme.txt" )
set ( CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_BINARY_DIR}/pkgs/pkgwelcome.txt" )

# Options for the "DEB" generator
# set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
set ( CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-gui (>= 4.8), libqt4-xml (>= 4.8)" )
set ( CPACK_DEBIAN_PACKAGE_RECOMMENDS "" )
set ( CPACK_DEBIAN_PACKAGE_SUGGESTS "" )
set ( CPACK_PACKAGE_CONTACT "Moravia Microsystems, s.r.o., <some address>" )
set ( CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.someaddress.com" )
set ( CPACK_DEBIAN_PACKAGE_SECTION "Development" )
set ( CPACK_DEBIAN_PACKAGE_DESCRIPTION "\t${CPACK_PACKAGE_DESCRIPTION_SUMMARY}.\n\t\t${MY_CPACK_PACKAGE_DETAILED_DESC}" )

# Options for the "RPM" generator
set ( CPACK_RPM_PACKAGE_REQUIRES "qt >= 4.8" )
set ( CPACK_RPM_PACKAGE_SUGGESTS "" )
set ( CPACK_RPM_PACKAGE_URL "http://www.someaddress.com" )
set ( CPACK_RPM_PACKAGE_GROUP "Development/Tools/IDE" )
set ( CPACK_RPM_PACKAGE_LICENSE "<package license>" )

# Options for the "NSIS" generator
set ( CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_BINARY_DIR}/pkgs/mmIDEicon.ico" )
# set ( CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_BINARY_DIR}/pkgs/uninstaller.ico" )
# set ( CPACK_PACKAGE_ICON "${CMAKE_CURRENT_BINARY_DIR}/pkgs/installer.bmp" )
set ( CPACK_NSIS_DISPLAY_NAME "mmIDE" )
set ( CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe'" )
# set ( CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_CURRENT_BINARY_DIR}/pkgs/something.exe" )
set ( CPACK_NSIS_HELP_LINK "http://www.my-project-home-page.org" )
set ( CPACK_NSIS_URL_INFO_ABOUT "http://www.my-project-home-page.org" )
set ( CPACK_NSIS_CONTACT "support@abc.xyz" )
set ( CPACK_NSIS_MUI_FINISHPAGE_RUN "mmIDE.exe" )

# Include packaging driver provided by CMake (that means generate installation packages).
include ( CPack )


# ------------------------------------------------------------------------------
# SUPPORT FOR AUTOMATED TESTS
# ------------------------------------------------------------------------------

# CTest is a testing tool distributed as a part of CMake.
include ( CTest )

