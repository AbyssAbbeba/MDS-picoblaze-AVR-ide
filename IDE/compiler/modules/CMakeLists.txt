# ==============================================================================
#
# Compiler modules for the supported programming languages.
#
# (C) copyright 2013, 2014, 2015 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name.
project ( CompilerModules )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES & CONFIGURE FEATURES
# ------------------------------------------------------------------------------

# List of additional compiler modules which can be enabled or disabled for this particular build.
set ( MDS_ADDITONAL_MODULES "" )

# Compiler module for C language.
if ( MDS_FEATURE_C_COMPILER )
    add_subdirectory ( C )
    list ( APPEND MDS_ADDITONAL_MODULES "CompilerModuleC" )
endif()

# Compiler modules for assembler.
add_subdirectory ( assembler )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}/utilities/DbgFile/"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles/"
                      "${CMAKE_SOURCE_DIR}/utilities/AdjSimProcDef/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../core/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/assembler/common/" )

# Add library to the project using the specified source files.
add_library ( ${PROJECT_NAME} CompilerModules.cxx )

# Link target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${MDS_ADDITONAL_MODULES}
                                        CompilerModulesAsm )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
