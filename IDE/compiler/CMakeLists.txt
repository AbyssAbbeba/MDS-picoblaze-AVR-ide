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

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

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

# Find Boost include dirs and libraries.
find_package ( Boost 1.55
               REQUIRED
               COMPONENTS filesystem system )

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# List of preprocessor include file search directories.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_CURRENT_SOURCE_DIR}/core"
                      "${CMAKE_CURRENT_SOURCE_DIR}/modules"
                      "${CMAKE_CURRENT_SOURCE_DIR}/modules/generators" )

# Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS}
                   "${CMAKE_CURRENT_SOURCE_DIR}/core/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/modules/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../utilities/os/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../utilities/DbgFile/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../utilities/MCUDataFiles/" )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        CompilerCore )


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
