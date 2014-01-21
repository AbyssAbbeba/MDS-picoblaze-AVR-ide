# ==============================================================================
#
# Multi-target multi-language cross compiler.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( compiler )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build lexical and syntax analyzers.
add_subdirectory ( modules )

# Build the compiler collection itself as a library.
add_subdirectory ( core )

# Build additional file of the compiler.
add_subdirectory ( include )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}/3rdParty"
                      "${CMAKE_CURRENT_SOURCE_DIR}/core"
                      "${CMAKE_CURRENT_SOURCE_DIR}/modules"
                      "${CMAKE_CURRENT_SOURCE_DIR}/modules/generators" )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/core/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/modules/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../utilities/os/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../utilities/DbgFile/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../utilities/MCUDataFiles/" )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} boost
                                        CompilerCore )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
