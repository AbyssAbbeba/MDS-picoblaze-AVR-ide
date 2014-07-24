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
# MACRO DEFINITIONS
# ------------------------------------------------------------------------------

## Make list o libraries to install, e.g. include optimized libraries only.
 # If it is not possible to determine which libraries are optimized and which
 # are debug, all libraries specified in the LIBRARIES list will be added to
 # LIBRARIES_TO_INSTALL.
 #
 # parameter LIBRARIES_TO_INSTALL : The resulting list of libraries.
 # parameter ...                  : The original list of libraries.
macro ( FILTER_RELEASE_ONLY LIBRARIES_TO_INSTALL )
    # Clear resulting list of libraries
    set ( ${LIBRARIES_TO_INSTALL} "" )

    # Flag: insert ${lib} into the result.
    set ( insertThis FALSE )

    # Iterate over the given list of libraries.
    foreach ( lib ${ARGN} )
        if ( insertThis )
            # Include ${lib} in the result.
            set ( insertThis FALSE )
            list ( APPEND ${LIBRARIES_TO_INSTALL} "${lib}" )
        else()
            # Determine whether the next ${lib} is optimized variant or not.
            if ( "${lib}" STREQUAL "optimized" )
                set ( insertThis TRUE )
            else()
                set ( insertThis FALSE )
            endif()
        endif()
    endforeach()

    # If the result if zero length, include all libraries.
    list ( LENGTH ${LIBRARIES_TO_INSTALL} length )
    if ( length EQUAL 0 )
        set ( ${LIBRARIES_TO_INSTALL} ${ARGN} )
    endif()
endmacro()

##
 #
macro ( INCLUDE_LIBRARIES_TO_INSTALLATION_PACKAGE TARGET_DIRECTORY COMPILER_DIRECTORY )

    set ( LIB_FILES_TO_INSTALL "" )

    # Include libGCC.
    find_library ( LIB_GCC
                   NAMES gcc_s_sjlj-1 gcc_s_seh-1 libgcc_s_dw2-1 gcc_s
                   DOC "GCC library"
                   PATHS ${COMPILER_DIRECTORY}
                   NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH )
    if ( NOT LIB_GCC )
        message ( SEND_ERROR "Unable to find gcc library." )
    endif()
    list ( APPEND LIB_FILES_TO_INSTALL ${LIB_GCC} )

    # Include C++ standard library.
    find_library ( LIB_STD_CPP
                   NAMES stdc++-6 stdc++
                   DOC "stdc++ library"
                   PATHS ${COMPILER_DIRECTORY}
                   NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH )
    if ( NOT LIB_STD_CPP )
        message ( SEND_ERROR "Unable to find stdc++ library." )
    endif()
    list ( APPEND LIB_FILES_TO_INSTALL ${LIB_STD_CPP} )

    # Include Qt framework libraries.
    filter_release_only ( QT_LIBS_TO_INSTALL ${QT_LIBRARIES} )
    if ( TARGET_OS STREQUAL "Windows" )
        list ( APPEND QT_LIBS_TO_INSTALL "${QT_LIBRARY_DIR}/QtCLucene4.dll" )
    endif()
    list ( APPEND LIB_FILES_TO_INSTALL ${QT_LIBS_TO_INSTALL} )

    # Include boost libraries.
    filter_release_only ( BOOST_LIBS_TO_INSTALL ${Boost_LIBRARIES} )
    list ( APPEND LIB_FILES_TO_INSTALL ${BOOST_LIBS_TO_INSTALL} )

    message ( STATUS "3rd party libraries to install as files:" )
    foreach ( FILE_NAME ${LIB_FILES_TO_INSTALL} )
        get_filename_component ( REAL_FILE ${FILE_NAME} REALPATH  )
        install ( FILES ${REAL_FILE} DESTINATION ${TARGET_DIRECTORY} )
        message ( STATUS "    * \"${REAL_FILE}\"" )
    endforeach()
endmacro()

# ------------------------------------------------------------------------------
# ADD SOME DATA FILES TO THE INSTALLATION PACKAGES.
# ------------------------------------------------------------------------------

if ( TARGET_OS STREQUAL "Linux" )
    #
    ## GNU/Linux and possibly other POSIX systems
    #

    set ( CPACK_GENERATOR "DEB" "RPM" "TBZ2" )

    # Install .desktop spec and application icon
    install ( FILES mds-ide.png                   DESTINATION "${INSTALL_DIR_SHARE}/pixmaps/" )
    install ( FILES mds-ide.desktop               DESTINATION "${INSTALL_DIR_SHARE}/applications/" )
    install ( FILES application-x-mds-project.xml DESTINATION "${INSTALL_DIR_SHARE}/mime/packages/" )

    if ( INDEPENDENT_PACKAGES )
        set ( CPACK_PACKAGING_INSTALL_PREFIX "/opt" )
        file ( GLOB COMPILER_DIR "/usr/lib/gcc/x86_64*linux*/4.8.2/" )
        INCLUDE_LIBRARIES_TO_INSTALLATION_PACKAGE ( ${INSTALL_DIR_LIB} ${COMPILER_DIR} )
    else()
        set ( CPACK_PACKAGING_INSTALL_PREFIX "/usr" )
    endif()

    set ( CMAKE_INSTALL_RPATH "${CPACK_PACKAGING_INSTALL_PREFIX}/${INSTALL_DIR_LIB}" )
    set ( CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE )

elseif ( TARGET_OS STREQUAL "Windows" )
    #
    ## Microsoft Windows
    #

    set ( CPACK_GENERATOR "NSIS" )

    get_filename_component ( COMPILER_DIR ${CMAKE_CXX_COMPILER} PATH )
    INCLUDE_LIBRARIES_TO_INSTALLATION_PACKAGE ( ${INSTALL_DIR_BIN} ${COMPILER_DIR} )

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
set ( CPACK_PACKAGE_VENDOR "${COMPANY_NAME}" )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PRODUCT_NAME}" )
set ( MY_CPACK_PACKAGE_DETAILED_DESC "${PRODUCT_NAME} by ${COMPANY_NAME}" )
set ( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${TARGET_OS}.${TARGET_ARCH}" )
string ( REGEX REPLACE " " "" CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME} )

#
## Resources.
#

set ( CPACK_RESOURCE_FILE_README     "${CMAKE_CURRENT_SOURCE_DIR}/pkgreadme.txt" )
set ( CPACK_RESOURCE_FILE_WELCOME    "${CMAKE_CURRENT_SOURCE_DIR}/pkgwelcome.txt" )
set ( CPACK_RESOURCE_FILE_LICENSE    "${CMAKE_CURRENT_SOURCE_DIR}/pkglicense.txt" )
set ( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/pkgdesc.txt" )

#
## Options for the "DEB" generator.
#

if ( TARGET_ARCH STREQUAL "x86_64" )
    set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
elseif ( TARGET_ARCH STREQUAL "x86" )
    set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386" )
endif()
set ( CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-core (>= ${QT_MIN_VER}), libqt4-gui (>= ${QT_MIN_VER}), libqt4-xml (>= ${QT_MIN_VER}), libqt4-help (>= ${QT_MIN_VER}), libboost-regex (= ${BOOST_MIN_VER}), libboost-filesystem (= ${BOOST_MIN_VER}), libboost-system (= ${BOOST_MIN_VER}), libgcc1 (>= ${GCC_MIN_VER}), libstdc++6 (>= ${GCC_MIN_VER})" )
set ( CPACK_DEBIAN_PACKAGE_RECOMMENDS "" )
set ( CPACK_DEBIAN_PACKAGE_SUGGESTS "" )
set ( CPACK_PACKAGE_CONTACT "${COMPANY_NAME}; ${COMPANY_ADDRESS}" )
set ( CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.moravia-microsystems.com" )
set ( CPACK_DEBIAN_PACKAGE_SECTION "Development" )
set ( CPACK_DEBIAN_PACKAGE_DESCRIPTION "\t${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n\t\t${MY_CPACK_PACKAGE_DETAILED_DESC}" )

#
## Options for the "RPM" generator.
#

set ( CPACK_RPM_PACKAGE_REQUIRES "qt-x11 >= ${QT_MIN_VER}, boost-filesystem = ${BOOST_MIN_VER}, boost-system = ${BOOST_MIN_VER}, libboost-regex = ${BOOST_MIN_VER}, libgcc >= ${GCC_MIN_VER}, libstdc++ >= ${GCC_MIN_VER} " )
set ( CPACK_RPM_PACKAGE_SUGGESTS "" )
set ( CPACK_RPM_PACKAGE_URL "http://www.moravia-microsystems.com" )
set ( CPACK_RPM_PACKAGE_GROUP "Development/Tools/IDE" )
set ( CPACK_RPM_PACKAGE_LICENSE "<package license>" )


#
## Options for the "NSIS" generator (Windows).
#

# Coverts a sane path (like "/x/y/z") into an MS-DOS "crap" (like "\\\\x\\\\y\\\\z").
macro ( SLASH_TO_QUADRUPLE_BACKSLACH Variable )
    string ( REGEX REPLACE "/" "\\\\\\\\" ${Variable} "${${Variable}}" )
    string ( REGEX REPLACE "\\|" "/" ${Variable} "${${Variable}}" )
endmacro()

 # Installation directory on the target system.
set ( CPACK_PACKAGE_INSTALL_DIRECTORY "Moravia Microsystems/${PRODUCT_NAME}" )
slash_to_quadruple_backslach ( CPACK_PACKAGE_INSTALL_DIRECTORY )
  # A branding image that will be displayed inside the installer
set ( CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/header.bmp" )
slash_to_quadruple_backslach ( CPACK_PACKAGE_ICON )
 # Various images.
set ( CPACK_NSIS_INSTALLER_MUI_ICON_CODE "
      ; Installer program icon.
      !define MUI_ICON \\\"${CMAKE_CURRENT_SOURCE_DIR}/installer.ico\\\"

      ; Uninstaller program icon.
      !define MUI_UNICON \\\"${CMAKE_CURRENT_SOURCE_DIR}/uninstaller.ico\\\"

      ; Sidebar image.
      !define MUI_WELCOMEFINISHPAGE_BITMAP \\\"${CMAKE_CURRENT_SOURCE_DIR}/sidebar.bmp\\\"
" )
slash_to_quadruple_backslach ( CPACK_NSIS_INSTALLER_MUI_ICON_CODE )
 # Set the compression algorithm used to compress files/data in the installer
set ( CPACK_NSIS_COMPRESSOR "/SOLID /FINAL lzma" )
 # Ask about uninstalling previous versions first.
set ( CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON )
 # Modify PATH toggle.
set ( CPACK_NSIS_MODIFY_PATH ON )
 # The display name string that appears in the Windows Add/Remove Program control panel.
set ( CPACK_NSIS_DISPLAY_NAME "${PRODUCT_NAME}" )
 # The title displayed at the top of the installer.
set ( CPACK_NSIS_PACKAGE_NAME "${PRODUCT_NAME}" )
 # A path to the executable that contains the app. icon.
set ( CPACK_NSIS_INSTALLED_ICON_NAME "${INSTALL_DIR_BIN}/mds-ide.exe" )
slash_to_quadruple_backslach ( CPACK_NSIS_INSTALLED_ICON_NAME )
 # URL to a web site providing assistance in installing your application.
set ( CPACK_NSIS_HELP_LINK "http://www.moravia-microsystems.com" )
 # URL to a web site providing more information about your application.
set ( CPACK_NSIS_URL_INFO_ABOUT "http://www.moravia-microsystems.com/about" )
 # Contact information for questions and comments about the installation process.
set ( CPACK_NSIS_CONTACT "support@moravia-microsystems.com" )
 # Additional NSIS commands to uninstall start menu shortcuts.
set ( CPACK_NSIS_DELETE_ICONS_EXTRA "" )
 # Specify an executable to add an option to run on the finish page of the NSIS installer
set ( CPACK_NSIS_MUI_FINISHPAGE_RUN "mds-ide.exe" )
 # Registry key used when installing this project.
SET ( CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Moravia Microsystems/${PRODUCT_NAME}" )
slash_to_quadruple_backslach ( CPACK_PACKAGE_INSTALL_REGISTRY_KEY )
 # Additional NSIS commands for creating start menu shortcuts.
set ( CPACK_NSIS_CREATE_ICONS_EXTRA "
      CreateShortCut \\\"$DESKTOP/${PRODUCT_NAME}.lnk\\\" \\\"$INSTDIR/${INSTALL_DIR_BIN}/mds-ide.exe\\\"
      CreateShortCut \\\"$QUICKLAUNCH/${PRODUCT_NAME}.lnk\\\" \\\"$INSTDIR/${INSTALL_DIR_BIN}/mds-ide.exe\\\"
      CreateShortCut '\$SMPROGRAMS/$STARTMENU_FOLDER/${PRODUCT_NAME}.lnk' '\$INSTDIR/${INSTALL_DIR_BIN}/mds-ide.exe'
" )
slash_to_quadruple_backslach ( CPACK_NSIS_CREATE_ICONS_EXTRA )
 # Specify links in [application] menu.
set ( CPACK_NSIS_MENU_LINKS
      "${INSTALL_DIR_DOC}/${MANUAL_PDF}" "${PRODUCT_NAME} user documentation"
      "http:||www.moravia-microsystems.com" "Moravia Microsystems Web Site" )
slash_to_quadruple_backslach ( CPACK_NSIS_MENU_LINKS )
 # Extra NSIS commands that will be added to the beginning of the install Section
set ( CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "" )
slash_to_quadruple_backslach ( CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS )
 # Extra NSIS commands that will be added to the end of the install Section
set ( CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    ; Associate .mds-project file extension with the mds-ide.exe executable.
    !include \\\"${CMAKE_CURRENT_SOURCE_DIR}/FileAssociation.nsh\\\"
    \\\${registerExtension} '\$INSTDIR/${INSTALL_DIR_BIN}/mds-ide.exe' '.mds-project' '${PRODUCT_NAME} Project File'

    ; Create new directory for storing MDS configuration files (with MS-DOS/Windows attribute HIDDEN set on).
    CreateDirectory \\\"$PROFILE/.mds\\\"
    SetFileAttributes \\\"$PROFILE/.mds\\\" HIDDEN
" )
slash_to_quadruple_backslach ( CPACK_NSIS_EXTRA_INSTALL_COMMANDS )
 # Extra NSIS commands that will be added to the uninstall Section
set ( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    Delete \\\"$DESKTOP/${PRODUCT_NAME}.lnk\\\"
    \\\${unregisterExtension} '.mds-project' '${PRODUCT_NAME} Project File'
" )
slash_to_quadruple_backslach ( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS )

# Include packaging driver provided by CMake.
include ( "CPack" )
