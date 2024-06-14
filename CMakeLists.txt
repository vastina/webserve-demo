cmake_minimum_required(VERSION 3.9)
project(demo)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_GENERATOR "Unix Makefiles")

add_compile_options("-Wall" "-Wextra")
set(CMAKE_CXX_STANDARD 17)

set(project_dir "./code")
include_directories("${project_dir}/include")
file(GLOB SOURCES "${project_dir}/*.cpp")

add_executable(demo ${SOURCES}
    # main.cpp
    # serve.cpp
    # epoll/epoll.cpp
    # epoll/clock.cpp
    # http/http.cpp
    # http/httpparse.cpp
    # http/httpresponse.cpp
    # log/loger.cpp
    # pool/Taskpool.cpp
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/f)