# ${CMAKE_SOURCE_DIR}/CMakeLists.txt
cmake_minimum_required(VERSION 3.9)
project(CompanyOfHeros CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL MODULE REQUIRED)
find_package(GLFW   MODULE REQUIRED)
find_package(ImGui  MODULE REQUIRED)

add_subdirectory(RuntimeCompiledCPlusPlus/Aurora)

if( WIN32 )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup" )
endif()

add_executable(
  CompanyOfHeros
)

target_sources(
  CompanyOfHeros
  PRIVATE
  main.cpp
  #
  StdioLogSystem.h
  StdioLogSystem.cpp
  RCCppMainLoop.h
  RCCppMainLoop.cpp
  SystemTable.h
)

target_link_libraries(
  CompanyOfHeros
  PRIVATE
  glfw::glfw
  imgui::glfw
  RuntimeCompiler
  RuntimeObjectSystem
)

target_include_directories(
  CompanyOfHeros
  PRIVATE
  ${CMAKE_SOURCE_DIR}/RuntimeCompiledCPlusPlus/Aurora/RuntimeObjectSystem
  ${CMAKE_SOURCE_DIR}/RuntimeCompiledCPlusPlus/Aurora/RuntimeCompiler
)

set_target_properties(
  CompanyOfHeros
  PROPERTIES
  CXX_EXTENSIONS        OFF
  CXX_STANDARD          17
  CXX_STANDARD_REQUIRED ON
)

target_link_options(
  CompanyOfHeros
  PUBLIC
  $<$<PLATFORM_ID:Windows>:"/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup">
)

target_compile_options(
  CompanyOfHeros
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
