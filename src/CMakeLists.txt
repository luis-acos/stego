add_executable(vsteg main.c Stego.c Bitmap.c)

target_link_libraries(vsteg LINK_PUBLIC Stego)
target_link_libraries(vsteg LINK_PUBLIC Util)
target_link_libraries(vsteg LINK_PUBLIC Bitmap)
