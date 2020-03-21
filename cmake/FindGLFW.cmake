# ${CMAKE_SOURCE_DIR}/cmake/FindGLFW.cmak

add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/glfw)

add_library(
  glfw::glfw
  ALIAS
  glfw
)

