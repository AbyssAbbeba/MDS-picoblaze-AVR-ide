
## Simulator engine module for 8-bit AVR

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(AVR8_test)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)

# After this command, you can add tests in the project using ADD_TEST command.
enable_testing()

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Specify directories in which the linker will look for libraries.
link_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/../../utilities/MCUDataFiles"
	"${CMAKE_CURRENT_BINARY_DIR}/../../simulators/MCUSim/AVR8")

## Add switches to compiler command line
 # -Wall       : Enables all the warnings.
 # -Wextra     : Enables some extra warning flags that are not enabled by -Wall.
 # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
 # -O2         : Optimization, level 2.
 # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
add_definitions(-Wall -Wextra -std=c++0x -O2 -pipe -I../../utilities/ -I../../simulators/MCUSim)

# Add an executable to the project using the specified source files.
add_executable(${PROJECT_NAME}
	${PROJECT_NAME}.cxx
)

# Add a dependency between top-level targets.
add_dependencies(${PROJECT_NAME} AVR8Sim MCUDataFiles)

# Link a target to given libraries.
target_link_libraries(${PROJECT_NAME} AVR8Sim MCUDataFiles)


# ------------------------------------------------------------------------------
# TESTS
# ------------------------------------------------------------------------------

add_test(NAME SimpleTest0 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} Hello)
add_test(NAME SimpleTest1 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} Hello)
add_test(NAME SimpleTest2 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} Hello)

