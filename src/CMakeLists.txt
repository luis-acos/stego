add_executable(vsteg main.c)

find_package(Threads REQUIRED)
include_directories(${THREADS_INCLUDE_DIRS})

cmake_policy (SET CMP0023 OLD)
target_link_libraries(vsteg Threads::Threads)
target_link_libraries(vsteg Stego)
target_link_libraries(vsteg Bitmap)
target_link_libraries(vsteg Util)
