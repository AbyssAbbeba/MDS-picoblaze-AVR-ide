# ==============================================================================
#
# API documentation.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( doxygen )

# ------------------------------------------------------------------------------
# GENERATE API DOCUMENTATION WITH DOXYGEN
# ------------------------------------------------------------------------------

if ( DOXYGEN_FOUND )
    # Set some configuration variables.
    set ( LOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/compilation_log.log" )
    set ( CAPTURE "${CMAKE_CURRENT_SOURCE_DIR}/../../captureOutput.sh" )
    set ( APIDOC_QT_HELP "MDS-API" )

    # List of additional files that will be cleaned as a part of the "make clean" stage.
    set ( CLEAN_FILES "${LOG_FILE};Doxyfile;html/;qhc/" )
    set_directory_properties ( PROPERTIES
                               ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES}" )

    # Use Doxygen to generate API documentation as XHTML + SVG.
    # And as Qt Help Project file (.qhp) and Qt Compressed Help (.qch).
    configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile @ONLY )
    add_custom_target ( doxygen
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        COMMENT "Generating API documentation as XHTML with Doxygen"
                        COMMAND bash ${CAPTURE} ${LOG_FILE} ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
                        VERBATIM )

    #
    ## Generate API documentation as Qt Help Collection file (loadable by qAssistant).
    #

    # Generate Qt Help Collection Project file (.qhcp).
    set ( qhcp "${CMAKE_CURRENT_SOURCE_DIR}/qhc/${APIDOC_QT_HELP}.qhcp" )
    file ( WRITE  "${qhcp}" "<?xml version='1.0' encoding='utf-8'?>\n" )
    file ( APPEND "${qhcp}" "<QHelpCollectionProject version='1.0'>\n" )
    file ( APPEND "${qhcp}" "    <docFiles>\n" )
    file ( APPEND "${qhcp}" "        <register>\n" )
    file ( APPEND "${qhcp}" "            <file>${APIDOC_QT_HELP}.qch</file>\n" )
    file ( APPEND "${qhcp}" "        </register>\n" )
    file ( APPEND "${qhcp}" "    </docFiles>\n" )
    file ( APPEND "${qhcp}" "</QHelpCollectionProject>\n" )

    # Generate Qt Help Collection file (.qhc).
    add_custom_target ( doxygen-qhc
                        DEPENDS doxygen
                        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/qhc"
                        COMMENT "Generating API documentation as QHC (Qt Help Collection) with Qt Help Framework tools."
                        COMMAND ${Q_COLLECTION_GENERATOR} "${APIDOC_QT_HELP}.qhcp" -o "${APIDOC_QT_HELP}.qhc"
                        VERBATIM )
endif()
