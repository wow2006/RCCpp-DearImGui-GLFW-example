# ${CMAKE_SOURCE_DIR}/cmake/FindImGui.cmak
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/thirdparty/imgui)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_INSTALL  OFF CACHE BOOL "GLFW lib only")
set(BUILD_EXAMPLES      OFF CACHE BOOL "RCCpp libsonly")

find_package(GL3W MODULE REQUIRED)

add_library(
  imgui
  STATIC
)

add_library(
  imgui::glfw
  ALIAS
  imgui
)

target_sources(
  imgui
  PRIVATE
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/examples/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/examples/imgui_impl_opengl3.cpp
)

target_include_directories(
  imgui
  SYSTEM PUBLIC
  ${IMGUI_DIR}/
  ${IMGUI_DIR}/examples/
)

target_link_libraries(
  imgui
  PUBLIC
  gl3w::gl3w
  OpenGL::GL
)

target_compile_definitions(
  imgui
  PUBLIC
  IMGUI_IMPL_OPENGL_LOADER_GL3W
)
