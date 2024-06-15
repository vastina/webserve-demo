cmake_minimum_required(VERSION 3.9)
project(demo)

add_compile_options("-Wall" "-Wextra")
set(CMAKE_CXX_STANDARD 20)

set(project_dir "${CMAKE_SOURCE_DIR}/code")
include_directories("${project_dir}/include")
file(GLOB SOURCES "${project_dir}/*.cpp")

add_executable(demo ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/f)