add_executable(vsteg main.c)

find_package(Threads REQUIRED)
include_directories(${THREADS_INCLUDE_DIRS})
target_link_libraries(vsteg Threads::Threads)

target_link_libraries(vsteg PUBLIC Stego)
target_link_libraries(vsteg PUBLIC Bitmap)
target_link_libraries(vsteg PUBLIC Util)
