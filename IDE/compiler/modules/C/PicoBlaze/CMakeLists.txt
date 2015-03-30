# ==============================================================================
#
# MDS Compiler module for the C language for PicoBlaze.
#
# (C) copyright 2015 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleCPicoBlaze )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/compiler/core/"
                      "${CMAKE_SOURCE_DIR}/compiler/modules/C/core" )

## Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME}
              CompilerCPicoBlazeBackend.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} CompilerCore )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
