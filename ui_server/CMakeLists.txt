cmake_minimum_required(VERSION 3.10)
project(ui_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost libraries
find_package(Boost 1.70 REQUIRED COMPONENTS system)

# Create the executable
add_executable(ui_server 
    src/main.cpp
    src/session.cpp
    src/server.cpp
)

# Link Boost libraries
target_link_libraries(ui_server PRIVATE Boost::system)

# Include directories
target_include_directories(ui_server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)
