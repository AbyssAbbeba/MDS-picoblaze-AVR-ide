# ==============================================================================
#
# Compiler semantic analyzer for Adaptable Assembler.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleAsmAdaptable )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzers.
flex_bison_pair ( AsmAdaptableLexer AsmAdaptableParser AsmAdaptableLexerParser )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_SOURCE_DIR}/compiler/core/"
                      "${CMAKE_SOURCE_DIR}/utilities/MCUDataFiles/"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../common/" )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME}
#               AsmPicoBlazeMacros.cxx
#               AsmPicoBlazeMemoryPtr.cxx
#               AsmPicoBlazeTreeDecoder.cxx
#               AsmPicoBlazeSymbolTable.cxx
#               AsmPicoBlazeCodeListing.cxx
#               AsmPicoBlazeCodeGenerator.cxx
              AsmAdaptableSemanticAnalyzer.cxx )

# Link a target to given libraries
target_link_libraries ( AsmAdaptableLexerParser CompilerCore )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        MCUDataFiles
                                        CompilerCore
                                        AsmAdaptableLexerParser
                                        CompilerModuleAsmCommon )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()
