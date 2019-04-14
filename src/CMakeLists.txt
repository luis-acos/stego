add_executable(vsteg main.c)

find_package(Threads REQUIRED)
include_directories(${THREADS_INCLUDE_DIRS})
target_link_libraries(vsteg Threads)

target_link_libraries(vsteg Stego)
target_link_libraries(vsteg Bitmap)
target_link_libraries(vsteg Util)
