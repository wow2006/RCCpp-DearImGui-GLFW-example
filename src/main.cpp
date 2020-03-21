// STL
#include <cstdlib>
// GL3W
#include <GL/gl3w.h>
// imgui headers
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// RCC++ headers
#include "RuntimeObjectSystem.h"
// headers from our example
#include "RCCppMainLoop.h"
#include "StdioLogSystem.h"
#include "SystemTable.h"
// GLFW3
#include <GLFW/glfw3.h>

// RCC++ Data
static StdioLogSystem g_Logger;
static SystemTable g_SystemTable;

bool RCCppInit();
void RCCppCleanup();
void RCCppUpdate();

// Power save
const int POWERSAVEDRAWNUM = 3;
int powerSaveCountDown = POWERSAVEDRAWNUM;

void ResetPowerSaveCountDown() { powerSaveCountDown = 3; }

void WindowResizeCallback(GLFWwindow *pWindow, int width, int height) {
  ResetPowerSaveCountDown();
}

void WindowPosCallback(GLFWwindow *pWindow, int xpos, int ypos) {
  ResetPowerSaveCountDown();
}

void KeyCallback(GLFWwindow *pWindow, int key, int scancode, int action,
                 int mods) {
  ResetPowerSaveCountDown();
  ImGui_ImplGlfw_KeyCallback(pWindow, key, scancode, action, mods);
}

void CharCallback(GLFWwindow *pWindow, unsigned int character) {
  ResetPowerSaveCountDown();
  ImGui_ImplGlfw_CharCallback(pWindow, character);
}

void MouseButtonCallback(GLFWwindow *pWindow, int button, int action, int mods) {
  ResetPowerSaveCountDown();
  ImGui_ImplGlfw_MouseButtonCallback(pWindow, button, action, mods);
}

void MousePosCallback(GLFWwindow *pWindow, double x, double y) {
  ResetPowerSaveCountDown();
}

void MouseWheelCallback(GLFWwindow *pWindow, double x, double y) {
  ResetPowerSaveCountDown();
  ImGui_ImplGlfw_ScrollCallback(pWindow, x, y);
}

int main(int argc, const char *argv[]) {
  if (!glfwInit()) {
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  const auto pMonitor = glfwGetPrimaryMonitor();
  const auto pMode    = glfwGetVideoMode(pMonitor);

  constexpr auto START_MENU_HEIGHT = 30 /*pixels*/;
  constexpr auto GLFW_VSYNC = 1;

  auto pWindow = glfwCreateWindow(pMode->width, pMode->height-START_MENU_HEIGHT, "Company of heros ISA", nullptr, nullptr);
  glfwMakeContextCurrent(pWindow);
  glfwSwapInterval(GLFW_VSYNC); // Enable vsync

  // Power save - ensure callbacks point to the correct place
  glfwSetWindowSizeCallback(pWindow,  WindowResizeCallback);
  glfwSetWindowPosCallback(pWindow,   WindowPosCallback);
  glfwSetKeyCallback(pWindow,         KeyCallback);
  glfwSetCharCallback(pWindow,        CharCallback);
  glfwSetMouseButtonCallback(pWindow, MouseButtonCallback);
  glfwSetCursorPosCallback(pWindow,   MousePosCallback);
  glfwSetScrollCallback(pWindow,      MouseWheelCallback);

  gl3wInit();

  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui_ImplGlfw_InitForOpenGL(pWindow, false);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  // Initialize RCC++
  RCCppInit();

  // Setup style
  ImGui::StyleColorsDark();

  const ImVec4 clear_color = ImColor(
      static_cast<int>(0.1F  * 255.F),
      static_cast<int>(0.1F  * 255.F),
      static_cast<int>(0.15F * 255.F));
  while (!glfwWindowShouldClose(pWindow)) {
    glfwPollEvents();

    // Update RCC++
    RCCppUpdate();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Call the function in our RCC++ class
    g_SystemTable.pRCCppMainLoopI->MainLoop();

    // Rendering
    {
      glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glfwSwapBuffers(pWindow);
    }

    // Power save
    if (g_pSys->power_save) {
      if (powerSaveCountDown) {
        --powerSaveCountDown;
        glfwPollEvents();
      } else {
        ResetPowerSaveCountDown();
        glfwWaitEvents();
      }
    }
  }

  // Cleanup
  RCCppCleanup();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();

  return EXIT_SUCCESS;
}

bool RCCppInit() {
  g_SystemTable.pImContext = ImGui::GetCurrentContext();

  g_SystemTable.pRuntimeObjectSystem = new RuntimeObjectSystem;
  if (!g_SystemTable.pRuntimeObjectSystem->Initialise(&g_Logger,
                                                      &g_SystemTable)) {
    delete g_SystemTable.pRuntimeObjectSystem;
    g_SystemTable.pRuntimeObjectSystem = 0;
    return false;
  }

  g_SystemTable.pRuntimeObjectSystem->CleanObjectFiles();
#ifndef _WIN32
  g_SystemTable.pRuntimeObjectSystem->SetAdditionalCompileOptions("-std=c++11");
#endif

  // ensure include directories are set - use location of this file as starting
  // point
  const auto basePath = g_SystemTable.pRuntimeObjectSystem->FindFile(__FILE__).ParentPath();
  const auto imguiIncludeDir = basePath / ".." / "thirdparty" / "imgui";
  g_SystemTable.pRuntimeObjectSystem->AddIncludeDir(imguiIncludeDir.c_str());
  // current
  const auto includeDir = basePath / ".." / "include";
  g_SystemTable.pRuntimeObjectSystem->AddIncludeDir(includeDir.c_str());

  return true;
}

void RCCppCleanup() { delete g_SystemTable.pRuntimeObjectSystem; }

void RCCppUpdate() {
  // check status of any compile
  if (g_SystemTable.pRuntimeObjectSystem->GetIsCompiledComplete()) {
    // load module when compile complete
    g_SystemTable.pRuntimeObjectSystem->LoadCompiledModule();
  }

  if (!g_SystemTable.pRuntimeObjectSystem->GetIsCompiling()) {
    float deltaTime = 1.0F / ImGui::GetIO().Framerate;
    g_SystemTable.pRuntimeObjectSystem->GetFileChangeNotifier()->Update(
        deltaTime);
  } else {
    ResetPowerSaveCountDown();
  }
}
