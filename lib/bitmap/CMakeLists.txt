add_library(Bitmap src/bitmap.c)
target_include_directories(Bitmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory (lib/util)
target_link_libraries(Bitmap LINK_PUBLIC Util)
