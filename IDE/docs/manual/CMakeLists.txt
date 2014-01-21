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

# ------------------------------------------------------------------------------
# GENERATE MDS MANUAL.
# ------------------------------------------------------------------------------

#
## Attempt to locate LaTeX tools.
#
if ( NOT ( PDFLATEX_COMPILER AND LATEX2HTML_CONVERTER AND MAKEINDEX_PROGRAM ) )
    find_program ( PDFLATEX_COMPILER
                   NAMES pdflatex
                   PATHS $ENV{PATH} )

    find_program ( LATEX2HTML_CONVERTER
                   NAMES latex2html
                   PATHS $ENV{PATH} )

    find_program ( MAKEINDEX_PROGRAM
                   NAMES makeindex
                   PATHS $ENV{PATH} )

    if ( PDFLATEX_COMPILER AND LATEX2HTML_CONVERTER AND MAKEINDEX_PROGRAM )
        set ( LATEX_FOUND TRUE CACHE BOOL "" )
        message ( STATUS "Found pdflatex: ${PDFLATEX_COMPILER}" )
        message ( STATUS "Found latex2html: ${LATEX2HTML_CONVERTER}" )
        message ( STATUS "Found makeindex: ${MAKEINDEX_PROGRAM}" )
    else()
        set ( LATEX_FOUND FALSE CACHE BOOL "" )
        message ( WARNING "Unable to locate some of the required LaTeX tools (pdflatex, latex2html, or makeindex)." )
    endif()
endif()

#
## Attempt to locate the Qt Help Framework tools.
#
if ( NOT ( Q_HELP_GENERATOR AND Q_COLLECTION_GENERATOR ) )
    find_program ( Q_HELP_GENERATOR
                   NAMES qhelpgenerator
                   PATHS $ENV{PATH} )

    find_program ( Q_COLLECTION_GENERATOR
                   NAMES qcollectiongenerator
                   PATHS $ENV{PATH} )

    if ( Q_HELP_GENERATOR AND Q_COLLECTION_GENERATOR )
        set ( Q_HELP_TOOLS_FOUND TRUE CACHE BOOL "" )
        message ( STATUS "Found qhelpgenerator: ${Q_HELP_GENERATOR}" )
        message ( STATUS "Found qcollectiongenerator: ${Q_COLLECTION_GENERATOR}" )
    else()
        set ( Q_HELP_TOOLS_FOUND FALSE CACHE BOOL "" )
        message ( WARNING "Unable to locate some of the required Qt tools (qhelpgenerator or qcollectiongenerator)." )
    endif()
endif()

if ( LATEX_FOUND AND Q_HELP_TOOLS_FOUND )
    # Set some configuration variables.
    set ( SIGNATURE "(C) copyright 2014 <a href=\"http://www.moravia-microsystems.com\">Moravia Microsystems, s.r.o."
                    "</a>, Brno, CZ, European Union." )
    set ( HTML_INDEX "MDS_manual/index.html" )
    set ( LOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/compilation_log.log" )

    # List of additional files that will be cleaned as a part of the "make clean" stage.
    set ( CLEAN_FILES_LATEX "MDS_manual.log;MDS_manual.pdf;MDS_manual.toc;MDS_manual.aux;MDS_manual.out;MDS_manual" )
    set ( CLEAN_FILES_MAKEINDEX "MDS_manual.idx;MDS_manual.ind;MDS_manual.ilg" )
    set ( CLEAN_FILES_QHELP "MDS_manual.qhc;MDS_manual.qch;MDS_manual.qhp" )
    set ( CLEAN_FILES "${CLEAN_FILES_LATEX};${CLEAN_FILES_QHELP};${LOG_FILE};${CLEAN_FILES_MAKEINDEX}" )
    set_directory_properties ( PROPERTIES
                               ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES}" )

    # Use LaTeX tools to generate end user documentation, and Qt Help Framework tools to generate Qt Help.
    if ( "${CMAKE_CURRENT_SOURCE_DIR}/MDS_manual.tex" IS_NEWER_THAN "${CMAKE_CURRENT_SOURCE_DIR}/MDS_manual.pdf" )
        add_custom_target ( manual-pdflatex ALL
                            COMMAND ${PDFLATEX_COMPILER} MDS_manual.tex > "${LOG_FILE}" 2>&1
                            COMMAND ${MAKEINDEX_PROGRAM} MDS_manual.idx >> "${LOG_FILE}" 2>&1
                            COMMAND ${PDFLATEX_COMPILER} MDS_manual.tex >> "${LOG_FILE}" 2>&1
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            COMMENT "Generating MDS documentation as PDF with PDF LaTeX."
                            VERBATIM )
        add_custom_target ( manual-latex2html ALL
                            COMMAND ${LATEX2HTML_CONVERTER} -local_icons -numbered_footnotes -address "${SIGNATURE}"
                                    -info 0 MDS_manual.tex >> "${LOG_FILE}" 2>&1
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            DEPENDS manual-pdflatex
                            COMMENT "Generating MDS documentation as HTML files with LaTeX2HTML converter."
                            VERBATIM )
        add_custom_target ( manual ALL
                            COMMAND gawk -f genQHP.awk -v DNPR=1 -v RESFILE=MDS_manual.qhp ${HTML_INDEX}
                                    MDS_manual.qhp.in >> "${LOG_FILE}" 2>&1
                            COMMAND ${Q_COLLECTION_GENERATOR} MDS_manual.qhcp -o MDS_manual.qhc >> "${LOG_FILE}" 2>&1
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            DEPENDS manual-latex2html
                            COMMENT "Generating MDS documentation as QHC with Qt Help Framework tools."
                            VERBATIM )
    else()
        add_custom_target ( manual ALL
                            COMMENT "MDS documentation is already built ... skipping." )
    endif()
endif()
