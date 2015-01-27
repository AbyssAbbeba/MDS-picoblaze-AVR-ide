##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(RegWatcher)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)



# here we define SAMPLE_SRCS variable that contains a list of all .cpp files
SET( SAMPLE_SRCS
	./regwatcher.cpp
    ./regwatcherwidget.cpp
    ./regwatcherdialog.cpp
)

# another list, this time it includes all header files that should be treated with moc
SET( SAMPLE_MOC_HDRS
    ./regwatcher.h
	./regwatcherwidget.h
    ./regwatcherdialog.h
)

# some .ui files
SET( SAMPLE_UIS
    ./regwatcher.ui
    ./regwatcherwidget.ui
    ./regwatcherdialog.ui
)


# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# this will run uic on .ui files:
QT4_WRAP_UI( SAMPLE_UI_HDRS ${SAMPLE_UIS} )

# and finally this will run moc:
QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library(${PROJECT_NAME}
	${SAMPLE_MOC_SRCS} ${SAMPLE_SRCS} ${SAMPLE_UI_HDRS})


target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} Simulator)


# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------



if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()