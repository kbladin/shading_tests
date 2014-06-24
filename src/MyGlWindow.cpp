#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>

#include "../include/Scene.h"
#include "../include/MyGlWindow.h"
#include "../include/SettingsManager.h"

Scene* MyGlWindow::scene_;
bool MyGlWindow::mouse_pressed_;


MyGlWindow::MyGlWindow()
{
  if (InitGLFW() != 0)
    std::cout << "ERROR: Failed to initialize GLFW!" << std::endl;
  if (InitOpenGL() != 0)
    std::cout << "ERROR: Failed to initialize GLEW!" << std::endl;
  int width, height;
  glfwGetWindowSize(window_, &width, &height);
  float aspect_ratio = static_cast<float>(width) / height;
  scene_ = new Scene::Scene(new Camera(glm::vec3(0.0f, 0.0f, 0.0f),
          200.0f,
          0.1f,
          aspect_ratio));
  InitTW();
}

MyGlWindow::~MyGlWindow()
{
  glfwTerminate();
  TwTerminate();
  delete scene_;
}

int MyGlWindow::InitGLFW()
{
	// Initialize glfw
  if (!glfwInit())
      return -1;
  // Modern OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Create a windowed mode window and its OpenGL context
  window_ = glfwCreateWindow(640, 480, "Shader Tests", NULL, NULL);
  if (!window_)
  {
      glfwTerminate();
      return -1;
  }
  // Make the window_'s context current
  glfwMakeContextCurrent(window_);
  // Set callback functions
  //glfwSetErrorCallback(window_, (GLFWerrorcallbackfun)ErrorCallback);
  glfwSetMouseButtonCallback(window_, (GLFWmousebuttonfun)MouseButtonCallback);
  glfwSetCursorPosCallback(window_, (GLFWcursorposfun)MousePosCallback);
  glfwSetScrollCallback(window_, (GLFWscrollfun)ScrollCallback);
  glfwSetWindowSizeCallback(window_, WindowSizeCallback);
  glfwSetKeyCallback(window_, (GLFWkeyfun)KeyCallback);
  glfwSetCharCallback(window_, (GLFWcharfun)CharCallback);

  return 0;
}

int MyGlWindow::InitOpenGL()
{
  glClearColor(0.1f,0.1f,0.2f,0.0f);
  glEnable(GL_DEPTH_TEST);
  // Initialize GLEW
  glewExperimental = true; // Needed in core profile
  if (glewInit() != GLEW_OK) {
      return -1;
  }
  return 0;
}

int MyGlWindow::InitTW()
{
  int width, height;
  glfwGetWindowSize(window_, &width, &height);
    // Initialize anttweakbar
  TwInit(TW_OPENGL_CORE, NULL);
  TwWindowSize(width*2, height*2);
  TwBar *global_bar;
  global_bar = TwNewBar("Global Variables");
  TwAddVarRW(
          global_bar,
          "Number of blur loops",
          TW_TYPE_UINT8,
          &SettingsManager::Instance()->n_blur_loops,
          "");
  TwAddVarRW(
          global_bar,
          "Blur filter size",
          TW_TYPE_UINT8,
          &SettingsManager::Instance()->filter_size,
          "");
  TwAddVarRW(
          global_bar,
          "Multiplier 1",
          TW_TYPE_FLOAT,
          &SettingsManager::Instance()->multiplier1,
          "min=-1.0 max=2.0 step=0.1");
  TwAddVarRW(
          global_bar,
          "Multiplier 2",
          TW_TYPE_FLOAT,
          &SettingsManager::Instance()->multiplier2,
          "min=-1.0 max=2.0 step=0.1");

  for (int i = 0; i < SettingsManager::Instance()->N_LIGHTSOURCES; ++i)
  {
    TwBar *light_bar;
    std::stringstream bar_name;
    bar_name << "Light " << i << " properties";
    light_bar = TwNewBar(bar_name.str().c_str());

    TwAddVarRW(
            light_bar,
            "Intensity", 
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].intensity,
            "min=0.0 max=100.0 step=1.0");
    TwAddVarRW(
            light_bar,
            "Color", 
            TW_TYPE_COLOR3F,
            &scene_->light_sources_[i].color.x,
            "");
    TwAddVarRW(
            light_bar,
            "Position", 
            TW_TYPE_DIR3F,
            &scene_->light_sources_[i].position.x,
            "");
/*    TwAddVarRW(
            light_bar,
            "Directional", 
            TW_TYPE_BOOL32,
            &scene_->light_sources_[i].position.w,
            "");*/
    TwAddVarRW(
            light_bar,
            "Constant attenuation",
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].constant_attenuation,
            "min=0.0 max=1.0 step=0.1");
    TwAddVarRW(
            light_bar,
            "Linear attenuation",
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].linear_attenuation,
            "min=0.0 max=1.0 step=0.1");
    TwAddVarRW(
            light_bar,
            "Quadratic attenuation",
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].quadratic_attenuation,
            "min=0.0 max=1.0 step=0.1");
    TwAddVarRW(
            light_bar,
            "Spot cutoff",
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].spot_cutoff,
            "min=0.0 max=100.0 step=1.0");
    TwAddVarRW(
            light_bar,
            "Spot exponent",
            TW_TYPE_FLOAT,
            &scene_->light_sources_[i].spot_exponent,
            "min=0.0 max=100.0 step=1.0");
    TwAddVarRW(
            light_bar,
            "Spot direction",
            TW_TYPE_DIR3F,
            &scene_->light_sources_[i].spot_direction.x,
            "");

  }

  TwDefine(" GLOBAL fontsize=3 ");
  TwDefine(" GLOBAL fontresizable=false "); // font cannot be resized

  return 0;
}

void MyGlWindow::MainLoop()
{
  float current_time = glfwGetTime();
  int FPS = 0;
  while (!glfwWindowShouldClose(window_))
  {
    UpdateMousePos();
    scene_->Update();

    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    scene_->Render(width, height);
    TwWindowSize(width*2, height*2);

    TwDraw();  // draw the tweak bar(s)
    // Print FPS
    ++FPS;
    if((glfwGetTime() - current_time) > 1){
      std::string title = "Shader Tests, ";
      std::ostringstream ss;
      ss << FPS;
      std::string s(ss.str());
      title.append(s);
      title.append(" FPS");
      glfwSetWindowTitle (window_,  title.c_str());
      FPS = 0;
      current_time = glfwGetTime();
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();


  }
}

void MyGlWindow::UpdateMousePos()
{
  double cursor_pos_x, cursor_pos_y;
  glfwGetCursorPos(window_, &cursor_pos_x, &cursor_pos_y);
  if (mouse_pressed_)
  {
    double diff_x = prev_cursor_pos_x_ - cursor_pos_x;
    double diff_y = prev_cursor_pos_y_ - cursor_pos_y;
    scene_->GetCamera()->IncrementXrotation(-diff_y*0.01);
    scene_->GetCamera()->IncrementYrotation(-diff_x*0.01);
  }
  prev_cursor_pos_x_ = cursor_pos_x;
  prev_cursor_pos_y_ = cursor_pos_y;
}

void MyGlWindow::ErrorCallback(int error, const char* description)
{
  fputs(description, stderr);
}

void MyGlWindow::KeyCallback(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods)
{
  switch (key)
  {
    case GLFW_KEY_ESCAPE :
    {
      glfwSetWindowShouldClose(window, true);
    }
  }
  TwEventKeyGLFW(key, action);
}

void MyGlWindow::ScrollCallback(GLFWwindow* window, double x_pos, double y_pos)
{
  scene_->GetCamera()->IncrementZposition(y_pos);
  TwEventMouseWheelGLFW(y_pos);
}

void MyGlWindow::MouseButtonCallback(
        GLFWwindow* window,
        int button,
        int action,
        int mods)
{
  if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
    mouse_pressed_ = true;
  else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE)
    mouse_pressed_ = false;
  TwEventMouseButtonGLFW(button, action);
}

void MyGlWindow::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
  scene_->GetCamera()->SetAspectRatio(width / static_cast<float>(height));
}

void MyGlWindow::MousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
  TwMouseMotion(int(xpos*2), int(ypos*2));
}

void MyGlWindow::CharCallback(GLFWwindow* window, int codepoint)
{
  TwEventCharGLFW(codepoint, GLFW_PRESS);
}

