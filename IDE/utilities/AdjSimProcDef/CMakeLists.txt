# ==============================================================================
#
# Library for working with processor definition files used Adjustable Simulator.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( AdjSimProcDef )

# ------------------------------------------------------------------------------
# WINDRES RC FILES.
# Windows only: generate windres RC file(s) to provide information for Windows.
# ------------------------------------------------------------------------------

windres_rc ( OUTPUT_VAR         RC_FILE
             FILETYPE           "DLL"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS ASPD"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             ${PROJECT_NAME}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( ${CMAKE_SOURCE_DIR} )

# Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME} SHARED
              ${RC_FILE}
              AdjSimProcDef.cxx
              AdjSimProcDefGui.cxx
              AdjSimProcDefParser.cxx
              AdjSimProcDefGenerator.cxx )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
