# ==============================================================================
#
# Assembly language support library, this tool translates between various
# dialects of a particular Assembly language,
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( AsmTranslator )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Find Boost include directories and libraries.
find_package ( Boost 1.46
               REQUIRED
               COMPONENTS regex filesystem system )

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

# Add include directories to the build.
include_directories ( ${Boost_INCLUDE_DIRS} )

# Specify directories in which the linker will look for libraries.
link_directories ( ${Boost_LIBRARY_DIRS} )

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions ( -Wall -std=c++0x -O2 -pipe )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              AsmTranslator.cxx
              AsmTranslatorBase.cxx
              AsmTranslatorConfig.cxx
              AsmTranslatorKcpsmXil.cxx )

# Link a target to given libraries.
target_link_libraries ( ${PROJECT_NAME} ${Boost_LIBRARIES} ${QT_LIBRARIES} )

# Main executable.
add_executable ( AsmTransTool main.cxx )
target_link_libraries ( AsmTransTool ${PROJECT_NAME} )

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install ( TARGETS ${PROJECT_NAME}
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib )
