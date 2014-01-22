# ==============================================================================
#
# Unit tests for MScript language.

# TMS_TestMScriptIII: T = Test, MS = MScript, _<nameOfTheTestSuite>
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TMS_TestMScriptIII )

# List of additional files that will be cleaned as a part of the "make clean" stage.
FILE ( GLOB resultFiles results/*.* )
set_directory_properties ( PROPERTIES
                           ADDITIONAL_MAKE_CLEAN_FILES "${resultFiles}" )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( ${Boost_INCLUDE_DIRS}
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../moraviascript"
                      "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../MScriptTestStrategy" )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../../../moraviascript"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../MScriptTestStrategy" )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestMScriptIII.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        ${Boost_LIBRARIES}
                        MScript
                        MScriptTestStrategy
                        CUnit )
