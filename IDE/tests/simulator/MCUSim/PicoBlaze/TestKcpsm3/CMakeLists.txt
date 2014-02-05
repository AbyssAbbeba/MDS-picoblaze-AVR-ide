# ==============================================================================
#
# Unit tests for PicoBlaze Simulator, test suite for single instructions.

# TSP_TestKcpsm3: T = Test, S = Simulator, P = PicoBlaze, _<nameOfTheTestSuite>
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TSP_TestKcpsm3 )

# List of additional files that will be cleaned as a part of the "make clean" stage.
file ( GLOB resultFiles results/*.* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles"
                      "${CMAKE_SOURCE_DIR}/simulators/MCUSim/PicoBlaze"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../.."
                      "${CMAKE_CURRENT_SOURCE_DIR}/../PicoBlazeStrategy" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestKcpsm3.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        PicoBlazeStrategy
                                        MCUSimTestScript
                                        PicoBlazeSim
                                        CUnit )

## Add a target with no output so it will always be built.
 # Compile .asm files for all testcases in the `testcases' directory
add_custom_target ( "${PROJECT_NAME}_compile" ALL
                    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/testcases/compileAll.sh
                    DEPENDS PicoBlazeAsmIncludeFiles_prc_file_kcpsm3
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/testcases"
                    COMMENT "Compiling .asm files for all testcases in `${CMAKE_CURRENT_SOURCE_DIR}/testcases'." )
