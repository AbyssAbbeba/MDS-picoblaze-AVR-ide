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
project ( compiler )

# Target name for the compilers's CLI executable binary.
set ( COMPILER_EXEC_TARGET "mds-${PROJECT_NAME}" )

# ------------------------------------------------------------------------------
# WINDRES RC FILES.
# Windows only: generate windres RC file(s) to provide information for Windows.
# ------------------------------------------------------------------------------

windres_rc ( OUTPUT_VAR         LIB_RC_FILE
             FILETYPE           "DLL"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS Compiler"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             ${PROJECT_NAME}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

windres_rc ( OUTPUT_VAR         EXEC_RC_FILE
             FILETYPE           "APP"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS Compiler"
             ICON               "windres/mds-compiler.ico"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             ${COMPILER_EXEC_TARGET}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/core"
                      "${CMAKE_CURRENT_SOURCE_DIR}/modules"
                      "${CMAKE_CURRENT_SOURCE_DIR}/modules/common" )

## Create moc code from a list of files containing Qt class with the Q_OBJECT declaration.
 # Per-directory preprocessor definitions are also added.
QT4_WRAP_CPP ( QT_MOC_SRCS CompilerThread.h )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              ${QT_MOC_SRCS}
              ${LIB_RC_FILE}
              Compiler.cxx
              CompilerThread.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} CompilerModules )

# Add an executable to the project using the specified source files.
add_executable ( ${COMPILER_EXEC_TARGET} ${EXEC_RC_FILE} main.cxx )

# Link a target to given libraries
target_link_libraries ( ${COMPILER_EXEC_TARGET} ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build lexical and syntax analyzers.
add_subdirectory ( modules )

# Build the compiler core as a library.
add_subdirectory ( core )

# Build additional file of the compiler.
add_subdirectory ( include )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )

if ( MDS_FEATURE_CLI_TOOLS )
    install ( TARGETS ${COMPILER_EXEC_TARGET}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
