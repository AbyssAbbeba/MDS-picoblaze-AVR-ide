# ==============================================================================
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# author: Martin Ošmera <martin.osmera@moravia-microsystems.com>
#
# ==============================================================================

macro ( WINDRES_RC )
    set ( optionalOneValKeys ICON YEARS FILEOS FILETYPE )
    set ( mandatoryOneValKeys OUTPUT_VAR TARGET FILE_DESCRIPTION FILE_VERSION COMPANY_NAME PRODUCT_NAME PRODUCT_VERSION)
    cmake_parse_arguments ( WINDRES_RC "" "${optionalOneValKeys};${mandatoryOneValKeys}" "" ${ARGN} )

    foreach ( key ${mandatoryKeys} )
        if ( WINDRES_RC_${key} STREQUAL "" )
            message ( SEND_ERROR "Mandatory option ${key} is not set." )
        endif()
    endforeach()

    if ( WINDRES_RC_FILETYPE STREQUAL "DLL" )
        set ( WINDRES_RC_FILETYPE "VFT_DLL" )
        set ( OriginalFilename "${CMAKE_SHARED_LIBRARY_PREFIX}${WINDRES_RC_TARGET}${CMAKE_SHARED_LIBRARY_SUFFIX}" )
    elseif ( WINDRES_RC_FILETYPE STREQUAL "APP" )
        set ( WINDRES_RC_FILETYPE "VFT_APP" )
        set ( OriginalFilename "${WINDRES_RC_TARGET}${CMAKE_EXECUTABLE_SUFFIX}" )
    else()
        message ( SEND_ERROR "Option value of FILETYPE = `${WINDRES_RC_FILETYPE}' was not understood (e {DLL, APP})." )
    endif()

    set ( rcFile "" )

    if ( TARGET_OS STREQUAL "Windows" )
        windres_version_to_csv ( fv ${WINDRES_RC_FILE_VERSION} )
        windres_version_to_csv ( pv ${WINDRES_RC_PRODUCT_VERSION} )

        set ( rcFile "${CMAKE_CURRENT_SOURCE_DIR}/${WINDRES_RC_TARGET}.rc" )

        get_directory_property ( cleanFiles ADDITIONAL_MAKE_CLEAN_FILES )
        set_directory_properties ( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${rcFile};${cleanFiles}" )

        file ( WRITE  ${rcFile} "#include <winver.h>\n" )
        if ( NOT "${WINDRES_RC_ICON}" STREQUAL "" )
            get_filename_component ( IconFile ${WINDRES_RC_ICON} ABSOLUTE )
            file ( APPEND ${rcFile} "id ICON \"${IconFile}\"\n" )
            file ( APPEND ${rcFile} "\n" )
        endif()
        file ( APPEND ${rcFile} "1 VERSIONINFO\n" )
        file ( APPEND ${rcFile} "FILEVERSION     ${fv0},${fv1},${fv2},${fv3}\n" )
        file ( APPEND ${rcFile} "PRODUCTVERSION  ${pv0},${pv1},${pv2},${pv3}\n" )
        if ( NOT "${WINDRES_RC_FILEOS}" STREQUAL "" )
            file ( APPEND ${rcFile} "FILEOS          VOS_${WINDRES_RC_FILEOS}\n" )
        endif()
        if ( NOT "${WINDRES_RC_FILETYPE}" STREQUAL "" )
            file ( APPEND ${rcFile} "FILETYPE        ${WINDRES_RC_FILETYPE}\n" )
        endif()
        file ( APPEND ${rcFile} "BEGIN\n" )
        file ( APPEND ${rcFile} "    BLOCK \"StringFileInfo\"\n" )
        file ( APPEND ${rcFile} "    BEGIN\n" )
        file ( APPEND ${rcFile} "        BLOCK \"040904b0\"\n" )
        file ( APPEND ${rcFile} "        BEGIN\n" )
        file ( APPEND ${rcFile} "            VALUE \"OriginalFilename\", \"${OriginalFilename}\\0\"\n" )
        file ( APPEND ${rcFile} "            VALUE \"CompanyName\",      \"${WINDRES_RC_COMPANY_NAME}\\0\"\n" )
        file ( APPEND ${rcFile} "            VALUE \"ProductName\",      \"${WINDRES_RC_PRODUCT_NAME}\\0\"\n" )
        file ( APPEND ${rcFile} "            VALUE \"FileVersion\",      \"${WINDRES_RC_FILE_VERSION}\\0\"\n" )
        file ( APPEND ${rcFile} "            VALUE \"FileDescription\",  \"${WINDRES_RC_FILE_DESCRIPTION}\\0\"\n" )
        if ( NOT "${WINDRES_RC_YEARS}" STREQUAL "" )
            file ( APPEND ${rcFile} "            VALUE \"LegalCopyright\",   \"Copyright (C) ${WINDRES_RC_YEARS} ${WINDRES_RC_COMPANY_NAME}.\\0\"\n" )
        endif()
        file ( APPEND ${rcFile} "        END\n" )
        file ( APPEND ${rcFile} "    END\n" )
        file ( APPEND ${rcFile} "    BLOCK \"VarFileInfo\"\n" )
        file ( APPEND ${rcFile} "    BEGIN\n" )
        file ( APPEND ${rcFile} "        VALUE \"Translation\", 0x0409, 1200\n" )
        file ( APPEND ${rcFile} "    END\n" )
        file ( APPEND ${rcFile} "END\n" )
    endif()

    set ( ${WINDRES_RC_OUTPUT_VAR} ${rcFile} )
endmacro()

macro ( WINDRES_VERSION_TO_CSV targetVar version )
    string ( REGEX REPLACE "\\." ";" versionList ${version} )
    list ( LENGTH versionList listLength )

    foreach ( index RANGE 4 )
        if ( index GREATER listLength OR index EQUAL listLength )
            set ( ${targetVar}${index} "0" )
        else()
            list ( GET versionList ${index} ${targetVar}${index} )
        endif()
    endforeach()
endmacro()

if ( TARGET_OS STREQUAL "Windows" )
    enable_language ( "RC" )

    if ( NOT CMAKE_RC_COMPILER_WORKS )
        message ( SEND_ERROR "CMAKE_RC_COMPILER_WORKS = ${CMAKE_RC_COMPILER_WORKS}" )
    endif()

    set ( CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>" )
endif()

# windres.cmake ends here
