# ==============================================================================
#
# Attempt to locate the Qt Help Framework tools.
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# author: Martin Ošmera <martin.osmera@moravia-microsystems.com>
#
# ==============================================================================

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

    mark_as_advanced ( Q_HELP_TOOLS_FOUND )
endif()

# QtHelpTools.cmake ends here
