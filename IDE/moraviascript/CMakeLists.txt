# ==============================================================================
#
# Moravia Script language support library.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( MScript )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Generate lexical and syntax analyzers.
FLEX_BISON_PAIR ( MScriptLexer MScriptParser MScriptLexerParser )

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
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

# Link the target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        MScriptLexerParser )

# DEBUG: testing executable
add_executable ( main main.cxx )
target_link_libraries ( main ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
