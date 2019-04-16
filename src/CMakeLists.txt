add_executable(vsteg main.c)

find_package(Threads REQUIRED)

target_link_libraries(vsteg "${CMAKE_THREAD_LIBS_INIT}")

target_link_libraries(vsteg Stego)
target_link_libraries(vsteg Bitmap)
target_link_libraries(vsteg Util)
