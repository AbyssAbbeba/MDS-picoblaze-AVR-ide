# ==============================================================================
#
# Unit tests for PicoBlaze Simulator, test suite for single instructions.

# TAT_TestPicoBlazeXil: T = Test, AT = AsmTranslator, _<nameOfTheTestSuite>
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( TAT_TestPicoBlazeXil )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add include directories to the build.
include_directories ( "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_SOURCE_DIR}/compiler"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles"
                      "${CMAKE_SOURCE_DIR}/utilities/AsmTranslator" )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestXilAsmTrans.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES}
                                        AsmTranslator
                                        compiler
                                        MCUDataFiles
                                        CUnit )
