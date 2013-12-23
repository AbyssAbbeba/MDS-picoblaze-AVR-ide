# ==============================================================================
#
# Compiler semantic analyzer for assembly language for PicoBlaze.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerModuleAsmPicoBlaze )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles, if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzers
include ( "${CMAKE_CURRENT_BINARY_DIR}/../../../../FlexBisonPair.cmake" )
FLEX_BISON_PAIR ( AsmPicoBlazeLexer AsmPicoBlazeParser AsmPicoBlazeLexerParser )

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../generators/"
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../core/"
                      "${CMAKE_CURRENT_BINARY_DIR}/../../../../utilities/MCUDataFiles/" )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../generators/"
                   "${CMAKE_CURRENT_BINARY_DIR}/../../../../utilities/MCUDataFiles/" )

## Add a library to the project using the specified source files,
 # in this case it will procude static C++ library.
add_library ( ${PROJECT_NAME} SHARED
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
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        MCUDataFiles
                                        AsmPicoBlazeLexerParser
                                        CompilerModuleAsmGenerators )

## Set properties on a target.
 # Targets can have properties that affect how they are built.
set_target_properties ( ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE "CXX" )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
