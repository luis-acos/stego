add_library(Stego src/stego.c)
target_include_directories(Stego PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Stego LINK_PUBLIC Util)
