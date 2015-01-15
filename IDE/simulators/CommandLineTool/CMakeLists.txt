# ==============================================================================
#
# CLI simulator interface.
#
# (C) copyright 2015 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( mds-proc-sim )

# ------------------------------------------------------------------------------
# WINDRES RC FILES.
# Windows only: generate windres RC file(s) to provide information for Windows.
# ------------------------------------------------------------------------------

windres_rc ( OUTPUT_VAR         RC_FILE
             FILETYPE           "APP"
             FILE_VERSION       ${PRODUCT_VERSION}
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS Processor Simulator"
             ICON               "windres/mds-simulator.ico"
             COMPANY_NAME       ${COMPANY_NAME}
             TARGET             ${PROJECT_NAME}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}" )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME} ${RC_FILE} main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} Simulator )


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
