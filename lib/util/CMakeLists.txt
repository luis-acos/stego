add_library(Util src/util.c)
target_include_directories(Util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Util LINK_PUBLIC Bitmap)
