
##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(SimWidgets)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

qt4_wrap_cpp(QT_MOC_GENERATED_CPP_FILES
#	RegDisplay.h
#	RegDisplayList.h
	McuMemoryView.h

	${CMAKE_CURRENT_BINARY_DIR}/../HexEdit/hexedit.h
	${CMAKE_CURRENT_BINARY_DIR}/../Counters/wlinecounter.h
	${CMAKE_CURRENT_BINARY_DIR}/../Counters/wcolumncounter.h
)

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(${PROJECT_NAME}
	${QT_MOC_GENERATED_CPP_FILES}
#	RegDisplay.cxx
#	RegDisplayList.cxx
	McuMemoryView.cxx

	${CMAKE_CURRENT_BINARY_DIR}/../HexEdit/hexedit.cpp
	${CMAKE_CURRENT_BINARY_DIR}/../Counters/wlinecounter.cpp
	${CMAKE_CURRENT_BINARY_DIR}/../Counters/wcolumncounter.cpp)

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} SimControl McuSimCfgMgr PicoBlazeSim MCUDataFiles)

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------


if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()