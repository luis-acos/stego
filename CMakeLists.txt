cmake_minimum_required(VERSION 3.5.1)
project(vsteg C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_subdirectory(build)

add_subdirectory(util/lib)
add_subdirectory(util/src)

add_subdirectory(bitmap/lib)
add_subdirectory(bitmap/src)

add_subdirectory(stego/lib)
add_subdirectory(stego/src)
