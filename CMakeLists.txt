cmake_minimum_required(VERSION 3.9)
project(demo)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_GENERATOR "Unix Makefiles")

add_compile_options(-pthread -Wall -g)
set(CMAKE_CXX_STANDARD 17)

include_directories("./code/include")
file(GLOB SOURCES "code/src/*.cpp")
file(GLOB SOURCES_BASE "code/src/base/*.cpp")
add_executable(demo ${SOURCES} ${SOURCES_BASE})
# looks like clangd don't understand and can't find the header files in the include directory 
# so again I use intelli sence 

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/f)