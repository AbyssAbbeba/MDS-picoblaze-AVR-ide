# ==============================================================================
#
# Compiler semantic analyzer for assembly language for PicoBlaze.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleAsmPicoBlaze )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzers.
FLEX_BISON_PAIR ( AsmPicoBlazeLexer AsmPicoBlazeParser AsmPicoBlazeLexerParser )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}/compiler/core/"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../generators/" )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME}
              AsmPicoBlazeMacros.cxx
              AsmPicoBlazeMemoryPtr.cxx
              AsmPicoBlazeTreeDecoder.cxx
              AsmPicoBlazeSymbolTable.cxx
              AsmPicoBlazeCodeListing.cxx
              AsmPicoBlazeSpecialMacros.cxx
              AsmPicoBlazeCodeGenerator.cxx
              AsmPicoBlazeInstructionSet.cxx
              AsmPicoBlazeInstructionSetN.cxx
              AsmPicoBlazeInstructionSet1.cxx
              AsmPicoBlazeInstructionSet2.cxx
              AsmPicoBlazeInstructionSet3.cxx
              AsmPicoBlazeInstructionSet6.cxx
              AsmPicoBlazeSemanticAnalyzer.cxx
              AsmPicoBlazeInstructionSet1CPLD.cxx )

# Link a target to given libraries
target_link_libraries ( AsmPicoBlazeLexerParser CompilerCore )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        MCUDataFiles
                                        CompilerCore
                                        AsmPicoBlazeLexerParser
                                        CompilerModuleAsmGenerators )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
