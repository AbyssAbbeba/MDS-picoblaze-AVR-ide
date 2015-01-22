# ==============================================================================
#
# Simulator libraries.
#
# (C) copyright 2013, 2014, 2015 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( Simulator )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build control simulator control unit
add_subdirectory ( SimControl )

# Build simulators for microprocessors and microcontrollers
add_subdirectory ( MCUSim )

# Build CLI simulator interface.
if ( MDS_FEATURE_CLI_TOOLS )
    add_subdirectory ( CommandLineTool )
endif()

# ------------------------------------------------------------------------------
# WINDRES RC FILES.
# Windows only: generate windres RC file(s) to provide information for Windows.
# ------------------------------------------------------------------------------

windres_rc ( OUTPUT_VAR         RC_FILE
             FILETYPE           "DLL"
             FILE_VERSION       ${PRODUCT_VERSION}
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS Simulator"
             COMPANY_NAME       ${COMPANY_NAME}
             TARGET             ${PROJECT_NAME}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

# ------------------------------------------------------------------------------
# CONFIGURE FEATURES
# ------------------------------------------------------------------------------

# List of additional simulator modules which can be enabled or disabled for this particular build.
set ( MDS_ADDITONAL_SIM_MODS "" )
set ( MDS_ADDITONAL_DEP_LIBS "" )

if ( MDS_FEATURE_AVR8 )
    list ( APPEND MDS_ADDITONAL_SIM_MODS $<TARGET_OBJECTS:AVR8Sim> )
endif()

if ( MDS_FEATURE_PIC8 )
    list ( APPEND MDS_ADDITONAL_SIM_MODS $<TARGET_OBJECTS:PIC8Sim> )
endif()

if ( MDS_FEATURE_PICOBLAZE )
    list ( APPEND MDS_ADDITONAL_SIM_MODS $<TARGET_OBJECTS:PicoBlazeSim> )
endif()

if ( MDS_FEATURE_ADAPTABLE_SIMULATOR )
    list ( APPEND MDS_ADDITONAL_SIM_MODS $<TARGET_OBJECTS:AdaptableSim> )
    list ( APPEND MDS_ADDITONAL_DEP_LIBS AdjSimProcDef )
endif()

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME} SHARED
              ${RC_FILE}

              # Simulators.
              ${MDS_ADDITONAL_SIM_MODS}

              # Support functions.
              $<TARGET_OBJECTS:SimControl>
              $<TARGET_OBJECTS:MCUSimulator>
              $<TARGET_OBJECTS:McuSimCfgMgr> )

# Link the target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        ${Boost_LIBRARIES}
                                        ${MDS_ADDITONAL_DEP_LIBS}
                                        DbgFile
                                        MCUDataFiles  )


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
