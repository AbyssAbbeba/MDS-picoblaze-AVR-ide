# ==============================================================================
#
# Compiler core functinality.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CompilerCore )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              CompilerBase.cxx
              CompilerCore.cxx
              CompilerExpr.cxx
              CompilerValue.cxx
              CompilerOptions.cxx
              CompilerStatement.cxx
              CompilerMsgFilter.cxx
              CompilerLexerUtils.cxx
              CompilerSerializer.cxx
              CompilerMsgIntfFile.cxx
              CompilerSerializable.cxx
              CompilerMessageStack.cxx
              CompilerMsgIntfStdout.cxx
              CompilerSourceLocation.cxx
              CompilerStatementTypes.cxx
              CompilerLocationTracker.cxx )

# Link the target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${QT_LIBRARIES}
                                        ${Boost_LIBRARIES}
                                        os )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION ${INSTALL_DIR_BIN}
          LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
