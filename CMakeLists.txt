cmake_minimum_required(VERSION 3.28)
project(plifaces LANGUAGES C CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

file(GLOB include ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
set(PLIFACES_NAMESPACE "plifaces" CACHE STRING "Library primary namespace name")

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/private/Config.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/generated/config/Config.hpp
        @ONLY
)

add_library(plifaces
    ${src}
    ${include}
)

target_include_directories(plifaces
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/public
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/private
        ${CMAKE_CURRENT_BINARY_DIR}/generated/config
)

target_compile_definitions(plifaces PUBLIC
    PLIFACES_NAMESPACE=${PLIFACES_NAMESPACE}
    "PLIFACES_NAMESPACE_BEGIN=namespace ${PLIFACES_NAMESPACE} {"
    "PLIFACES_NAMESPACE_END=}"
)

add_executable(plifaces_test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(plifaces_test PRIVATE plifaces)