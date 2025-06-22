cmake_minimum_required(VERSION 3.31.5)

set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(cft)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/app/)

file (GLOB APP_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(cft 
  ${CMAKE_SOURCE_DIR}/main.cpp
  ${APP_SOURCES})

add_executable(server ${CMAKE_SOURCE_DIR}/server.cpp)
target_include_directories(cft PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/include/)


