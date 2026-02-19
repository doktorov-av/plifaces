cmake_minimum_required(VERSION 4.0)
project(plifaces LANGUAGES C CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB include ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(plifaces
    ${src}
    ${include}
)

target_include_directories(plifaces
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/private
)

add_executable(plifaces_test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(plifaces_test PRIVATE plifaces)