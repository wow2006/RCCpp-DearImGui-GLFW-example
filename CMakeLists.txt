# ${CMAKE_SOURCE_DIR}/CMakeLists.txt
cmake_minimum_required(VERSION 3.9)
project(demo CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL MODULE REQUIRED)
find_package(GLFW   MODULE REQUIRED)
find_package(ImGui  MODULE REQUIRED)
add_subdirectory(thirdparty/RuntimeCompiledCPlusPlus/Aurora)

add_executable(
  demo
)

target_sources(
  demo
  PRIVATE
  src/main.cpp
  #
  include/StdioLogSystem.h
  src/StdioLogSystem.cpp
  include/RCCppMainLoop.h
  src/RCCppMainLoop.cpp
  include/SystemTable.h
)

target_link_libraries(
  demo
  PRIVATE
  glfw::glfw
  imgui::glfw
  RuntimeCompiler
  RuntimeObjectSystem
)

target_include_directories(
  demo
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include/
  ${CMAKE_SOURCE_DIR}/thirdparty/RuntimeCompiledCPlusPlus/Aurora/RuntimeObjectSystem
  ${CMAKE_SOURCE_DIR}/thirdparty/RuntimeCompiledCPlusPlus/Aurora/RuntimeCompiler
)

set_target_properties(
  demo
  PROPERTIES
  CXX_EXTENSIONS        OFF
  CXX_STANDARD          17
  CXX_STANDARD_REQUIRED ON
)

target_link_options(
  demo
  PUBLIC
  $<$<PLATFORM_ID:Windows>:"/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup">
)

target_compile_options(
  demo
  PUBLIC
  $<$<COMPILE_LANG_AND_ID:CXX,AppleClang>:-stdlib=libc++>
)

if( MSVC )
  if( ${CMAKE_VERSION} VERSION_LESS "3.6.0" )
    message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'RCCpp-DearImGui-GLFW-example' as StartUp Project in Visual Studio.\n" )
  else()
    set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RCCpp-DearImGui-GLFW-example )
  endif()
endif()
