add_executable(vsteg main.c)

find_package(Threads REQUIRED)
include_directories(${THREADS_INCLUDE_DIRS})
target_link_libraries(vsteg LINK_PUBLIC Threads)

target_link_libraries(vsteg LINK_PUBLIC Stego)
target_link_libraries(vsteg LINK_PUBLIC Bitmap)
target_link_libraries(vsteg LINK_PUBLIC Util)
