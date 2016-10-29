cmake_minimum_required(VERSION 3.6)
project(Task-manager)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp include/Task.hpp include/TaskManager.hpp include/Worker.hpp src/Worker.cpp src/TaskManager.cpp include/header.hpp)
add_executable(TaskManager ${SOURCE_FILES})
#add_library(TaskManager STATIC ${SOURCE_FILES})