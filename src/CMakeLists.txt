add_executable(vsteg main.c)

find_package(Threads REQUIRED)

if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(vsteg "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(vsteg "${CMAKE_THREAD_LIBS_INIT}")
endif()

target_link_libraries(vsteg Stego)
target_link_libraries(vsteg Bitmap)
target_link_libraries(vsteg Util)
