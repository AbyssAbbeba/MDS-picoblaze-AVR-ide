# ==============================================================================
#
# Attempt to locate LaTeX tools.
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# author: Martin Ošmera <martin.osmera@moravia-microsystems.com>
#
# ==============================================================================

if ( NOT ( PDFLATEX_COMPILER AND HTLATEX_CONVERTER AND MAKEINDEX_COMPILER ) )
    find_program ( TEX_COMPILER
                   NAMES tex tex.exe
                   PATHS $ENV{PATH}
                   DOC "path to the tex executable" )

    find_program ( PDFLATEX_COMPILER
                   NAMES pdflatex pdflatex.exe
                   PATHS $ENV{PATH}
                   DOC "path to the pdflatex executable" )

    find_program ( HTLATEX_CONVERTER
                   NAMES htlatex htlatex.exe
                   PATHS $ENV{PATH}
                   DOC "path to the htlatex executable" )

    find_program ( MAKEINDEX_COMPILER
                   NAMES makeindex makeindex.exe
                   PATHS $ENV{PATH}
                   DOC "path to the makeindex executable" )

    mark_as_advanced ( TEX_COMPILER )
    mark_as_advanced ( PDFLATEX_COMPILER )
    mark_as_advanced ( HTLATEX_CONVERTER )
    mark_as_advanced ( MAKEINDEX_COMPILER )

    if ( TEX_COMPILER AND PDFLATEX_COMPILER AND HTLATEX_CONVERTER AND MAKEINDEX_COMPILER )
        set ( LATEX_FOUND TRUE CACHE BOOL "" )
        message ( STATUS "Found tex: ${TEX_COMPILER}" )
        message ( STATUS "Found pdflatex: ${PDFLATEX_COMPILER}" )
        message ( STATUS "Found htlatex: ${HTLATEX_CONVERTER}" )
        message ( STATUS "Found makeindex: ${MAKEINDEX_COMPILER}" )
    else()
        set ( LATEX_FOUND FALSE CACHE BOOL "" )
        if ( NOT TEX_COMPILER )
            message ( WARNING "plain tex compiler not found." )
        endif()
        if ( NOT PDFLATEX_COMPILER )
            message ( WARNING "pdflatex compiler not found." )
        endif()
        if ( NOT HTLATEX_CONVERTER )
            message ( WARNING "htlatex coverter not found." )
        endif()
        if ( NOT MAKEINDEX_COMPILER )
            message ( WARNING "makeindex utility not found." )
        endif()
    endif()

    mark_as_advanced ( LATEX_FOUND )
endif()

# MDSFindLatex.cmake ends here
