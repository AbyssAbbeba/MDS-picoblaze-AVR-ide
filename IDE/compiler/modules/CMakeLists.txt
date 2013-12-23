# ==============================================================================
#
# Compiler modules for the supported programming languages.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModules )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )


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

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../core/"
                      "${CMAKE_CURRENT_BINARY_DIR}/assembler/generators/"
                      "${CMAKE_CURRENT_BINARY_DIR}/../../utilities/DbgFile/"
                      "${CMAKE_CURRENT_BINARY_DIR}/../../utilities/MCUDataFiles/" )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME} SHARED
              CompilerModules.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} CompilerModulesAsm
                                        CompilerModuleC
                                        MCUDataFiles
                                        DbgFile )


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
