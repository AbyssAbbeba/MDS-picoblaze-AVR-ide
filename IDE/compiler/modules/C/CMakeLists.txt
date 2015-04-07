# ==============================================================================
#
# MDS Compiler module for the C language.
#
# (C) copyright 2015 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleC )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES & CONFIGURE FEATURES
# ------------------------------------------------------------------------------

# List of additional compiler modules which can be enabled or disabled for this particular build.
set ( C_LANG_BACKENDS "" )

# Compiler module for C language for PicoBlaze.
if ( MDS_FEATURE_PICOBLAZE )
    add_subdirectory ( PicoBlaze )
    list ( APPEND C_LANG_BACKENDS "CompilerModuleCPicoBlaze" )
endif()

# C language compiler core.
add_subdirectory ( core )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/compiler/core/"
                      "${CMAKE_SOURCE_DIR}/compiler/modules/C/core" )

# Add library to the project using the specified source files.
add_library ( ${PROJECT_NAME} CompilerModuleC.cxx )

# Link target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        ${C_LANG_BACKENDS}
                                        CompilerModuleCCore )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
