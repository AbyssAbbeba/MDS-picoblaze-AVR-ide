# ==============================================================================
#
# Moravia Script language support library.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( MScript )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzers
include ( "${CMAKE_CURRENT_BINARY_DIR}/../FlexBisonPair.cmake" )
FLEX_BISON_PAIR ( MScriptLexer MScriptParser MScriptLexerParser )

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../utilities/os" )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              MScript.cxx
              MScriptCore.cxx
              MScriptBase.cxx
              MScriptExpr.cxx
              MScriptValue.cxx
              MScriptComplex.cxx
              MScriptVariable.cxx
              MScriptVarTable.cxx
              MScriptStrategy.cxx
              MScriptFuncTable.cxx
              MScriptInterpret.cxx
              MScriptStmtTypes.cxx
              MScriptStatement.cxx
              MScriptIdManager.cxx
              MScriptArrayIndex.cxx
              MScriptExprSolver.cxx
              MScriptNamespaces.cxx
              MScriptSerializer.cxx
              MScriptExprAlgebra.cxx
              MScriptSrcLocation.cxx
              MScriptExecContext.cxx
              MScriptSerializable.cxx
              MScriptLexerAuxFunc.cxx
              MScriptExprProcessor.cxx )

## Set properties on a target.
 # Targets can have properties that affect how they are built.
set_target_properties ( ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE "CXX" )

# Link a target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        MScriptLexerParser )

# DEBUG: testing executable
add_executable ( main main.cxx )
target_link_libraries ( main ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
