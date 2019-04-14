cmake_minimum_required(VERSION 3.10.2)
project(vsteg C)

set(CMAKE_C_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
set (CMAKE_C_FLAGS "-g")
set (CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(lib/util)
add_subdirectory(lib/stego)
add_subdirectory(lib/bitmap)
