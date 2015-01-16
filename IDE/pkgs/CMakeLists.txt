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
        # QtCLucene4 is required on windows but tends to CMake ignore that.
        list ( APPEND QT_LIBS_TO_INSTALL "${QT_LIBRARY_DIR}/QtCLucene4.dll" )
        # QHelp required this file in "sqldrivers" directory.
        message ( STATUS "qsqlite4.dll will be installed in ${TARGET_DIRECTORY}/sqldrivers/" )
        install ( FILES "${QT_PLUGINS_DIR}/sqldrivers/qsqlite4.dll" DESTINATION "${TARGET_DIRECTORY}/sqldrivers/" )
    endif()
    list ( APPEND LIB_FILES_TO_INSTALL ${QT_LIBS_TO_INSTALL} )

    # Include boost libraries.
    filter_release_only ( BOOST_LIBS_TO_INSTALL ${Boost_LIBRARIES} )
    list ( APPEND LIB_FILES_TO_INSTALL ${BOOST_LIBS_TO_INSTALL} )

    #
    ## On Linux we also need to add the whole tree of library dependencies.
    #
    if ( TARGET_OS STREQUAL "Linux" )
        set ( dep ${LIB_FILES_TO_INSTALL} )

        foreach ( lib ${LIB_FILES_TO_INSTALL} )
            execute_process ( COMMAND bash -c "ldd ${lib} | gawk '/=>/ {print($3)}'"
                              OUTPUT_VARIABLE ldd
                              ERROR_VARIABLE lddErrors )
            string ( REPLACE "\n" ";" ldd "${ldd}" )
            list ( APPEND dep "${ldd}" )
        endforeach()

        set ( LIB_FILES_TO_INSTALL "" )

        foreach ( file ${dep} )
            if ( EXISTS "${file}" )
                get_filename_component ( file ${file} REALPATH  )
                list ( APPEND LIB_FILES_TO_INSTALL ${file} )
            endif()
        endforeach()

        list ( REMOVE_DUPLICATES LIB_FILES_TO_INSTALL )
    endif()

    #
    ## On Linux we also need to create proper symlinks to the library files.
    #
    if ( TARGET_OS STREQUAL "Linux" )
        set ( symLinks "" )
        foreach ( lib ${LIB_FILES_TO_INSTALL} )
            get_filename_component ( filename ${lib} NAME )         # lib.so.m.n.p
            get_filename_component ( basename ${filename} NAME_WE ) # lib

            string ( REGEX MATCH "[0-9]+(\\.[0-9]+(\\.[0-9]+)?)?$" version "${filename}" )
            string ( REGEX MATCH "^[0-9]+" versionMayor "${version}" )
            string ( REGEX MATCH "^[0-9]+\\.[0-9]+" versionMinor "${version}}" )
            string ( REGEX MATCH "[0-9]+$" versionMinor "${versionMinor}" )
            string ( REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" versionPatch "${version}" )
            string ( REGEX MATCH "[0-9]+$" versionPatch "${versionPatch}" )

            set ( libName    "${basename}.so" )
            set ( libNameM   "${libName}.${versionMayor}" )
            set ( libNameMN  "${libNameM}.${versionMinor}" )
            set ( libNameMNP "${libNameMN}.${versionPatch}" )

            string ( REGEX REPLACE "\\.+$" "" libNameM ${libNameM} )
            string ( REGEX REPLACE "\\.+$" "" libNameMN ${libNameMN} )
            string ( REGEX REPLACE "\\.+$" "" libNameMNP ${libNameMNP} )

            foreach ( name ${libName} ${libNameM} ${libNameMN} ${libNameMNP} )
                if ( NOT filename STREQUAL name )
                    # message ( "lib='${lib}' | '${name}' --> '${filename}' | target='symlink-${basename}-${i}'" )
                    list ( APPEND symLinks ${name} )
                    if ( NOT EXISTS "${name}" )
                        execute_process ( COMMAND ln -fs ${filename} ${name}
                                          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
                    endif()
                endif()
            endforeach()
        endforeach()

        install ( FILES ${symLinks} DESTINATION ${TARGET_DIRECTORY} )
        get_directory_property ( files2clean DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} ADDITIONAL_MAKE_CLEAN_FILES )
        set_directory_properties ( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${files2clean};${symLinks}" )
    endif()

    # Set selected files to install.
#     message ( STATUS "3rd party libraries to install as files:" )
    foreach ( FILE_NAME ${LIB_FILES_TO_INSTALL} )
        install ( FILES ${FILE_NAME} DESTINATION ${TARGET_DIRECTORY} )
#         message ( STATUS "    * \"${FILE_NAME}\"" )
    endforeach()
endmacro()

# ------------------------------------------------------------------------------
# ADD SOME DATA FILES TO THE INSTALLATION PACKAGES.
# ------------------------------------------------------------------------------

if ( TARGET_OS STREQUAL "Linux" )
    #
    ## GNU/Linux and possibly other POSIX systems
    #

    set ( CPACK_SET_DESTDIR ON )
    set ( CPACK_GENERATOR "DEB" "RPM" "TBZ2" )
    set ( CPACK_PACKAGING_INSTALL_PREFIX "${INSTALL_DIR_PREFIX}" )

    if ( INDEPENDENT_PACKAGES )
        if ( TARGET_ARCH STREQUAL "x86_64" )
            set ( gccLibDirPrefix "x86_64" )
        elseif ( TARGET_ARCH STREQUAL "x86" )
            set ( gccLibDirPrefix "i686" )
        endif()
        file ( GLOB compilerDir "/usr/lib/gcc/${gccLibDirPrefix}*linux*/${gccVersion}/" )
        file ( GLOB compilerDir2 "/usr/lib/gcc/${gccLibDirPrefix}*linux*/${GCC_VERSION_MAYOR}.${GCC_VERSION_MINOR}*/" )
        INCLUDE_LIBRARIES_TO_INSTALLATION_PACKAGE ( "${INSTALL_DIR_LIB_3RD_PARTY}" "${compilerDir};${compilerDir2}" )
    endif()

    set ( CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE PARENT_SCOPE )
    set ( CMAKE_INSTALL_RPATH "${INSTALL_DIR_LIB_3RD_PARTY};${INSTALL_DIR_LIB}" PARENT_SCOPE )

    # Install .desktop spec and application icon
    install ( FILES mds-ide.png                   DESTINATION "${INSTALL_DIR_MAIN_SHARE}/pixmaps/" )
    install ( FILES mds-ide.desktop               DESTINATION "${INSTALL_DIR_MAIN_SHARE}/applications/" )
    install ( FILES mds-ide.appdata.xml           DESTINATION "${INSTALL_DIR_MAIN_SHARE}/appdata/" )
    install ( FILES application-x-mds-project.xml DESTINATION "${INSTALL_DIR_MAIN_SHARE}/mime/packages/" )

elseif ( TARGET_OS STREQUAL "Windows" )
    #
    ## Microsoft Windows
    #

    if ( TARGET_ARCH STREQUAL "x86_64" )
        set ( CPACK_GENERATOR "NSIS64" )
    else()
        set ( CPACK_GENERATOR "NSIS" )
    endif()

    get_filename_component ( COMPILER_DIR ${CMAKE_CXX_COMPILER} PATH )
    INCLUDE_LIBRARIES_TO_INSTALLATION_PACKAGE ( "${INSTALL_DIR_BIN}" "${COMPILER_DIR}" )

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
## General options.
#

set ( CPACK_PACKAGE_VENDOR "${COMPANY_NAME}" )
set ( CPACK_PACKAGE_NAME "${PRODUCT_NAME_SHORT}" )
set ( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/pkglicense.txt" )
set ( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/pkgdesc-${MDS_VARIANT}.txt" )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PRODUCT_NAME} ${MDS_GRADE} for ${MDS_TARGET} in ${MDS_VARIANT} version" )
set ( PACKAGE_DETAILED_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n MDS is a window-based software development platform combining robust and modern tools for development of embedded applications. This integrated development environment is intended to expedite the development process by adding advanced and reliable tools to your development arsenal. Our focus is efficiency and precision." )

file ( READ "${CMAKE_CURRENT_SOURCE_DIR}/pkglicense-${MDS_VARIANT}.txt" licenseTerms )
file ( WRITE "${CMAKE_CURRENT_SOURCE_DIR}/pkglicense.txt.in" "${licenseTerms}" )
configure_file ( "${CMAKE_CURRENT_SOURCE_DIR}/pkglicense.txt.in" "${CMAKE_CURRENT_SOURCE_DIR}/pkglicense.txt" @ONLY )
get_directory_property ( files2clean DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} ADDITIONAL_MAKE_CLEAN_FILES )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES
                           "${files2clean};${CMAKE_CURRENT_SOURCE_DIR}/pkglicense.txt;${CMAKE_CURRENT_SOURCE_DIR}/pkglicense.txt.in" )

#
## Package version.
#

set ( CPACK_PACKAGE_VERSION ${PRODUCT_VERSION} )
set ( CPACK_PACKAGE_VERSION_MAJOR ${PRODUCT_VERSION_MAJOR} )
set ( CPACK_PACKAGE_VERSION_MINOR ${PRODUCT_VERSION_MINOR} )
set ( CPACK_PACKAGE_VERSION_PATCH ${PRODUCT_VERSION_PATCH} )

#
## Package file name.
#

set ( MDS_TARGET_PKG "-${MDS_TARGET}" )
if ( MDS_VARIANT STREQUAL "Commercial" )
    set ( MDS_VARIANT_PKG "" )
else()
    set ( MDS_VARIANT_PKG "-${MDS_VARIANT}" )
endif()
if ( MDS_GRADE STREQUAL "Professional" )
    set ( MDS_GRADE_PKG "-Profi" )
else()
    set ( MDS_GRADE_PKG "-${MDS_GRADE}" )
endif()
set ( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}${MDS_GRADE_PKG}${MDS_TARGET_PKG}${MDS_VARIANT_PKG}-${CPACK_PACKAGE_VERSION}-${TARGET_OS}.${TARGET_ARCH}" )

#
## Options for the "DEB" generator.
#

if ( TARGET_ARCH STREQUAL "x86_64" )
    set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" )
elseif ( TARGET_ARCH STREQUAL "x86" )
    set ( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386" )
endif()
set ( CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= ${GLIBC_MIN_VER})" )
if ( NOT INDEPENDENT_PACKAGES )
    set ( CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= ${GLIBC_MIN_VER}), libqt4-core (>= ${QT_MIN_VER}), libqt4-gui (>= ${QT_MIN_VER}), libqt4-xml (>= ${QT_MIN_VER}), libqt4-help (>= ${QT_MIN_VER}), libboost-regex (= ${BOOST_MIN_VER}), libboost-filesystem (= ${BOOST_MIN_VER}), libboost-system (= ${BOOST_MIN_VER}), libgcc1 (>= ${GCC_MIN_VER}), libstdc++6 (>= ${GCC_MIN_VER})" )
endif()
set ( CPACK_DEBIAN_PACKAGE_RECOMMENDS "" )
set ( CPACK_DEBIAN_PACKAGE_SUGGESTS "" )
set ( CPACK_PACKAGE_CONTACT "${COMPANY_NAME}; ${COMPANY_ADDRESS}" )
set ( CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.moravia-microsystems.com" )
set ( CPACK_DEBIAN_PACKAGE_SECTION "Development" )
set ( CPACK_DEBIAN_PACKAGE_DESCRIPTION "${PACKAGE_DETAILED_DESCRIPTION}" )

#
## Options for the "RPM" generator.
#

set ( CPACK_RPM_PACKAGE_REQUIRES "glibc >= ${GLIBC_MIN_VER}" )
if ( NOT INDEPENDENT_PACKAGES )
    set ( CPACK_RPM_PACKAGE_REQUIRES "glibc >= ${GLIBC_MIN_VER}, qt-x11 >= ${QT_MIN_VER}, boost-filesystem = ${BOOST_MIN_VER}, boost-system = ${BOOST_MIN_VER}, libboost-regex = ${BOOST_MIN_VER}, libgcc >= ${GCC_MIN_VER}, libstdc++ >= ${GCC_MIN_VER} " )
endif()
set ( CPACK_RPM_PACKAGE_AUTOREQPROV OFF )
set ( CPACK_RPM_PACKAGE_RELOCATABLE OFF )
set ( CPACK_RPM_PACKAGE_SUGGESTS "" )
set ( CPACK_RPM_PACKAGE_URL "http://www.moravia-microsystems.com" )
set ( CPACK_RPM_PACKAGE_GROUP "Development/Tools/IDE" )
set ( CPACK_RPM_PACKAGE_LICENSE "Moravia Microsystems license" )
set ( CPACK_RPM_PACKAGE_DESCRIPTION "${PACKAGE_DETAILED_DESCRIPTION}" )

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
      "${INSTALL_DIR_DOC}/${MDS_MANUAL_PDF}" "${PRODUCT_NAME} user documentation"
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
