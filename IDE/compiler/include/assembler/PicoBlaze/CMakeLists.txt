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

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of additional files that will be cleaned as a part of the "make clean" stage.
FILE ( GLOB PRC_FILES *.prc )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${PRC_FILES}" )

# Locate compiler binary.
set ( MDS_COMPILER "../../../compiler" )
if ( CMAKE_HOST_WIN32 )
    set ( MDS_COMPILER "${MDS_COMPILER}.exe" )
endif ( CMAKE_HOST_WIN32 )

# Build device specification files.
foreach ( PROCESSOR "kcpsm1" "kcpsm1cpld" "kcpsm2" "kcpsm3" "kcpsm6" )
    add_custom_target ( "${PROJECT_NAME}_build_prc_file_${PROCESSOR}" ALL
                        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                        COMMENT "Building device specification file for ${PROCESSOR}."
                        DEPENDS compiler
                        COMMAND ${MDS_COMPILER} --plang=asm
                                                --arch=PicoBlaze
                                                --silent
                                                --precompile="${PROCESSOR}.prc"
                                                --file="${PROCESSOR}.asm" &> /dev/null )
endforeach ( PROCESSOR )


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

foreach ( EXTENSION "asm" "prc" "v" "vhd" )
    file ( GLOB FILES2INSTALL *.${EXTENSION} )
    install ( FILES ${FILES2INSTALL} DESTINATION "include/assembler/PicoBlaze" )
endforeach ( EXTENSION )