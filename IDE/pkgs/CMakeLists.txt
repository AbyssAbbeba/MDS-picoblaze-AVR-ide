# ==============================================================================
#
# Environment for building installation packages.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( MultitargetDevelopmentSystem )

# ------------------------------------------------------------------------------
# ADD SOME DATA FILES TO THE INSTALLATION PACKAGES.
# ------------------------------------------------------------------------------

if ( TARGET_OS STREQUAL "Linux" )
    #
    ## GNU/Linux and possibly other POSIX systems
    #

    set ( CPACK_GENERATOR "DEB" "RPM" "TBZ2" )
    set ( CPACK_PACKAGING_INSTALL_PREFIX "/usr/local" )

    # Install .desktop spec and application icon
    install ( FILES mds-ide.png           DESTINATION "${INSTALL_DIR_SHARE}/pixmaps/" )
    install ( FILES mds-ide.desktop       DESTINATION "${INSTALL_DIR_SHARE}/applications/" )
    install ( FILES application-x-mds.xml DESTINATION "${INSTALL_DIR_SHARE}/mime/packages/" )

elseif ( TARGET_OS STREQUAL "Windows" )
    #
    ## Microsoft Windows
    #

    set ( CPACK_GENERATOR "NSIS" )

    get_filename_component ( COMPILER_DIRECTORY ${CMAKE_CXX_COMPILER} PATH )

    find_library ( LIB_GCC
                   NAMES libgcc_s_sjlj-1.dll
                   PATHS ${COMPILER_DIRECTORY}
                   NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH )

    find_library ( LIB_STD_CPP
                   NAMES libstdc++-6.dll
                   PATHS ${COMPILER_DIRECTORY}
                   NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH )

    INSTALL ( FILES ${LIB_GCC}         DESTINATION ${INSTALL_DIR_BIN} ) # Include libGCC.
    INSTALL ( FILES ${LIB_STD_CPP}     DESTINATION ${INSTALL_DIR_BIN} ) # Include C++ standard library.
    INSTALL ( FILES ${QT_LIBRARIES}    DESTINATION ${INSTALL_DIR_BIN} ) # Include Qt framework libraries.
    INSTALL ( FILES ${Boost_LIBRARIES} DESTINATION ${INSTALL_DIR_BIN} ) # Include boost libraries.

elseif ( TARGET_OS STREQUAL "Darwin" )
    #
    ## Apple MAC OS X
    #

endif()

# ------------------------------------------------------------------------------
# BUILD INSTALLATION PACKAGES
# ( http://www.cmake.org/Wiki/CMake:CPackPackageGenerator )
# ------------------------------------------------------------------------------

#
## Package version.
#

set ( CPACK_PACKAGE_VERSION ${PRODUCT_VERSION} )
string ( REGEX REPLACE "\\." ";" versionList ${CPACK_PACKAGE_VERSION} )
list ( LENGTH versionList listLength )
list ( GET versionList 0 CPACK_PACKAGE_VERSION_MAJOR )
if ( listLength GREATER 1 )
    list ( GET versionList 1 CPACK_PACKAGE_VERSION_MINOR )
else()
    set ( CPACK_PACKAGE_VERSION_MINOR 0 )
endif()
if ( listLength GREATER 2 )
    list ( GET versionList 2 CPACK_PACKAGE_VERSION_PATCH )
else()
    set ( CPACK_PACKAGE_VERSION_PATCH 0 )
endif()

#
## General options.
#

set ( CPACK_PACKAGE_NAME "${PRODUCT_NAME}" )
set ( CPACK_PACKAGE_VENDOR "Moravia Microsystems, s.r.o." )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PRODUCT_NAME}" )
set ( MY_CPACK_PACKAGE_DETAILED_DESC "<MY_CPACK_PACKAGE_DETAILED_DESC> ... ... ... ... ... ... ... ... ... ... " )
set ( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${TARGET_OS}.${TARGET_ARCH}" )
string ( REGEX REPLACE " " "" CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME} )
set ( CPACK_PACKAGE_VERSION_MAJOR 1 )
set ( CPACK_PACKAGE_VERSION_MINOR 0 )
set ( CPACK_PACKAGE_VERSION_PATCH 0 )

#
## Resources.
#

set ( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/pkgs/pkgdesc.txt" )
set ( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/pkgs/pkglicense.txt" )
set ( CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/pkgs/pkgreadme.txt" )
set ( CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/pkgs/pkgwelcome.txt" )

#
## Options for the "DEB" generator.
#

if ( TARGET_ARCH STREQUAL "x86_64" )
    set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
elseif ( TARGET_ARCH STREQUAL "x86" )
    set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i686" )
endif()
set ( CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-core (>= ${QT_MIN_VER}), libqt4-gui (>= ${QT_MIN_VER}), libqt4-xml (>= ${QT_MIN_VER}), libqt4-help (>= ${QT_MIN_VER}), libboost-regex1.48.0, libboost-filesystem1.48.0, libboost-system1.48.0, libgcc1 (>= ${GCC_MIN_VER}), libstdc++6 (>= ${GCC_MIN_VER})" )
set ( CPACK_DEBIAN_PACKAGE_RECOMMENDS "" )
set ( CPACK_DEBIAN_PACKAGE_SUGGESTS "" )
set ( CPACK_PACKAGE_CONTACT "Moravia Microsystems, s.r.o.; trida Kpt. Jarose 1946/35, 602 00 Brno, CZ, European Union" )
set ( CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.moravia-microsystems.com" )
set ( CPACK_DEBIAN_PACKAGE_SECTION "Development" )
set ( CPACK_DEBIAN_PACKAGE_DESCRIPTION "\t${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n\t\t${MY_CPACK_PACKAGE_DETAILED_DESC}" )

#
## Options for the "RPM" generator.
#

set ( CPACK_RPM_PACKAGE_REQUIRES "" )
set ( CPACK_RPM_PACKAGE_SUGGESTS "" )
set ( CPACK_RPM_PACKAGE_URL "http://www.moravia-microsystems.com" )
set ( CPACK_RPM_PACKAGE_GROUP "Development/Tools/IDE" )
set ( CPACK_RPM_PACKAGE_LICENSE "<package license>" )


#
## Options for the "NSIS" generator (Windows).
#

 # Installation directory on the target system.
set ( CPACK_PACKAGE_INSTALL_DIRECTORY "Moravia Microsystems\\\\${PRODUCT_NAME}" )
  # A branding image that will be displayed inside the installer
set ( CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/pkgs/header.bmp" )
string ( REGEX REPLACE "/" "\\\\\\\\" CPACK_PACKAGE_ICON ${CPACK_PACKAGE_ICON} )
 # Various images.
set ( CPACK_NSIS_INSTALLER_MUI_ICON_CODE "
      \; Installer program icon
      !define MUI_ICON \\\"${CMAKE_SOURCE_DIR}/pkgs/installer.ico\\\"

      \; Uninstaller program icon.
      !define CPACK_NSIS_MUI_UNIICON \\\"${CMAKE_SOURCE_DIR}/pkgs/uninstaller.ico\\\"

      \; Sidebar image.
      !define MUI_WELCOMEFINISHPAGE_BITMAP \\\"${CMAKE_SOURCE_DIR}/pkgs/sidebar.bmp\\\"
" )
string ( REGEX REPLACE "/" "\\\\\\\\" CPACK_NSIS_INSTALLER_MUI_ICON_CODE ${CPACK_NSIS_INSTALLER_MUI_ICON_CODE} )
 # Set the compression algorithm used to compress files/data in the installer
set ( CPACK_NSIS_COMPRESSOR "/SOLID /FINAL lzma" )
 # Ask about uninstalling previous versions first.
set ( CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON )
 # Modify PATH toggle.
set ( CPACK_NSIS_MODIFY_PATH OFF )
 # The display name string that appears in the Windows Add/Remove Program control panel.
set ( CPACK_NSIS_DISPLAY_NAME "${PRODUCT_NAME}" )
 # The title displayed at the top of the installer.
set ( CPACK_NSIS_PACKAGE_NAME "${PRODUCT_NAME}" )
 # A path to the executable that contains the app. icon.
set ( CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\mds-ide.exe" )
 # URL to a web site providing assistance in installing your application.
set ( CPACK_NSIS_HELP_LINK "http://www.moravia-microsystems.com" )
 # URL to a web site providing more information about your application.
set ( CPACK_NSIS_URL_INFO_ABOUT "http://www.moravia-microsystems.com/about" )
 # Contact information for questions and comments about the installation process.
set ( CPACK_NSIS_CONTACT "support@moravia-microsystems.com" )
 # Additional NSIS commands for creating start menu shortcuts.
set ( CPACK_NSIS_CREATE_ICONS_EXTRA "
      CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PRODUCT_NAME}.lnk' '\$INSTDIR\\\\bin\\\\mds-ide.exe'
      CreateShortCut \\\"$DESKTOP\\\\${PRODUCT_NAME}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\mds-ide.exe\\\"
" )
 # Additional NSIS commands to uninstall start menu shortcuts.
set ( CPACK_NSIS_DELETE_ICONS_EXTRA "" )
 # Specify an executable to add an option to run on the finish page of the NSIS installer
set ( CPACK_NSIS_MUI_FINISHPAGE_RUN "mds-ide.exe" )
 # Specify links in [application] menu.
set ( CPACK_NSIS_MENU_LINKS
      "doc/cmake-@CMake_VERSION_MAJOR@.@CMake_VERSION_MINOR@/cmake.html"
      "Moravia Microsystems, s.r.o."
      "http://www.moravia-microsystems.com" "${PRODUCT_NAME} Web Site" )
 # Extra NSIS commands that will be added to the beginning of the install Section
set ( CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "" )
 # Extra NSIS commands that will be added to the end of the install Section
set ( CPACK_NSIS_EXTRA_INSTALL_COMMANDS "" )
 # Extra NSIS commands that will be added to the uninstall Section
set ( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete \\\"$DESKTOP\\\\${PRODUCT_NAME}.lnk\\\"" )
 # Registry key used when installing this project.
SET ( CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Moravia Microsystems\\\\${PRODUCT_NAME}" )

# Include packaging driver provided by CMake.
include ( CPack )
