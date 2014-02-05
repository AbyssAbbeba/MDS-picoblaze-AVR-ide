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

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 FATAL_ERROR )

# ------------------------------------------------------------------------------
# GENERATE API DOCUMENTATION WITH DOXYGEN
# ------------------------------------------------------------------------------

find_package ( Doxygen )
if ( DOXYGEN_FOUND )
    # Set some configuration variables.
    set ( LOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/compilation_log.log" )
    set ( CAPTURE "${CMAKE_CURRENT_SOURCE_DIR}/../../captureOutput.sh" )

    # List of additional files that will be cleaned as a part of the "make clean" stage.
    set ( CLEAN_FILES "${LOG_FILE};Doxyfile;html/" )
    set_directory_properties ( PROPERTIES
                               ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES}" )

    # Use doxygen to generate API documentation,
    configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile @ONLY )
    add_custom_target ( doxygen
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        COMMENT "Generating API documentation with Doxygen"
                        COMMAND bash ${CAPTURE} ${LOG_FILE} ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
                        VERBATIM )
endif()
