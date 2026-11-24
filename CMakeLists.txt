cmake_minimum_required(VERSION 3.26)
project(sdl3cs LANGUAGES CXX)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_custom_target(copy_assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different # or copy_directory
  ${CMAKE_CURRENT_LIST_DIR}/res
  ${CMAKE_CURRENT_BINARY_DIR}/res # or $<TARGET_FILE_DIR:app>/res
)

foreach(ei
  e00_minimal
)
  add_executable(${ei} "./examples/${ei}.cpp")
  target_compile_features(${ei} PRIVATE cxx_std_20)
  set_target_properties(${ei} PROPERTIES CXX_EXTENSIONS OFF)
  target_link_libraries(${ei} PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
  )
  target_include_directories(${ei} PRIVATE stb)
  add_dependencies(${ei} copy_assets)
  if(WIN32)
    add_custom_command(TARGET ${ei} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different -t
      $<TARGET_FILE_DIR:${ei}> $<TARGET_RUNTIME_DLLS:${ei}>
      COMMAND_EXPAND_LISTS
    )
  endif()
endforeach()
