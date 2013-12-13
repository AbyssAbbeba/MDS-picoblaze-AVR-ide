# ==============================================================================
#
# MDS manual.
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

find_program ( PDFLATEX_COMPILER
               NAMES pdflatex
               PATHS /usr/bin )

find_program ( LATEX2HTML_CONVERTER
               NAMES latex2html
               PATHS /usr/bin )

if ( PDFLATEX_COMPILER AND LATEX2HTML_CONVERTER )
    message("pdflatex found: '${PDFLATEX_COMPILER}'")
    message("latex2html found: '${LATEX2HTML_CONVERTER}'")
    set ( LATEX_FOUND 1 )
endif ( PDFLATEX_COMPILER AND LATEX2HTML_CONVERTER )

if ( LATEX_FOUND )
    add_custom_target ( doc
                        ${PDFLATEX_COMPILER} ${CMAKE_CURRENT_BINARY_DIR}/MDS_manual.tex
                        COMMAND ${LATEX2HTML_CONVERTER} ${CMAKE_CURRENT_BINARY_DIR}/MDS_manual.tex
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Generating MDS documentation with PDF LaTeX" VERBATIM )
endif ( LATEX_FOUND )
