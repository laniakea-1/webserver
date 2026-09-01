cmake_minimum_required (VERSION 3.22)

project (webserver)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include sub-projects.
add_subdirectory(src/Server)

add_executable(webserver src/main.cpp)
target_link_libraries(webserver PRIVATE Server)