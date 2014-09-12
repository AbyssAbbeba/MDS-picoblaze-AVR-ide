# ==============================================================================
#
# Configure MDS fetures and generate "mds.h".
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ----------------------------------------------------------------------------------------------------------------------
# Set internal configuration variables.
# ----------------------------------------------------------------------------------------------------------------------

if ( MDS_GRADE STREQUAL "Basic" )
    set ( MDS_FEATURE_CONVERTER_TOOL            TRUE )
    set ( MDS_FEATURE_8_SEGMENT_EDITOR          TRUE )
    set ( MDS_FEATURE_FILECONVERTER             FALSE )
    set ( MDS_FEATURE_DISASSEMBLER              FALSE )
    set ( MDS_FEATURE_TRANSLATOR                FALSE )
    set ( MDS_FEATURE_LOOP_GENERATOR            FALSE )
    set ( MDS_FEATURE_SIM_LED_PANEL             FALSE )
    set ( MDS_FEATURE_SIM_7_SEGMENT             FALSE )
    set ( MDS_FEATURE_ADAPTABLE_SIMULATOR       FALSE )

elseif ( MDS_GRADE STREQUAL "Premium" )
    set ( MDS_FEATURE_CONVERTER_TOOL            TRUE )
    set ( MDS_FEATURE_8_SEGMENT_EDITOR          TRUE )
    set ( MDS_FEATURE_FILECONVERTER             TRUE )
    set ( MDS_FEATURE_DISASSEMBLER              TRUE )
    set ( MDS_FEATURE_TRANSLATOR                TRUE )
    set ( MDS_FEATURE_LOOP_GENERATOR            TRUE )
    set ( MDS_FEATURE_SIM_LED_PANEL             TRUE )
    set ( MDS_FEATURE_SIM_7_SEGMENT             TRUE )
    set ( MDS_FEATURE_ADAPTABLE_SIMULATOR       FALSE )

elseif ( MDS_GRADE STREQUAL "Professional" )
    set ( MDS_FEATURE_CONVERTER_TOOL            TRUE )
    set ( MDS_FEATURE_8_SEGMENT_EDITOR          TRUE )
    set ( MDS_FEATURE_FILECONVERTER             TRUE )
    set ( MDS_FEATURE_DISASSEMBLER              TRUE )
    set ( MDS_FEATURE_TRANSLATOR                TRUE )
    set ( MDS_FEATURE_LOOP_GENERATOR            TRUE )
    set ( MDS_FEATURE_SIM_LED_PANEL             TRUE )
    set ( MDS_FEATURE_SIM_7_SEGMENT             TRUE )
    set ( MDS_FEATURE_ADAPTABLE_SIMULATOR       TRUE )

elseif ( MDS_GRADE STREQUAL "Ultimate" )
    set ( MDS_FEATURE_CONVERTER_TOOL            TRUE )
    set ( MDS_FEATURE_8_SEGMENT_EDITOR          TRUE )
    set ( MDS_FEATURE_FILECONVERTER             TRUE )
    set ( MDS_FEATURE_DISASSEMBLER              TRUE )
    set ( MDS_FEATURE_TRANSLATOR                TRUE )
    set ( MDS_FEATURE_LOOP_GENERATOR            TRUE )
    set ( MDS_FEATURE_SIM_LED_PANEL             TRUE )
    set ( MDS_FEATURE_SIM_7_SEGMENT             TRUE )
    set ( MDS_FEATURE_ADAPTABLE_SIMULATOR       TRUE )
endif()

if ( MDS_VARIANT STREQUAL "Trial" )
    set ( MDS_FEATURE_TRIAL TRUE )
else()
    set ( MDS_FEATURE_LICENCE_CERTIFICATE TRUE )
endif()

# ----------------------------------------------------------------------------------------------------------------------
# GENERATE "mds.h"
# ----------------------------------------------------------------------------------------------------------------------

file ( WRITE "mds.h" "// Basic definitions for MDS, generated automatically by CMake.\n" )

file ( APPEND "mds.h" "#define MDS_VARIANT_${MDS_VARIANT_UPPER_CASE}\n" )
file ( APPEND "mds.h" "#define MDS_GRADE_${MDS_GRADE_UPPER_CASE}\n" )
file ( APPEND "mds.h" "#define MDS_TARGET_${MDS_TARGET_UPPER_CASE}\n" )

if ( MDS_VARIANT STREQUAL "Trial" )
    file ( APPEND "mds.h" "#define MDS_TRIAL_PERIOD 30\n" )
endif()

foreach ( var   MDS_FEATURE_FILECONVERTER       MDS_FEATURE_DISASSEMBLER        MDS_FEATURE_TRANSLATOR
                MDS_FEATURE_CONVERTER_TOOL      MDS_FEATURE_LOOP_GENERATOR      MDS_FEATURE_8_SEGMENT_EDITOR
                MDS_FEATURE_SIM_LED_PANEL       MDS_FEATURE_SIM_7_SEGMENT       MDS_FEATURE_ADAPTABLE_SIMULATOR
                MDS_FEATURE_TRIAL               MDS_FEATURE_LICENCE_CERTIFICATE )

    if ( ${var} )
        file ( APPEND "mds.h" "#define ${var}\n" )
    endif()
endforeach()

set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "mds.h" )
