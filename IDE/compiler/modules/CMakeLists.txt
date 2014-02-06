# ==============================================================================
#
# Compiler modules for the supported programming languages.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name.
project ( CompilerModules )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Compiler module for C language.
add_subdirectory ( C )

# Compiler modules for assembler.
add_subdirectory ( assembler )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}/utilities/DbgFile/"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../core/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/assembler/generators/" )

# Add library to the project using the specified source files.
add_library ( ${PROJECT_NAME} CompilerModules.cxx )

# Link target to given libraries.
target_link_libraries ( ${PROJECT_NAME} CompilerModuleC
                                        CompilerModulesAsm )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
