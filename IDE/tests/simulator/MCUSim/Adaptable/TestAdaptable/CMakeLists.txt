# ==============================================================================
#
# Unit tests for Adaptable Simulator, test suite for single instructions.

# TSA_Adaptable: T = Test, S = Simulator, A = Adaptable, _<nameOfTheTestSuite>
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TSA_Adaptable )

# List of additional files that will be cleaned as a part of the "make clean" stage.
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "results" )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/utilities/"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles"
                      "${CMAKE_SOURCE_DIR}/simulators/MCUSim/McuSimCfgMgr"
                      "${CMAKE_SOURCE_DIR}/simulators/MCUSim/AdaptableSim"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../.."
                      "${CMAKE_CURRENT_SOURCE_DIR}/../AdaptableStrategy" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestAdaptable.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        AdaptableStrategy
                                        CUnit )

## Add a target with no output so it will always be built.
 # Compile .asm files for all testcases in the `testcases' directory
add_custom_target ( "${PROJECT_NAME}_compile" ALL
                    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/testcases/compileAll.sh
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/testcases"
                    COMMENT "Compiling .asm files for all testcases in `${CMAKE_CURRENT_SOURCE_DIR}/testcases'." )
