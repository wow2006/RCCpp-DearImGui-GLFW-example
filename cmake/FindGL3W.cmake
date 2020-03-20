# ${CMAKE_SOURCE_DIR}/cmake/FindGL3W.cmake
project(gl3w C)

set(GL3W_DIR ${CMAKE_SOURCE_DIR}/thirdparty/gl3w/)

add_library(
  gl3w
  STATIC
  ${GL3W_DIR}/src/gl3w.c
)

add_library(
  gl3w::gl3w
  ALIAS
  gl3w
)

target_include_directories(
  gl3w
  SYSTEM PUBLIC
  ${GL3W_DIR}/include/
)

