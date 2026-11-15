cmake_minimum_required(VERSION 3.26)
project(sdl3cs LANGUAGES CXX)

find_package(SDL3 REQUIRED)

add_custom_target(copy_assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different # or copy_directory
  ${CMAKE_CURRENT_LIST_DIR}/res
  ${CMAKE_CURRENT_BINARY_DIR}/res # or $<TARGET_FILE_DIR:app>/res
)

add_executable(e00_minimal "./examples/e00_minimal.cpp")
target_compile_features(e00_minimal PUBLIC cxx_std_20)
set_target_properties(e00_minimal PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(e00_minimal PRIVATE SDL3::SDL3)

add_executable(e01_handle_event "./examples/e01_handle_event.cpp")
target_compile_features(e01_handle_event PUBLIC cxx_std_20)
set_target_properties(e01_handle_event PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(e01_handle_event PRIVATE SDL3::SDL3)

add_executable(e02_texture_stream "./examples/e02_texture_stream.cpp")
target_compile_features(e02_texture_stream PUBLIC cxx_std_20)
set_target_properties(e02_texture_stream PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(e02_texture_stream PRIVATE SDL3::SDL3)

add_executable(e03_load_font "./examples/e03_load_font.cpp")
target_compile_features(e03_load_font PUBLIC cxx_std_20)
set_target_properties(e03_load_font PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(e03_load_font PRIVATE SDL3::SDL3)
target_include_directories(e03_load_font PRIVATE stb)
add_dependencies(e03_load_font copy_assets)
