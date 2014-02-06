# ==============================================================================
#
# MDS manual.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
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

# Path to MDS manual as PDF file.
set ( MANUAL_PDF "MDS_manual.pdf" CACHE STRING "Path to MDS manual as PDF file." )

#
## Attempt to locate LaTeX tools.
#
if ( NOT ( PDFLATEX_COMPILER AND HTLATEX_CONVERTER AND MAKEINDEX_PROGRAM ) )
    find_program ( PDFLATEX_COMPILER
                   NAMES pdflatex pdflatex.exe
                   PATHS $ENV{PATH}
                   DOC "path to the pdflatex executable" )

    find_program ( HTLATEX_CONVERTER
                   NAMES htlatex htlatex.exe
                   PATHS $ENV{PATH}
                   DOC "path to the htlatex executable" )

    find_program ( MAKEINDEX_PROGRAM
                   NAMES makeindex makeindex.exe
                   PATHS $ENV{PATH}
                   DOC "path to the makeindex executable" )

    mark_as_advanced ( PDFLATEX_COMPILER )
    mark_as_advanced ( HTLATEX_CONVERTER )
    mark_as_advanced ( MAKEINDEX_PROGRAM )

    if ( PDFLATEX_COMPILER AND HTLATEX_CONVERTER AND MAKEINDEX_PROGRAM )
        set ( LATEX_FOUND TRUE CACHE BOOL "" )
        message ( STATUS "Found pdflatex: ${PDFLATEX_COMPILER}" )
        message ( STATUS "Found htlatex: ${HTLATEX_CONVERTER}" )
        message ( STATUS "Found makeindex: ${MAKEINDEX_PROGRAM}" )
    else()
        set ( LATEX_FOUND FALSE CACHE BOOL "" )
        if ( NOT PDFLATEX_COMPILER )
            message ( WARNING "pdflatex compiler not found." )
        endif()
        if ( NOT HTLATEX_CONVERTER )
            message ( WARNING "htlatex coverter not found." )
        endif()
        if ( NOT MAKEINDEX_PROGRAM )
            message ( WARNING "makeindex utility not found." )
        endif()
    endif()
endif()

#
## Attempt to locate the Qt Help Framework tools.
#
if ( NOT ( Q_HELP_GENERATOR AND Q_COLLECTION_GENERATOR ) )
    find_program ( Q_HELP_GENERATOR
                   NAMES qhelpgenerator qhelpgenerator.exe
                   PATHS $ENV{PATH}
                   DOC "path to the qhelpgenerator executable" )

    find_program ( Q_COLLECTION_GENERATOR
                   NAMES qcollectiongenerator qcollectiongenerator.exe
                   PATHS $ENV{PATH}
                   DOC "path to the qcollectiongenerator executable" )

    mark_as_advanced ( Q_HELP_GENERATOR )
    mark_as_advanced ( Q_COLLECTION_GENERATOR )

    if ( Q_HELP_GENERATOR AND Q_COLLECTION_GENERATOR )
        set ( Q_HELP_TOOLS_FOUND TRUE CACHE BOOL "" )
        message ( STATUS "Found qhelpgenerator: ${Q_HELP_GENERATOR}" )
        message ( STATUS "Found qcollectiongenerator: ${Q_COLLECTION_GENERATOR}" )
    else()
        set ( Q_HELP_TOOLS_FOUND FALSE CACHE BOOL "" )
        if ( NOT Q_HELP_GENERATOR )
            message ( WARNING "qcollectiongenerator program not found." )
        endif()
        if ( NOT Q_COLLECTION_GENERATOR )
            message ( WARNING "qcollectiongenerator program not found." )
        endif()
    endif()

endif()

mark_as_advanced ( Q_HELP_TOOLS_FOUND )
mark_as_advanced ( LATEX_FOUND )
mark_as_advanced ( MANUAL_PDF )

if ( LATEX_FOUND AND Q_HELP_TOOLS_FOUND )
    # Set some configuration variables.
    set ( LOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/compilation_log.log" )
    set ( CAPTURE "${CMAKE_CURRENT_SOURCE_DIR}/../../captureOutput.sh" )

    # List of additional files that will be cleaned as a part of the "make clean" stage.
    file ( GLOB CLEAN_HTLATEX *.html *.css *.4ct *.4tc *.lg *.tc *.dvi *.idv *.tmp *.xref texput.log genXHTML.log )
    set ( CLEAN_MAKEINDEX "MDS_manual.idx;MDS_manual.ind;MDS_manual.ilg" )
    set ( CLEAN_QHELP     "MDS_manual.qhc;MDS_manual.qch;MDS_manual.qhp" )
    set ( CLEAN_LATEX     "MDS_manual.log;${MANUAL_PDF};MDS_manual.toc;MDS_manual.aux;MDS_manual.out;MDS_manual" )
    set ( CLEAN_FILES "${LOG_FILE};${CLEAN_LATEX};${CLEAN_QHELP};${CLEAN_MAKEINDEX};${CLEAN_HTLATEX}" )
    set_directory_properties ( PROPERTIES
                               ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES}" )

    # Use LaTeX tools to generate end user documentation, and Qt Help Framework tools to generate Qt Help.
    if ( "${CMAKE_CURRENT_SOURCE_DIR}/MDS_manual.tex" IS_NEWER_THAN "${CMAKE_CURRENT_SOURCE_DIR}/${MANUAL_PDF}" )
        add_custom_target ( manual-pdflatex ALL
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            COMMENT "Generating MDS documentation as PDF with PDF LaTeX."
                            COMMAND bash ${CAPTURE} ${LOG_FILE} ${PDFLATEX_COMPILER} MDS_manual.tex
                            COMMAND bash ${CAPTURE} ${LOG_FILE} ${MAKEINDEX_PROGRAM} MDS_manual.idx
                            COMMAND bash ${CAPTURE} ${LOG_FILE} ${PDFLATEX_COMPILER} MDS_manual.tex
                            VERBATIM )

        if ( TARGET_OS STREQUAL "Windows" )
            add_custom_target ( manual-htlatex ALL
                                DEPENDS manual-pdflatex
                                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                                COMMENT "Generating MDS documentation as XHTML files with htlatex converter."
                                COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/genXHTML.bat"
                                        ${HTLATEX_CONVERTER} MDS_manual.tex genXHTML.log
                                VERBATIM )
        else()
            add_custom_target ( manual-htlatex ALL
                                DEPENDS manual-pdflatex
                                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                                COMMAND bash ${CAPTURE} ${LOG_FILE} ${HTLATEX_CONVERTER}
                                        MDS_manual.tex "xhtml,charset=utf-8,next,3" "-utf8"
                                COMMENT "Generating MDS documentation as XHTML files with htlatex converter."
                                VERBATIM )
        endif()

        add_custom_target ( manual ALL
                            DEPENDS manual-htlatex
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            COMMENT "Generating MDS documentation as QHC with Qt Help Framework tools."
                            COMMAND bash ${CAPTURE} ${LOG_FILE} gawk -f genQHP.awk
                                                                     -v DNPR=1
                                                                     -v RESFILE=MDS_manual.qhp
                                                                     -v NAME=MDS_Manual
                                                                     -v INDEX_FILE=MDS_manual.html
                                                                     MDS_manual.html MDS_manual.qhp.in
                            COMMAND bash ${CAPTURE} ${LOG_FILE} ${Q_COLLECTION_GENERATOR}
                                    MDS_manual.qhcp -o MDS_manual.qhc
                            VERBATIM )
    else()
        add_custom_target ( manual ALL
                            COMMENT "MDS documentation is already built ... skipping." )
    endif()
endif()

install ( FILES "${CMAKE_CURRENT_SOURCE_DIR}/${MANUAL_PDF}"
          DESTINATION ${INSTALL_DIR_DOC} )
