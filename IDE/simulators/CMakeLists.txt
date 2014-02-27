# ==============================================================================
#
# Simulator libraries.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
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

# Build simulators for microcontrollers
add_subdirectory ( MCUSim )

# ------------------------------------------------------------------------------
# WINDRES RC FILES.
# Windows only: generate windres RC file(s) to provide information for Windows.
# ------------------------------------------------------------------------------

windres_rc ( OUTPUT_VAR         RC_FILE
             FILETYPE           "DLL"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS Simulator"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             ${PROJECT_NAME}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME} SHARED
              ${RC_FILE}

              # Simulators.
              $<TARGET_OBJECTS:AVR8Sim>
              $<TARGET_OBJECTS:PIC8Sim>
              $<TARGET_OBJECTS:PicoBlazeSim>

              # Support functions.
              $<TARGET_OBJECTS:SimControl>
              $<TARGET_OBJECTS:McuSimCfgMgr> )

# Link the target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        DbgFile
                                        MCUDataFiles  )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
