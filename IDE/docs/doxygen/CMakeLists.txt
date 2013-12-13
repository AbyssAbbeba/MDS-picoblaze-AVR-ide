# ==============================================================================
#
# API documentation.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( doxygen )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# ------------------------------------------------------------------------------
# GENERATE API DOCUMENTATION WITH DOXYGEN
# ------------------------------------------------------------------------------

find_package ( Doxygen )
if ( DOXYGEN_FOUND )
    configure_file ( ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY )
    add_custom_target ( doxygen ALL
                        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Generating API documentation with Doxygen" VERBATIM )
endif ( DOXYGEN_FOUND )
