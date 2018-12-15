cmake_minimum_required(VERSION 1.0.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(Intel.CloudFreed.FrameComposer.HpciFrameComposer)

# option to compile with debug or relese (example: cmake -D CMAKE_BUILD_TYPE=Debug ..)
if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

add_subdirectory(Intel.CloudFreed.FrameComposer.Example)

#add header files
include_directories(include)
include_directories(intel.cloudfreed.framecomposer.interfaces/include)

#add source files:
file(GLOB SOURCES "src/*.cpp")

#Generate the static library from the sources
add_library(Intel.CloudFreed.FrameComposer.HpciFrameComposer STATIC ${SOURCES})

#Set the location for library installation at /usr/local/lib
install(TARGETS Intel.CloudFreed.FrameComposer.HpciFrameComposer DESTINATION /usr/local/lib)