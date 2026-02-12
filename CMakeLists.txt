cmake_minimum_required(VERSION 4.0)
project(plifaces LANGUAGES C CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

file(GLOB include ${CMAKE_SOURCE_DIR}/include/*.hpp)
file(GLOB src ${CMAKE_SOURCE_DIR}/src/*.cpp)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(plifaces
    ${src}
)

target_include_directories(plifaces
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/public
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/private
)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE plifaces)