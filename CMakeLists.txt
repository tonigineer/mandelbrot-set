cmake_minimum_required(VERSION 3.25)

project(
    Mandelbrot
    VERSION 0.0.1
    LANGUAGES CXX)

message(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(SFML 2 REQUIRED COMPONENTS graphics system window)
set(SFML_LIBS sfml-graphics sfml-system sfml-window)

include_directories("include")
file(GLOB SOURCES "src/*.cpp")

add_executable(
    ${PROJECT_NAME}
    ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME}
    ${SFML_LIBS})

set_property(
    TARGET
    ${PROJECT_NAME}
    PROPERTY CXX_STANDARD 20)
