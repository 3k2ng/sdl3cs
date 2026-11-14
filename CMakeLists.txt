cmake_minimum_required(VERSION 3.25)
project(sdl3cs LANGUAGES CXX)

find_package(SDL3 REQUIRED)

add_executable(e00_minimal "./examples/e00_minimal.cpp")
target_compile_features(e00_minimal PUBLIC cxx_std_20)
set_target_properties(e00_minimal PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(e00_minimal PRIVATE SDL3::SDL3)
