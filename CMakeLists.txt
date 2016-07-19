set(CMAKE_MACOSX_RPATH 1)
cmake_minimum_required(VERSION 2.8)
enable_testing()

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -Wall -g")
#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -Wall -DNDEBUG")

message(STATUS " project source directory = " ${PROJECT_SOURCE_DIR})

project (toystl)
add_subdirectory(src/test)
 