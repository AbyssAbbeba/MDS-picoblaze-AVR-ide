# ==============================================================================
#
# The entire Multitarget Development System.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# M   M  OO  RRR    A   V   V III   A      M   M III  OOO  RRR   OO   SSSS Y   Y  SSSS TTTTT EEEEE M   M  SSSS
# MM MM O  O R  R  A A  V   V  I   A A     MM MM  I  O   O R  R O  O S      Y Y  S       T   E     MM MM S
# M M M O  O RRR  A   A V   V  I  A   A    M M M  I  O   O RRR  O  O  SS     Y    SS     T   EEEEE M M M  SS
# M   M O  O RR   AAAAA  V V   I  AAAAA    M   M  I  O   O RR   O  O    SS   Y      SS   T   E     M   M    SS
# M   M O  O R R  A   A  V V   I  A   A    M   M  I  O   O R R  O  O      S  Y        S  T   E     M   M      S
# M   M  OO  R  R A   A   V   III A   A    M   M III  OOO  R  R  OO   SSSS   Y    SSSS   T   EEEEE M   M  SSSS

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( mmIDE )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 FATAL_ERROR )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# Set default build type to Debug.
if ( NOT CMAKE_BUILD_TYPE )
  set ( CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE )
endif()
message ( STATUS "Configuring environment for build in mode: ${CMAKE_BUILD_TYPE}" )

## Set global compilation flags:
 # - for all build types:
set ( CMAKE_CXX_FLAGS            "-Wall -Wextra -std=c++0x -pipe" )
 # - for debug build only:
set ( CMAKE_CXX_FLAGS_DEBUG      "-O0 -g" )
 # - for release build only:
set ( CMAKE_CXX_FLAGS_RELEASE    "-O3 -DNDEBUG" )
 # - for minimum size release build only:
set ( CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG" )

# Set the same compilation flags for C as they are set for C++.
set ( CMAKE_C_FLAGS              "${CMAKE_CXX_FLAGS}" )
set ( CMAKE_C_FLAGS_DEBUG        "${CMAKE_CXX_FLAGS_DEBUG}" )
set ( CMAKE_C_FLAGS_RELEASE      "${CMAKE_CXX_FLAGS_RELEASE}" )
set ( CMAKE_C_FLAGS_MINSIZEREL   "${CMAKE_CXX_FLAGS_MINSIZEREL}" )

# Optionally build with support for coverage testing and profiling support.
option ( TEST_COVERAGE "Configure build for coverage analysis and profiling." OFF )
if ( TEST_COVERAGE )
    message ( STATUS "This build is configured for coverage analysis and profiling!" )
    set ( CMAKE_EXE_LINKER_FLAGS "--coverage" )
    set ( CMAKE_SHARED_LINKER_FLAGS "--coverage" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage" )
endif()

# Optionally start test subjects in Valgrind:Memcheck.
option ( TEST_MEMCHECK "Start test subjects in Valgrind:Memcheck." ON )

# Optionally colorize compiler output messages.
if ( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCPP )
    option ( COLOR_GCC "Filter compiler output messages to add VT100 terminal compatible colors to them." ON )
    if ( COLOR_GCC )
        set_property ( GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_CURRENT_SOURCE_DIR}/gccColor.sh" )
    endif()
endif()

# Include our own CMake scripts.
include ( "${CMAKE_SOURCE_DIR}/tests.cmake" )
include ( "${CMAKE_SOURCE_DIR}/FlexBisonPair.cmake" )

# ------------------------------------------------------------------------------
# SUPPORT FOR AUTOMATED TESTS
# ------------------------------------------------------------------------------

# After this command, tests may be added in the project using ADD_TEST command.
enable_testing()

# CTest is a testing tool distributed as a part of CMake.
include ( CTest )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build utility libraries.
add_subdirectory ( utilities )

# Build GUI.
add_subdirectory ( GUI )

# Build dev. hardware support software.
add_subdirectory ( HW )

# Build simulators.
add_subdirectory ( simulators )

# Build Moravia Script language support.
add_subdirectory ( moraviascript )

# Build compiler collection.
add_subdirectory ( compiler )

# Build disassemblers.
add_subdirectory ( disassembler )

# Add some data files to installation packages.
add_subdirectory ( pkgs )

# Build API and user documentation.
add_subdirectory ( docs )

# Build automated tests.
if ( NOT CMAKE_HOST_WIN32 )
    add_subdirectory ( tests )
endif()

# ------------------------------------------------------------------------------
# BUILD INSTALLATION PACKAGES
# ------------------------------------------------------------------------------

# http://www.cmake.org/Wiki/CMake:CPackPackageGenerator
# set ( CMAKE_INSTALL_PREFIX "/some/path" )

# Set which packages we want to build
if ( CMAKE_HOST_APPLE )
    # Apple MAC OS X
    set ( CPACK_GENERATOR "" )
elseif ( CMAKE_HOST_WIN32 )
    # Microsoft Windows
    set ( CPACK_GENERATOR "NSIS" )
else()
    # GNU/Linux and possibly other POSIX systems
    set ( CPACK_GENERATOR "DEB;RPM;STGZ" )
endif()

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
set ( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/pkgs/pkgdesc.txt" )
set ( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/pkgs/pkglicense.txt" )
set ( CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/pkgs/pkgreadme.txt" )
set ( CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}/pkgs/pkgwelcome.txt" )

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
set ( CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/pkgs/mmIDEicon.ico" )
# set ( CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/pkgs/uninstaller.ico" )
# set ( CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/pkgs/installer.bmp" )
set ( CPACK_NSIS_DISPLAY_NAME "mmIDE" )
set ( CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe'" )
# set ( CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_CURRENT_SOURCE_DIR}/pkgs/something.exe" )
set ( CPACK_NSIS_HELP_LINK "http://www.project-home-page.org" )
set ( CPACK_NSIS_URL_INFO_ABOUT "http://www.project-home-page.org" )
set ( CPACK_NSIS_CONTACT "support@abc.xyz" )
set ( CPACK_NSIS_MUI_FINISHPAGE_RUN "mmIDE.exe" )

# Include packaging driver provided by CMake (that means generate installation packages).
include ( CPack )
