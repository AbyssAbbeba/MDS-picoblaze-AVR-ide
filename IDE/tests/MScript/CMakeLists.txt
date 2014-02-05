# ==============================================================================
#
# Unit tests for Moravia Script.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsMScript )

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

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../moraviascript"
                      "${CMAKE_CURRENT_SOURCE_DIR}/MScriptTestStrategy"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/AutoTest/" )

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
                        AutoTest )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

# add_test_subject ( "Moravia_Script" ${PROJECT_NAME} )
