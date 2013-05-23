# ==============================================================================
#
# Unit tests for PicoBlaze Assembler.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( testsAssemblerPicoBlaze )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )

# After this command, you can add tests in the project using ADD_TEST command.
enable_testing()


# # ------------------------------------------------------------------------------
# # COMPILATION OPTIONS
# # ------------------------------------------------------------------------------
# 
# # Specify directories in which the linker will look for libraries.
# link_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../../utilities/MCUDataFiles"
#                    "${CMAKE_CURRENT_BINARY_DIR}/../../simulators/MCUSim/AVR8"
#                    "${CMAKE_CURRENT_BINARY_DIR}/../../simulators/MCUSim/McuSimCfgMgr" )
# 
# # Add include directories to the build.
# include_directories ( "${CMAKE_CURRENT_BINARY_DIR}/../../utilities/MCUDataFiles"
#                       "${CMAKE_CURRENT_BINARY_DIR}/../../simulators/MCUSim"
#                       "${CMAKE_CURRENT_BINARY_DIR}/../../simulators/MCUSim/AVR8"
#                       "${CMAKE_CURRENT_BINARY_DIR}/../../simulators/MCUSim/McuSimCfgMgr" )
# 
# # Include Qt4's XML module
# find_package ( Qt4 REQUIRED )
# set ( QT_USE_QTXML 1 )
# include ( ${QT_USE_FILE} )
# 
# ## Add switches to compiler command line
#  # -pipe       : Use pipes rather than temporary files for communication between the various stages of compilation.
#  # -std=c++0x  : Default template arguments may not be used in function templates without -std=c++0x or -std=gnu++0x.
#  # -O1         : Optimization, level 1.
# add_definitions ( -pipe -std=c++0x -O1 )
# 
# # Add an executable to the project using the specified source files.
add_executable ( ${PROJECT_NAME}
                 main.cxx )
# 
# # Link a target to given libraries
# target_link_libraries ( ${PROJECT_NAME}
#                         ${QT_LIBRARIES}
#                         cunit
#                         McuSimCfgMgr
#                         AVR8Sim
#                         MCUDataFiles )
# 
# 
# # ------------------------------------------------------------------------------
# # TESTS
# # ------------------------------------------------------------------------------
# 
# add_test ( NAME ${PROJECT_NAME}
#            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} )
