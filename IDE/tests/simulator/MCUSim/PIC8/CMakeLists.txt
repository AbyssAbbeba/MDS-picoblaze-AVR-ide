# ==============================================================================
#
# Unit tests for PIC8 Simulator.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsMCUSimPIC8 )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Test suite: Single instructions.
add_subdirectory ( TestGenericSubsys )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}/3rdParty"
                      "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/utilities/AutoTest/" )

# Specify directories in which the linker will look for libraries.
link_directories (
                   "${CMAKE_SOURCE_DIR}/utilities/AutoTest/" )

# Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        boost
                        TSC_TestGenericSubsys
                        AutoTest )

# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

# ADD_TEST_SUBJECT ( "MCU_Simulator_PIC8" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}" )
