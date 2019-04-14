add_executable(vsteg main.c)

find_package(Threads REQUIRED)

target_link_libraries(vsteg LINK_PUBLIC Stego)
target_link_libraries(vsteg LINK_PUBLIC Bitmap)
target_link_libraries(vsteg LINK_PUBLIC Util)
target_link_libraries(vsteg Threads::Threads)
