cmake_minimum_required(VERSION 3.20)
project(OpenSimNextViewer VERSION 0.1.0 LANGUAGES CXX)
option(OPENSIM_NEXT_BUILD_TESTS "Build unit tests" ON)

add_library(opensim_next_core
  src/app/application.cpp
  src/world/world_state.cpp
  src/renderer/mock_renderer.cpp
  src/renderer/world_render_bridge.cpp
  src/renderer/camera_controller.cpp
)
target_include_directories(opensim_next_core PUBLIC include)
target_compile_features(opensim_next_core PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(opensim_next_core PRIVATE /W4 /permissive-)
else()
  target_compile_options(opensim_next_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(opensim-next-viewer src/main.cpp)
target_link_libraries(opensim-next-viewer PRIVATE opensim_next_core)

if(OPENSIM_NEXT_BUILD_TESTS)
  enable_testing()
  add_executable(opensim-next-core-tests tests/core_tests.cpp)
  target_link_libraries(opensim-next-core-tests PRIVATE opensim_next_core)
  # These tests use assert expressions for both checks and setup. Keep them active in Release builds.
  if(MSVC)
    target_compile_options(opensim-next-core-tests PRIVATE /UNDEBUG)
  else()
    target_compile_options(opensim-next-core-tests PRIVATE -UNDEBUG)
  endif()
  add_test(NAME opensim-next-core-tests COMMAND opensim-next-core-tests)
endif()
