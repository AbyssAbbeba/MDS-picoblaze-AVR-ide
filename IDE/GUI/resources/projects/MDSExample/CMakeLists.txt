# ==============================================================================
#
# MDS resources / MDS projects / The tutorial project
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# set project's name
project ( MDSExample )

# ------------------------------------------------------------------------------
# COMPILATION & INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

file ( GLOB PSM_FILES *.psm )
foreach ( PSM_FILE ${PSM_FILES} )
    set ( FILE_BASE ${PSM_FILE} )
    string(REGEX MATCH "/?[^/]+$" FILE_BASE ${FILE_BASE} )
    string(REGEX REPLACE "\\.psm$" "" FILE_BASE ${FILE_BASE} )
    string(REGEX REPLACE "^/" "" FILE_BASE ${FILE_BASE} )

    if ( "${FILE_BASE}.psm" IS_NEWER_THAN "${FILE_BASE}.dbg" )
        add_custom_target ( "${PROJECT_NAME}_${FILE_BASE}_psm" ALL
                            SOURCES "${FILE_BASE}.psm"
                            DEPENDS ${COMPILER_EXEC_TARGET}
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                            COMMENT "Building Tutorial Project file ${FILE_BASE}.psm."
                            COMMAND bash "${CMAKE_SOURCE_DIR}/runMdsCompiler.sh" ${COLOR_GCC}
                                    $<TARGET_FILE:${COMPILER_EXEC_TARGET}>
                                        --silent
                                        --plang=asm
                                        --arch=PicoBlaze
                                        --dev=kcpsm6
                                        --lst=${FILE_BASE}.lst
                                        --hex=${FILE_BASE}.ihex
                                        --dbg=${FILE_BASE}.dbg
                                        --srec=${FILE_BASE}.srec
                                        --binary=${FILE_BASE}.bin
                                        --mtable=${FILE_BASE}.mtbl
                                        --stable=${FILE_BASE}.stbl
                                        --vhdl=${FILE_BASE}.vhd
                                        --verilog=${FILE_BASE}.v
                                        --raw-hex-dump=${FILE_BASE}.rawhex
                                        --mem=${FILE_BASE}.mem
                                        --base-inc-dir=${CMAKE_SOURCE_DIR}/compiler/include
                                        "${FILE_BASE}.psm" )
    else()
        add_custom_target ( "${PROJECT_NAME}_${FILE_BASE}_psm" ALL
                            COMMENT "Tutorial Project file ${FILE_BASE}.psm is already built ... skipping." )
    endif()

    foreach ( extension "psm" "bin" "dbg" "ihex" "lst" "mem" "mtbl" "rawhex" "srec" "stbl" "v" "vhd" )
        install ( FILES "${FILE_BASE}.${extension}" DESTINATION ${INSTALL_DIR_DEMOPROJECT} )
    endforeach ( extension )
endforeach ( PSM_FILE )

# List of additional files that will be cleaned as a part of the "make clean" stage.
file ( GLOB COMPILATION_RESULTS *.bin *.dbg *.ihex *.lst *.mem *.mtbl *.rawhex *.srec *.stbl *.v *.vhd *~ )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${COMPILATION_RESULTS}" )

# Install the project file.
install ( FILES ${PROJECT_NAME}.mds-project DESTINATION ${INSTALL_DIR_DEMOPROJECT} )
