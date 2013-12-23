# ==============================================================================
#
# Unit tests for Moravia Script.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsMScript )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# After this command, you can add tests in the project using ADD_TEST command.
enable_testing()

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Testing strategy for the MScript language.
add_subdirectory ( MScriptTestStrategy )

# Test suite I.
add_subdirectory ( TestMScriptI )

# Test suite II.
add_subdirectory ( TestMScriptII )

# Test suite III.
add_subdirectory ( TestMScriptIII )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Find Boost include dirs and libraries.
find_package ( Boost 1.55
               REQUIRED
               COMPONENTS filesystem system )

# Add include directories to the build.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../moraviascript"
                      "${CMAKE_CURRENT_SOURCE_DIR}/MScriptTestStrategy" )

# Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS}
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../moraviascript"
                   "${CMAKE_CURRENT_SOURCE_DIR}/MScriptTestStrategy" )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        ${Boost_LIBRARIES}
                        TMS_TestMScriptI
                        TMS_TestMScriptII
                        TMS_TestMScriptIII
                        MScript
                        MScriptTestStrategy
                        cunit )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test ( NAME ${PROJECT_NAME}
           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME} )
