
# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(CompilerCore)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Load settings specific for the Qt4 library (we use it here for i18n)
find_package( Qt4 REQUIRED )
include(${QT_USE_FILE})

# List of preprocessor include file search directories.
include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/../grammar"
)

# Specify directories in which the linker will look for libraries.
link_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/../grammar"
)

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions(-Wall -Wextra -std=c++0x -O2 -pipe)

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(${PROJECT_NAME} SHARED
	CompilerExpr.cxx
	CompilerStatement.cxx
	StatementTypes.cxx
	CompilerOptions.cxx
	CompilerCore.cxx
	CompilerMsgInftStdout.cxx
	Compiler.cxx
)

# Link a target to given libraries.
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} CompilerParsers)

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)
