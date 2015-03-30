# ======================================================================================================================
#
# Configure MDS fetures and generate "mds.h".
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ======================================================================================================================

# ----------------------------------------------------------------------------------------------------------------------
# MDS fetures - configuration variables.
# ----------------------------------------------------------------------------------------------------------------------

set ( MDS_FEATURES

      MDS_FEATURE_FILECONVERTER       MDS_FEATURE_DISASSEMBLER        MDS_FEATURE_TRANSLATOR
      MDS_FEATURE_CONVERTER_TOOL      MDS_FEATURE_LOOP_GENERATOR      MDS_FEATURE_8_SEGMENT_EDITOR
      MDS_FEATURE_SIM_LED_PANEL       MDS_FEATURE_SIM_7_SEGMENT       MDS_FEATURE_ADAPTABLE_SIMULATOR
      MDS_FEATURE_CLI_TOOLS           MDS_FEATURE_TRIAL               MDS_FEATURE_LICENCE_CERTIFICATE
      MDS_FEATURE_PICOBLAZE           MDS_FEATURE_AVR8                MDS_FEATURE_PIC8
      MDS_FEATURE_MCS51               MDS_FEATURE_C_COMPILER          MDS_FEATURE_C_TESTBENCH
      MDS_FEATURE_SIM_SWITCH          MDS_FEATURE_SIM_PORT_LOGGER     MDS_FEATURE_VHDL_WIZARD
      MDS_FEATURE_EXTERNAL_APPS       MDS_FEATURE_SIM_CALLWATCHER     MDS_FEATURE_SIM_REGWATCHER
      MDS_FEATURE_COMPATIBILITY_MODE  MDS_FEATURE_SYMBOLTABLE         MDS_FEATURE_SIM_STOPWATCH )

# ----------------------------------------------------------------------------------------------------------------------
# Set internal configuration variables.
# ----------------------------------------------------------------------------------------------------------------------

# Disable "all" fetures.
foreach ( var ${MDS_FEATURES} )
    set ( ${var} FALSE )
endforeach()

# Trial period [days] (valid only for trial version).
set ( MDS_TRIAL_PERIOD 30 )

if ( MDS_VARIANT STREQUAL "Trial" )
    set ( MDS_FEATURE_TRIAL TRUE )
else()
    set ( MDS_FEATURE_LICENCE_CERTIFICATE TRUE )
endif()

if ( MDS_GRADE STREQUAL "Basic" )
    set ( MDS_FEATURE_CONVERTER_TOOL            TRUE )
    set ( MDS_FEATURE_8_SEGMENT_EDITOR          TRUE )
    set ( MDS_FEATURE_TRANSLATOR                TRUE )

elseif ( MDS_GRADE STREQUAL "Premium" )
    set ( MDS_FEATURE_CONVERTER_TOOL            TRUE )
    set ( MDS_FEATURE_8_SEGMENT_EDITOR          TRUE )
    set ( MDS_FEATURE_CLI_TOOLS                 TRUE )
    set ( MDS_FEATURE_FILECONVERTER             TRUE )
    set ( MDS_FEATURE_DISASSEMBLER              TRUE )
    set ( MDS_FEATURE_TRANSLATOR                TRUE )
    set ( MDS_FEATURE_LOOP_GENERATOR            TRUE )
    set ( MDS_FEATURE_SIM_LED_PANEL             TRUE )
    set ( MDS_FEATURE_SIM_7_SEGMENT             TRUE )
    set ( MDS_FEATURE_SIM_SWITCH                TRUE )
    set ( MDS_FEATURE_EXTERNAL_APPS             TRUE )
    set ( MDS_FEATURE_VHDL_WIZARD               TRUE )
    set ( MDS_FEATURE_SIM_CALLWATCHER           TRUE )
    set ( MDS_FEATURE_SIM_REGWATCHER            TRUE )
    set ( MDS_FEATURE_COMPATIBILITY_MODE        TRUE )
    set ( MDS_FEATURE_SYMBOLTABLE               TRUE )
    set ( MDS_FEATURE_SIM_STOPWATCH             TRUE )

elseif ( MDS_GRADE STREQUAL "Professional" )
    set ( MDS_FEATURE_CONVERTER_TOOL            TRUE )
    set ( MDS_FEATURE_8_SEGMENT_EDITOR          TRUE )
    set ( MDS_FEATURE_CLI_TOOLS                 TRUE )
    set ( MDS_FEATURE_FILECONVERTER             TRUE )
    set ( MDS_FEATURE_DISASSEMBLER              TRUE )
    set ( MDS_FEATURE_TRANSLATOR                TRUE )
    set ( MDS_FEATURE_LOOP_GENERATOR            TRUE )
    set ( MDS_FEATURE_SIM_LED_PANEL             TRUE )
    set ( MDS_FEATURE_SIM_7_SEGMENT             TRUE )
    set ( MDS_FEATURE_SIM_SWITCH                TRUE )
    set ( MDS_FEATURE_ADAPTABLE_SIMULATOR       TRUE )
   # set ( MDS_FEATURE_C_COMPILER                TRUE )
    set ( MDS_FEATURE_EXTERNAL_APPS             TRUE )
    set ( MDS_FEATURE_VHDL_WIZARD               TRUE )
    set ( MDS_FEATURE_SIM_CALLWATCHER           TRUE )
    set ( MDS_FEATURE_SIM_REGWATCHER            TRUE )
    set ( MDS_FEATURE_COMPATIBILITY_MODE        TRUE )
    set ( MDS_FEATURE_SYMBOLTABLE               TRUE )
    set ( MDS_FEATURE_SIM_STOPWATCH             TRUE )

elseif ( MDS_GRADE STREQUAL "Ultimate" )
    set ( MDS_FEATURE_CONVERTER_TOOL            TRUE )
    set ( MDS_FEATURE_8_SEGMENT_EDITOR          TRUE )
    set ( MDS_FEATURE_CLI_TOOLS                 TRUE )
    set ( MDS_FEATURE_FILECONVERTER             TRUE )
    set ( MDS_FEATURE_DISASSEMBLER              TRUE )
    set ( MDS_FEATURE_TRANSLATOR                TRUE )
    set ( MDS_FEATURE_LOOP_GENERATOR            TRUE )
    set ( MDS_FEATURE_SIM_LED_PANEL             TRUE )
    set ( MDS_FEATURE_SIM_7_SEGMENT             TRUE )
    set ( MDS_FEATURE_SIM_SWITCH                TRUE )
    set ( MDS_FEATURE_SIM_PORT_LOGGER           TRUE )
    set ( MDS_FEATURE_ADAPTABLE_SIMULATOR       TRUE )
  #  set ( MDS_FEATURE_C_COMPILER                TRUE )
    set ( MDS_FEATURE_C_TESTBENCH               TRUE )
    set ( MDS_FEATURE_EXTERNAL_APPS             TRUE )
    set ( MDS_FEATURE_VHDL_WIZARD               TRUE )
    set ( MDS_FEATURE_SIM_CALLWATCHER           TRUE )
    set ( MDS_FEATURE_SIM_REGWATCHER            TRUE )
    set ( MDS_FEATURE_COMPATIBILITY_MODE        TRUE )
    set ( MDS_FEATURE_SYMBOLTABLE               TRUE )
    set ( MDS_FEATURE_SIM_STOPWATCH             TRUE )
endif()

if ( MDS_TARGET STREQUAL "PicoBlaze" )
    set ( MDS_FEATURE_PICOBLAZE  TRUE )
elseif ( MDS_TARGET STREQUAL "AVR8" )
    set ( MDS_FEATURE_AVR8       TRUE )
elseif ( MDS_TARGET STREQUAL "PIC8" )
    set ( MDS_FEATURE_PIC8       TRUE )
elseif ( MDS_TARGET STREQUAL "MCS51" )
    set ( MDS_FEATURE_MCS51      TRUE )
elseif ( MDS_TARGET STREQUAL "All" )
    set ( MDS_FEATURE_PICOBLAZE  TRUE )
    set ( MDS_FEATURE_AVR8       TRUE )
    set ( MDS_FEATURE_PIC8       TRUE )
    set ( MDS_FEATURE_MCS51      TRUE )
else()
    message ( SEND_ERROR "MDS_TARGET = ${MDS_TARGET} will not configure any target processors." )
endif()

# ----------------------------------------------------------------------------------------------------------------------
# (RE-)GENERATE "mds.h"
# ----------------------------------------------------------------------------------------------------------------------

set ( MDS_H "// Basic definitions for MDS, generated automatically by CMake.\n" )

set ( MDS_H "${MDS_H}#define MDS_VERSION \"${PRODUCT_VERSION}\"\n" )
set ( MDS_H "${MDS_H}#define PRODUCT_VERSION_MAJOR ${PRODUCT_VERSION_MAJOR}\n" )
set ( MDS_H "${MDS_H}#define PRODUCT_VERSION_MINOR ${PRODUCT_VERSION_MINOR}\n" )
set ( MDS_H "${MDS_H}#define PRODUCT_VERSION_PATCH ${PRODUCT_VERSION_PATCH}\n" )
set ( MDS_H "${MDS_H}#define MDS_VARIANT_${MDS_VARIANT_UPPER_CASE}\n" )
set ( MDS_H "${MDS_H}#define MDS_GRADE_${MDS_GRADE_UPPER_CASE}\n" )
set ( MDS_H "${MDS_H}#define MDS_TARGET_${MDS_TARGET_UPPER_CASE}\n" )

if ( ( MDS_VARIANT STREQUAL "Noncommercial" ) AND ( MDS_GRADE STREQUAL "Basic" ) )
    set ( MDS_H "${MDS_H}#define MDS_CODE_LIMITATION 256\n" )
endif()

if ( MDS_VARIANT STREQUAL "Trial" )
    set ( MDS_H "${MDS_H}#define MDS_TRIAL_PERIOD ${MDS_TRIAL_PERIOD}\n" )
endif()

foreach ( var ${MDS_FEATURES} )
    if ( ${var} )
        set ( MDS_H "${MDS_H}#define ${var}\n" )
    endif()
endforeach()


if ( EXISTS "mds.h" )
    file ( READ "mds.h" MDS_H_READ )
    string ( COMPARE EQUAL "${MDS_H}" "${MDS_H_READ}" MDS_H_DONT_UPDATE )
else()
    set ( MDS_H_DONT_UPDATE FALSE )
endif()

if ( NOT MDS_H_DONT_UPDATE )
    file ( WRITE "mds.h" "${MDS_H}" )
endif()

set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "mds.h" )
