# ==============================================================================
#
# FindBISON.cmake and FindFLEX.cmake wrapper, see manual for these CMake script
# for details regarding their purpose and operation.
#
# Intention of this wrapper is remove redundant regeneration and recompilation
# of all bison and flex generated analyzers during each compilation cycle. When
# developing/debugging a compiler, or something similar which heavily depends on
# bison generated parser(s) and/or flex generated lexer(s); it shortly becomes
# very annoying to be always forced to regenerate and recompile all involved
# lexical and syntax analyzers.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# author: Martin Ošmera <martin.osmera@moravia-microsystems.com>
#
# ==============================================================================

# find_package ( FLEX  REQUIRED )
# find_package ( BISON REQUIRED )

## Apply syntax analyzer generator.
 # This macro has exactly the same usage as macro FLEX_TARGET.
macro ( FLEX_TARGET_IMPROVED )
    if ( "${CMAKE_CURRENT_BINARY_DIR}/${ARGV1}" IS_NEWER_THAN "${CMAKE_CURRENT_BINARY_DIR}/${ARGV2}" )
        FLEX_TARGET ( ${ARGV} )
    else()
        set ( FLEX_${ARGV0}_OUTPUTS "${ARGV2}" )
    endif()
endmacro()

## Apply lexical analyzer generator.
 # This macro has exactly the same usage as macro BISON_TARGET.
macro ( BISON_TARGET_IMPROVED )
    if ( "${CMAKE_CURRENT_BINARY_DIR}/${ARGV1}" IS_NEWER_THAN "${CMAKE_CURRENT_BINARY_DIR}/${ARGV2}" )
        BISON_TARGET ( ${ARGV} )
    else()
        set ( BISON_${ARGV0}_OUTPUTS "${ARGV2}" )
    endif()
endmacro()

## Apply lexical and syntax analyzer generator, and set threir output files to be cleaned in the "make clean" stage.
 # parameter LexerTarget   : expectes LexerTarget.l; generates LexerTarget.h, and LexerTarget.cxx
 # parameter ParserTarget  : expectes ParserTarget.l; generates ParserTarget.h, ParserTarget.cxx, ParserTarget.output
 # parameter libraryTarget : name of static library where the parser and lexer will be compiled into (with -fPIC).
macro ( FLEX_BISON_PAIR LexerTarget ParserTarget libraryTarget )
    # Build lexical and syntax analyzers.
    FLEX_TARGET_IMPROVED  ( ${LexerTarget} "${LexerTarget}.l" "${LexerTarget}.cxx" )
    BISON_TARGET_IMPROVED ( ${ParserTarget} "${ParserTarget}.y" "${ParserTarget}.cxx" VERBOSE "${ParserTarget}.output" )

    # List of additional files that will be cleaned as a part of the "make clean" stage.
    set ( LEXER_CLEAN_FILES "${LexerTarget}.h;${LexerTarget}.cxx" )
    set ( PARSER_CLEAN_FILES "${ParserTarget}.output;${ParserTarget}.h;${ParserTarget}.cxx" )
    get_directory_property ( cleanFiles ADDITIONAL_MAKE_CLEAN_FILES )
    list ( APPEND cleanFiles ${LEXER_CLEAN_FILES} ${PARSER_CLEAN_FILES} )
    set_directory_properties ( PROPERTIES
                               ADDITIONAL_MAKE_CLEAN_FILES "${cleanFiles}" )

    ## Procude static C++ library from the generated lexer and parser.
    add_library ( ${libraryTarget} STATIC
                  ${FLEX_${LexerTarget}_OUTPUTS}
                  ${BISON_${ParserTarget}_OUTPUTS} )

    ## Make the library position-independent code (PIC) suitable for use in a shared library,
    # and disable compiler warnings for it.
    set_target_properties ( ${libraryTarget} PROPERTIES
                                             LINKER_LANGUAGE "CXX"
                                             COMPILE_FLAGS "-w" )

endmacro()

# FlexBisonPair.cmake ends here
