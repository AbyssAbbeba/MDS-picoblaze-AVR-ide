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
cmake_minimum_required(VERSION 2.8)

# Set project name
project ( manual )

# ------------------------------------------------------------------------------
# GENERATE MDS MANUAL.
# ------------------------------------------------------------------------------

# Path to MDS manual as PDF file, and as Qt Help Collection Files.
set ( MANUAL_QHC "${CMAKE_CURRENT_SOURCE_DIR}/MDS_manual.qhc" CACHE STRING "Path to MDS manual as QHC file." )
set ( MANUAL_QCH "${CMAKE_CURRENT_SOURCE_DIR}/MDS_manual.qch" CACHE STRING "Path to MDS manual as QCH file." )
set ( MANUAL_PDF "${CMAKE_CURRENT_SOURCE_DIR}/${MDS_MANUAL_PDF}" CACHE STRING "Path to MDS manual as PDF file." )

mark_as_advanced ( MANUAL_PDF MANUAL_QHC MANUAL_QCH )

macro ( IS_UPDATE_REQUIRED returnValue )
    set ( ${returnValue} FALSE )
    file ( GLOB texFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.tex" )
    foreach ( inFile ${texFiles} )
        foreach ( outFile ${MANUAL_PDF} ${MANUAL_QHC} ${MANUAL_QCH} )
            if ( "${inFile}" IS_NEWER_THAN "${outFile}" )
                set ( ${returnValue} TRUE )
                break()
            endif()
        endforeach()
        if ( ${${returnValue}} )
            break()
        endif()
    endforeach()
endmacro()

if ( LATEX_FOUND AND Q_HELP_TOOLS_FOUND )
    # Set some configuration variables.
    set ( LOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/compilation_log.log" )
    set ( CAPTURE "${CMAKE_CURRENT_SOURCE_DIR}/../../captureOutput.sh" )

    # List of additional files that will be cleaned as a part of the "make clean" stage.
    file ( GLOB CLEAN_HTLATEX *.html *.css *.4ct *.4tc *.lg *.tc *.dvi *.idv *.tmp *.xref texput.log genXHTML.log )
    set ( CLEAN_MAKEINDEX "MDS_manual.idx;MDS_manual.ind;MDS_manual.ilg;MDS_manual.4dx;MDS_manual.4ix;MDS_manual.lot" )
    set ( CLEAN_QHELP     "${MANUAL_QHC};${MANUAL_QCH};MDS_manual.qhp;idxmake.log" )
    set ( CLEAN_LATEX     "${MANUAL_PDF};MDS_manual.log;MDS_manual.toc;MDS_manual.aux;MDS_manual.out;MDS_manual" )
    set ( MAN_CLEAN_FILES "${LOG_FILE};${CLEAN_LATEX};${CLEAN_QHELP};${CLEAN_MAKEINDEX};${CLEAN_HTLATEX}" )
    set_directory_properties ( PROPERTIES
                               ADDITIONAL_MAKE_CLEAN_FILES "${MAN_CLEAN_FILES}" )

    # Use LaTeX tools to generate end user documentation, and Qt Help Framework tools to generate Qt Help.
    is_update_required(UPDATE_PDF)
    if ( NOT ${UPDATE_PDF} )
        add_custom_target ( manual ALL
                            COMMENT "MDS documentation is already built ... skipping." )
    else()
        add_custom_target ( manual-pdflatex ALL
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            COMMENT "Generating MDS documentation as PDF with PDF LaTeX."
                            COMMAND bash -c "[ -e MDS_manual.ind ] && rm MDS_manual.ind; exit 0"
                            COMMAND bash ${CAPTURE} ${LOG_FILE} ${PDFLATEX_COMPILER} -interaction nonstopmode MDS_manual.tex
                            COMMAND bash ${CAPTURE} ${LOG_FILE} ${MAKEINDEX_COMPILER} MDS_manual.idx
                            COMMAND bash ${CAPTURE} ${LOG_FILE} ${PDFLATEX_COMPILER} -interaction nonstopmode MDS_manual.tex
                            VERBATIM )

        if ( TARGET_OS STREQUAL "Windows" )
            add_custom_target ( manual-htlatex ALL
                                DEPENDS manual-pdflatex
                                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                                COMMENT "Generating MDS documentation as XHTML files with htlatex converter."
                                COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/genXHTML.bat"
                                        ${HTLATEX_CONVERTER} ${TEX_COMPILER} ${MAKEINDEX_COMPILER}
                                        MDS_manual.tex genXHTML.log MDS_manual.ind MDS_manual.4dx
                                VERBATIM )
        else()
            add_custom_target ( manual-htlatex ALL
                                DEPENDS manual-pdflatex
                                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                                COMMAND bash ${CAPTURE} ${LOG_FILE} ${HTLATEX_CONVERTER}
                                        MDS_manual "xhtml,charset=utf-8,next,index=2,5" "-utf8"
                                COMMAND bash ${CAPTURE} ${LOG_FILE} ${TEX_COMPILER}
                                        "\\def\\filename{{MDS_manual}{idx}{4dx}{ind}} \\input idxmake.4ht"
                                COMMAND bash ${CAPTURE} ${LOG_FILE} ${MAKEINDEX_COMPILER}
                                        -o MDS_manual.ind MDS_manual.4dx
                                COMMAND bash ${CAPTURE} ${LOG_FILE} ${HTLATEX_CONVERTER}
                                        MDS_manual "xhtml,charset=utf-8,next,index=2,5" "-utf8"
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
    endif()
endif()

install ( FILES "${MANUAL_PDF}" "${MANUAL_QHC}" "${MANUAL_QCH}" "QuickUserGuideAssembler.pdf"
          DESTINATION ${INSTALL_DIR_DOC} )
