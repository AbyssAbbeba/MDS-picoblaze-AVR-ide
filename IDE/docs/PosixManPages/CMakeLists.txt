# ==============================================================================
#
# Unix/Linux style manual pages.
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( PosixManPages )

# ------------------------------------------------------------------------------
# BUILD & INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

set ( compressedManPageFiles "" )
file ( GLOB manualPages *.1 )

foreach ( manPage ${manualPages} )
    set ( manPageCompressed "${manPage}.gz" )

    list ( APPEND compressedManPageFiles "${manPageCompressed}" )

    string ( REGEX REPLACE "/.*/" "" manPageTarget "${manPage}" )
    string ( REPLACE "." "-" manPageTarget "${manPageTarget}" )

    if ( "${manPage}" IS_NEWER_THAN "${manPageCompressed}" )
        add_custom_target ( man-page-${manPageTarget} ALL
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            COMMENT "Compressing manual page: \"${manPage}\"."
                            COMMAND gzip --quiet --best --to-stdout ${manPage} > ${manPageCompressed}
                            COMMAND touch ${manPageCompressed}
                            SOURCES ${manPage}
                            VERBATIM )
    else()
        add_custom_target ( man-page-${manPageTarget} ALL
                            COMMENT "Compressing manual page: ${manPage} ... already done, skipping." )
    endif()
endforeach()

set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${compressedManPageFiles}" )

install ( FILES ${compressedManPageFiles}
          DESTINATION ${INSTALL_DIR_MAN} )
