# ==============================================================================
#
# Unit tests for PicoBlaze Simulator, test suite for single instructions.

# TSP_TestKcpsm1: T = Test, S = Simulator, P = PicoBlaze, _<nameOfTheTestSuite>
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TSP_TestKcpsm1 )

# List of additional files that will be cleaned as a part of the "make clean" stage.
file ( GLOB resultFiles results/*.* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../simulators/MCUSim/PicoBlaze"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../utilities/MCUDataFiles"
                      "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../.." )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestKcpsm1.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        MCUSimTestScript
                                        PicoBlazeSim
                                        CUnit )

## Add a target with no output so it will always be built.
 # Compile .asm files for all testcases in the `testcases' directory
add_custom_target ( "${PROJECT_NAME}_compile" ALL
                    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testcases/compileAll.sh
                    DEPENDS PicoBlazeAsmIncludeFiles_prc_file_kcpsm1
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/testcases"
                    COMMENT "Compiling .asm files for all testcases in `${CMAKE_CURRENT_SOURCE_DIR}/testcases'." )
