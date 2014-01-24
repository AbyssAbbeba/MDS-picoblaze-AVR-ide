# ==============================================================================
#
# Multi-target multi-language cross compiler.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( compiler )

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build lexical and syntax analyzers.
add_subdirectory ( modules )

# Build the compiler core as a library.
add_subdirectory ( core )

# Build additional file of the compiler.
add_subdirectory ( include )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/core"
                      "${CMAKE_CURRENT_SOURCE_DIR}/modules"
                      "${CMAKE_CURRENT_SOURCE_DIR}/modules/generators" )

## Create moc code from a list of files containing Qt class with the Q_OBJECT declaration.
 # Per-directory preprocessor definitions are also added.
QT4_WRAP_CPP ( QT_MOC_SRCS
               CompilerThread.h )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              ${QT_MOC_SRCS}
              Compiler.cxx
              CompilerThread.cxx )

# Link a target to given libraries
target_link_libraries ( ${PROJECT_NAME} CompilerModules )

# Add an executable to the project using the specified source files.
add_executable ( mds-${PROJECT_NAME}
                 main.cxx )

# Link a target to given libraries
target_link_libraries ( mds-${PROJECT_NAME} ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
