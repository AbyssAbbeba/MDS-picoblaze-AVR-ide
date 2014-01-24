##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project(Widgets)

# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

# Create verbose makefiles if on
set(CMAKE_VERBOSE_MAKEFILE OFF)

# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build the simulator widgets
add_subdirectory(sim)
#add_subdirectory(Tests)
#add_subdirectory(GraphWidget)
add_subdirectory(Counters)
add_subdirectory(DockManager)
add_subdirectory(Editor)
add_subdirectory(HexEdit)
#add_subdirectory(MovGridLayout)
add_subdirectory(ProjectTree)
add_subdirectory(PicoBlazeGrid)
#add_subdirectory(SimulationInfo)
add_subdirectory(Highlighter)
add_subdirectory(BookmarkList)
add_subdirectory(BreakpointList)
add_subdirectory(Tools)
add_subdirectory(TabBar)
add_subdirectory(Analyser)
add_subdirectory(AnalyserWidget)
#add_subdirectory(HWCanvas)
#add_subdirectory(CompileWidget)
add_subdirectory(CompileInfo)
add_subdirectory(StackWidget)
add_subdirectory(RegistersWidget)
add_subdirectory(PortHexEdit)
add_subdirectory(HelpWidget)
#add_subdirectory(ShowHideWidget)
