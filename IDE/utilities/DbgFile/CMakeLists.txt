# ==============================================================================
#
# Library for working with  SDCC's .cdb files, and other kinds "debugger" files.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( DbgFile )

# ------------------------------------------------------------------------------
# WINDRES RC FILES.
# Windows only: generate windres RC file(s) to provide information for Windows.
# ------------------------------------------------------------------------------

windres_rc ( OUTPUT_VAR         RC_FILE
             FILETYPE           "DLL"
             FILE_VERSION       ${PRODUCT_VERSION}
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS Debugging Files Support"
             COMPANY_NAME       ${COMPANY_NAME}
             TARGET             ${PROJECT_NAME}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/.." )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              ${RC_FILE}
              DbgFile.cxx
              DbgFileCDB.cxx
              DbgFileNative.cxx
              DbgFileAvrCoff.cxx
              DbgFileAvraLst.cxx )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
