cmake_minimum_required(VERSION 3.9)
project(demo)

add_compile_options(-pthread -Wall -g)
set(CMAKE_CXX_STANDARD 17)

add_executable(demo 
    main.cpp
    serve.cpp
    epoll/epoll.cpp
    epoll/clock.cpp
    http/http.cpp
    http/httpparse.cpp
    http/httpresponse.cpp
    log/loger.cpp
)

set(EXECUTABLE_OUTPUT_PATH /home/net/webserve-demo/f)