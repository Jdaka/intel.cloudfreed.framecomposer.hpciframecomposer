cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

message(MMMMMMMMMMMMMMMMMMMM ${CMAKE_CURRENT_LIST_DIR})
#add header files
include_directories(include)
include_directories(intel.cloudfreed.framecomposer.interfaces/include)

#add source files:
file(GLOB SOURCES "*.cpp")

#Generate exe from the sources
add_executable(Intel.CloudFreed.FrameComposer.Example ${SOURCES})