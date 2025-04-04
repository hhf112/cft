cmake_minimum_required(VERSION 3.20) 

project (cft )

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/app")

file (GLOB SRC_FILES "./src/*.cpp")

add_executable(cft ./main.cpp)

target_sources (cft PRIVATE ${SRC_FILES})
