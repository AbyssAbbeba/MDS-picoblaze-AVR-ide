# ==============================================================================
#
# Environment for building installation packages.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( pkgs )

# ------------------------------------------------------------------------------
# ADD SOME DATA FILES TO INSTALLATION PACKAGES.
# ------------------------------------------------------------------------------

if ( CMAKE_HOST_APPLE )
    #
    ## Apple MAC OS X
    #

elseif ( CMAKE_HOST_WIN32 )
    #
    ## Microsoft Windows
    #

    FILE ( GLOB libgcc "C:/MinGW/bin/libgcc*.dll" )
    FILE ( GLOB libstdc "C:/MinGW/bin/libstdc*.dll" )
    FILE ( GLOB libmingwm "C:/MinGW/bin/mingwm*.dll" )
    FILE ( GLOB libqtcore "C:/QtSDK/Desktop/Qt/4.8.0/mingw/bin/QtCored*.dll" )
    FILE ( GLOB libqtgui "C:/QtSDK/Desktop/Qt/4.8.0/mingw/bin/QtGuid*.dll" )
    FILE ( GLOB libqtxml "C:/QtSDK/Desktop/Qt/4.8.0/mingw/bin/QtXmld*.dll" )
    INSTALL ( FILES ${libgcc} ${libstdc} ${libmingwm} ${libqtcore} ${libqtgui} ${libqtxml}
              DESTINATION bin )

else()
    #
    ## GNU/Linux and possibly other POSIX systems
    #

    # Install .desktop spec and application icon
    install ( FILES mmIDEicon.png DESTINATION share/pixmaps/ )
    install ( FILES mmIDE.desktop DESTINATION share/applications/ )
    install ( FILES application-x-mmIDE.xml DESTINATION share/mime/packages/ )

endif()

# ------------------------------------------------------------------------------
# BUILD INSTALLATION PACKAGES
# ------------------------------------------------------------------------------

# http://www.cmake.org/Wiki/CMake:CPackPackageGenerator
set ( CMAKE_INSTALL_PREFIX "/" )

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
set ( CPACK_PACKAGE_VENDOR "Moravia Microsystems, s.r.o." )
set ( CPACK_PACKAGE_VERSION_MAJOR 1 )
set ( CPACK_PACKAGE_VERSION_MINOR 0 )
set ( CPACK_PACKAGE_VERSION_PATCH 0 )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Multitarget Development System" )
set ( MY_CPACK_PACKAGE_DETAILED_DESC "<MY_CPACK_PACKAGE_DETAILED_DESC> ... ... ... ... ... ... ... ... ... ... " )

# Resource file (see the CPack manual for details)
set ( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/pkgs/pkgdesc.txt" )
set ( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/pkgs/pkglicense.txt" )
set ( CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/pkgs/pkgreadme.txt" )
set ( CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/pkgs/pkgwelcome.txt" )

# Options for the "DEB" generator
if ( TARGET_ARCH STREQUAL "x86_64" )
    set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
elseif ( TARGET_ARCH STREQUAL "x86" )
    set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386" )
endif()
set ( CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-gui (>= 4.8), libqt4-xml (>= 4.8)" )
set ( CPACK_DEBIAN_PACKAGE_RECOMMENDS "" )
set ( CPACK_DEBIAN_PACKAGE_SUGGESTS "" )
set ( CPACK_PACKAGE_CONTACT "Moravia Microsystems, s.r.o.; trida Kpt. Jarose 1946/35, 602 00 Brno, CZ, European Union" )
set ( CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.moravia-microsystems.eu" )
set ( CPACK_DEBIAN_PACKAGE_SECTION "Development" )
set ( CPACK_DEBIAN_PACKAGE_DESCRIPTION "\t${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n\t\t${MY_CPACK_PACKAGE_DETAILED_DESC}" )

# Options for the "RPM" generator
set ( CPACK_RPM_PACKAGE_REQUIRES "qt >= 4.8" )
set ( CPACK_RPM_PACKAGE_SUGGESTS "" )
set ( CPACK_RPM_PACKAGE_URL "http://www.moravia-microsystems.eu" )
set ( CPACK_RPM_PACKAGE_GROUP "Development/Tools/IDE" )
set ( CPACK_RPM_PACKAGE_LICENSE "<package license>" )

# Options for the "NSIS" generator
set ( CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/pkgs/mmIDEicon.ico" )
# set ( CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/pkgs/uninstaller.ico" )
# set ( CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/pkgs/installer.bmp" )
set ( CPACK_NSIS_DISPLAY_NAME "mmIDE" )
set ( CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe'" )
# set ( CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_SOURCE_DIR}/pkgs/something.exe" )
set ( CPACK_NSIS_HELP_LINK "http://www.moravia-microsystems.eu" )
set ( CPACK_NSIS_URL_INFO_ABOUT "http://www.moravia-microsystems.eu" )
set ( CPACK_NSIS_CONTACT "support@moravia-microsystems.eu" )
set ( CPACK_NSIS_MUI_FINISHPAGE_RUN "mmIDE.exe" )

# Include packaging driver provided by CMake (that means generate installation packages).
include ( CPack )
