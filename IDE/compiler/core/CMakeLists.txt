# ==============================================================================
#
# Compiler core functinality.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
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

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}"
                      "${CMAKE_CURRENT_SOURCE_DIR}/../modules" )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/../modules"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/os"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/DbgFile/"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/MCUDataFiles/" )

## Create moc code from a list of files containing Qt class with the Q_OBJECT declaration.
 # Per-directory preprocessor definitions are also added.
QT4_WRAP_CPP ( QT_MOC_SRCS
               CompilerThread.h )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              ${QT_MOC_SRCS}
              Compiler.cxx
              CompilerBase.cxx
              CompilerCore.cxx
              CompilerExpr.cxx
              CompilerValue.cxx
              CompilerThread.cxx
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

# Link a target to given libraries.
target_link_libraries ( ${PROJECT_NAME}
                        ${QT_LIBRARIES}
                        ${Boost_LIBRARIES}
                        CompilerModules
                        os )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
