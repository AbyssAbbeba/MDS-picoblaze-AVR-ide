# ==============================================================================
#
# Unit tests for PIC8 Simulator, test suite for single instructions.

# TSC_TestGenericSubsys: T = Test, S = Simulator, C = PIC8, _<nameOfTheTestSuite>
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TSC_TestGenericSubsys )

# List of additional files that will be cleaned as a part of the "make clean" stage.
FILE ( GLOB resultFiles results/* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../.."
                      "${CMAKE_SOURCE_DIR}/simulators/MCUSim/PIC8"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestGenericSubsys.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        MCUSimTestScript
                                        PIC8Sim
                                        CUnit )

## Add a target with no output so it will always be built.
 # Compile .asm files for all testcases in the `testcases' directory
add_custom_target ( "${PROJECT_NAME}_compile" ALL
                    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/testcases/compileAll.sh
                    DEPENDS mds-compiler
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/testcases"
                    COMMENT "Compiling .asm files for all testcases in `${CMAKE_CURRENT_SOURCE_DIR}/testcases'." )
