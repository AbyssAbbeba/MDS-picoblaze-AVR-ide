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
project ( manual )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# ------------------------------------------------------------------------------
# GENERATE MDS MANUAL.
# ------------------------------------------------------------------------------

#
## Attempt to locate LaTeX tools.
#

find_program ( PDFLATEX_COMPILER
               NAMES pdflatex
               PATHS /usr/bin )

find_program ( LATEX2HTML_CONVERTER
               NAMES latex2html
               PATHS /usr/bin )

find_program ( MAKEINDEX_PROGRAM
               NAMES makeindex
               PATHS /usr/bin )

if ( PDFLATEX_COMPILER AND LATEX2HTML_CONVERTER AND MAKEINDEX_PROGRAM )
    message("pdflatex found: '${PDFLATEX_COMPILER}'")
    message("latex2html found: '${LATEX2HTML_CONVERTER}'")
    message("makeindex found: '${MAKEINDEX_PROGRAM}'")
    set ( LATEX_FOUND 1 )
endif ( PDFLATEX_COMPILER AND LATEX2HTML_CONVERTER AND MAKEINDEX_PROGRAM )


#
## Attempt to locate the Qt Help Framework tools.
#

find_program ( Q_HELP_GENERATOR
               NAMES qhelpgenerator
               PATHS /usr/bin )

find_program ( Q_COLLECTION_GENERATOR
               NAMES qcollectiongenerator
               PATHS /usr/bin )

if ( Q_HELP_GENERATOR AND Q_COLLECTION_GENERATOR )
    message("qhelpgenerator found: '${Q_HELP_GENERATOR}'")
    message("qcollectiongenerator found: '${Q_COLLECTION_GENERATOR}'")
    set ( Q_HELP_TOOLS_FOUND 1 )
endif ( Q_HELP_GENERATOR AND Q_COLLECTION_GENERATOR )

if ( LATEX_FOUND AND Q_HELP_TOOLS_FOUND )
    # Set some configuration variables.
    set ( SIGNATURE "(C) copyright 2014 <a href=\"http://www.moravia-microsystems.com\">Moravia Microsystems, s.r.o."
                    "</a>, Brno, CZ, European Union." )
    set ( HTML_INDEX "MDS_manual/index.html" )
    set ( LOG_FILE "${CMAKE_CURRENT_BINARY_DIR}/compilation_log.log" )

    # List of additional files that will be cleaned as a part of the "make clean" stage.
    set ( CLEAN_FILES_LATEX "MDS_manual.log;MDS_manual.pdf;MDS_manual.toc;MDS_manual.aux;MDS_manual.out;MDS_manual" )
    set ( CLEAN_FILES_QHELP "MDS_manual.qhc;MDS_manual.qch;MDS_manual.qhp" )
    set ( CLEAN_FILES "${CLEAN_FILES_LATEX};${CLEAN_FILES_QHELP};${LOG_FILE}" )
    set_directory_properties ( PROPERTIES
                               ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES}" )

    # Use LaTeX tools to generate end user documentation, and Qt Help Framework tools to generate Qt Help.
    add_custom_target ( manual ALL
                        COMMAND ${PDFLATEX_COMPILER} MDS_manual.tex > "${LOG_FILE}" 2>&1
                        COMMAND ${MAKEINDEX_PROGRAM} MDS_manual.idx >> "${LOG_FILE}" 2>&1
                        COMMAND ${PDFLATEX_COMPILER} MDS_manual.tex >> "${LOG_FILE}" 2>&1
                        COMMAND ${LATEX2HTML_CONVERTER} -local_icons -numbered_footnotes -address "${SIGNATURE}"
                                -info 0 MDS_manual.tex >> "${LOG_FILE}" 2>&1
                        COMMAND gawk -f genQHP.awk -v DNPR=1 -v RESFILE=MDS_manual.qhp ${HTML_INDEX}
                                MDS_manual.qhp.in >> "${LOG_FILE}" 2>&1
                        COMMAND ${Q_COLLECTION_GENERATOR} MDS_manual.qhcp -o MDS_manual.qhc >> "${LOG_FILE}" 2>&1
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Generating MDS documentation with PDF LaTeX, and Qt Help Framework tools."
                        VERBATIM )
endif ( LATEX_FOUND AND Q_HELP_TOOLS_FOUND )
