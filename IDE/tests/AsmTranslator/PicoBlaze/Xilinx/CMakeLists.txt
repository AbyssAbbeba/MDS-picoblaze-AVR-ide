# ==============================================================================
#
# Unit tests for PicoBlaze Simulator, test suite for single instructions.

# TAT_TestPicoBlazeXil: T = Test, AT = AsmTranslator, _<nameOfTheTestSuite>
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
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
include_directories ( "${CMAKE_SOURCE_DIR}/3rdParty"
                      "${CMAKE_SOURCE_DIR}"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../../compiler"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities/MCUDataFiles"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities/AsmTranslator" )

# Specify directories in which the linker will look for libraries.
link_directories (
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../../../compiler/core"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities/MCUDataFiles"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../../../utilities/AsmTranslator" )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              TestXilAsmTrans.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME}
                        boost
                        AsmTranslator
                        CompilerCore
                        MCUDataFiles
                        CUnit )
