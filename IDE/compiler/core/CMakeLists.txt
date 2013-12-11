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

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# Find Boost include directories and libraries.
find_package ( Boost 1.46
               REQUIRED
               COMPONENTS filesystem system )

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../grammar"
                      "${CMAKE_CURRENT_BINARY_DIR}/asm/avr8"
                      "${CMAKE_CURRENT_BINARY_DIR}/asm/mcs51"
                      "${CMAKE_CURRENT_BINARY_DIR}/asm/pic8"
                      "${CMAKE_CURRENT_BINARY_DIR}/asm/PicoBlaze" )

# Specify directories in which the linker will look for libraries.
link_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../grammar"
                   "${CMAKE_CURRENT_BINARY_DIR}/../../utilities/os" )

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
 # -fPIC       : Generate position-independent code (PIC) suitable for use in a shared library
add_definitions ( -Wall -Wextra -std=c++0x -O2 -pipe -fPIC )

## Create moc code from a list of files containing Qt class with the Q_OBJECT declaration.
 # Per-directory preprocessor definitions are also added.
QT4_WRAP_CPP ( QT_MOC_SRCS
               CompilerThread.h )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} STATIC
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
              CompilerStatementTypes.cxx )

# Link a target to given libraries.
target_link_libraries ( ${PROJECT_NAME}
                        ${QT_LIBRARIES}
                        ${Boost_LIBRARIES}
                        CompilerModules
                        os )


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

# install ( TARGETS ${PROJECT_NAME}
#           RUNTIME DESTINATION bin
#           LIBRARY DESTINATION lib )
