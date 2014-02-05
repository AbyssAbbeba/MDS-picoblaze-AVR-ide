# ==============================================================================
#
# Multi-target multi-language cross compiler.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( PicoBlazeAsmIncludeFiles )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of additional files that will be cleaned as a part of the "make clean" stage.
file ( GLOB PRC_FILES *.prc )
file ( GLOB LST_FILES *.lst )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${PRC_FILES};${LST_FILES}" )

# Locate compiler executable binary.
get_property ( MDS_COMPILER TARGET ${COMPILER_EXEC_TARGET} PROPERTY LOCATION )

# Build device specification files.
foreach ( PROCESSOR "kcpsm1" "kcpsm1cpld" "kcpsm2" "kcpsm3" "kcpsm6" )
    set ( FILE_BASE "${CMAKE_CURRENT_SOURCE_DIR}/${PROCESSOR}" )
    if ( "${FILE_BASE}.asm" IS_NEWER_THAN "${FILE_BASE}.prc" )
        add_custom_target ( "${PROJECT_NAME}_prc_file_${PROCESSOR}" ALL
                            SOURCES "${FILE_BASE}.asm"
                            DEPENDS ${COMPILER_EXEC_TARGET}
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                            COMMENT "Building device specification file for ${PROCESSOR}."
                            COMMAND bash "${CMAKE_SOURCE_DIR}/runMdsCompiler.sh" ${COLOR_GCC}
                                        ${MDS_COMPILER} --silent
                                                        --plang=asm
                                                        --arch=PicoBlaze
                                                        --lst=${FILE_BASE}.lst
                                                        --precompile=${FILE_BASE}.prc
                                                        --base-inc-dir=${CMAKE_SOURCE_DIR}/compiler/include
                                                        "${FILE_BASE}.asm" )
    else()
        add_custom_target ( "${PROJECT_NAME}_prc_file_${PROCESSOR}" ALL
                            COMMENT "Device specification file for ${PROCESSOR} is already built ... skipping." )
    endif()
endforeach ( PROCESSOR )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

foreach ( extension "asm" "prc" "v" "vhd" )
    file ( GLOB files2install *.${extension} )
    install ( FILES ${files2install} DESTINATION "${INSTALL_DIR_INCLUDE}/assembler/PicoBlaze" )
endforeach ( extension )
