# ==============================================================================
#
# Multi-target multi-language cross compiler.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
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

# Locate compiler binary.
set ( MDS_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/../../../mds-compiler" )
if ( CMAKE_HOST_WIN32 )
    set ( MDS_COMPILER "${MDS_COMPILER}.exe" )
endif()

# Build device specification files.
foreach ( PROCESSOR "kcpsm1" "kcpsm1cpld" "kcpsm2" "kcpsm3" "kcpsm6" )
    set ( FILE_BASE "${CMAKE_CURRENT_SOURCE_DIR}/${PROCESSOR}" )
    if ( "${FILE_BASE}.asm" IS_NEWER_THAN "${FILE_BASE}.prc" )
        add_custom_target ( "${PROJECT_NAME}_prc_file_${PROCESSOR}" ALL
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                            COMMENT "Building device specification file for ${PROCESSOR}."
                            DEPENDS mds-compiler
                            SOURCES "${FILE_BASE}.asm"
                            COMMAND ${MDS_COMPILER} --silent
                                                    --plang=asm
                                                    --arch=PicoBlaze
                                                    --lst="${FILE_BASE}.lst"
                                                    --precompile="${FILE_BASE}.prc"
                                                    "${FILE_BASE}.asm" 1> /dev/null )
    else()
        add_custom_target ( "${PROJECT_NAME}_prc_file_${PROCESSOR}" ALL
                            COMMENT "Device specification file for ${PROCESSOR} is already built ... skipping." )
    endif()
endforeach ( PROCESSOR )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

foreach ( EXTENSION "asm" "prc" "v" "vhd" )
    file ( GLOB FILES2INSTALL *.${EXTENSION} )
    install ( FILES ${FILES2INSTALL} DESTINATION "include/assembler/PicoBlaze" )
endforeach ( EXTENSION )
