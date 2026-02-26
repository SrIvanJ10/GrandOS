cmake_minimum_required(VERSION 3.20)
project(ws_server CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LWS REQUIRED libwebsockets)

add_executable(ws_server ws_server.cpp)

target_include_directories(ws_server PRIVATE ${LWS_INCLUDE_DIRS})
target_link_libraries(ws_server PRIVATE ${LWS_LIBRARIES})
target_compile_options(ws_server PRIVATE ${LWS_CFLAGS_OTHER})
