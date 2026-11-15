cmake_minimum_required(VERSION 3.26)
project(sdl3cs LANGUAGES CXX)

find_package(SDL3 REQUIRED)

add_custom_target(copy_assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different # or copy_directory
  ${CMAKE_CURRENT_LIST_DIR}/res
  ${CMAKE_CURRENT_BINARY_DIR}/res # or $<TARGET_FILE_DIR:app>/res
)

foreach(ei
  e00_minimal
  e01_handle_event
  e02_texture_stream
  e03_load_font
)
  add_executable(${ei} "./examples/${ei}.cpp")
  target_compile_features(${ei} PRIVATE cxx_std_20)
  set_target_properties(${ei} PROPERTIES CXX_EXTENSIONS OFF)
  target_link_libraries(${ei} PRIVATE SDL3::SDL3)
endforeach()

target_include_directories(e03_load_font PRIVATE stb)
add_dependencies(e03_load_font copy_assets)
