#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../include/Scene.h"

#include "../include/MyGlWindow.h"

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
}

MyGlWindow::~MyGlWindow()
{
  glfwTerminate();
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
  glfwSetKeyCallback(window_, KeyCallback);
  glfwSetErrorCallback(ErrorCallback);
  glfwSetScrollCallback(window_, ScrollFun);
  glfwSetMouseButtonCallback(window_, MouseButtonFun);
  glfwSetWindowSizeCallback(window_, WindowSizeFun);
  return 0;
}

int MyGlWindow::InitOpenGL()
{
  glClearColor(0.5f,0.5f,0.5f,0.5f);
  glEnable(GL_DEPTH_TEST);
  // Initialize GLEW
  glewExperimental = true; // Needed in core profile
  if (glewInit() != GLEW_OK) {
      return -1;
  }
  return 0;
}

void MyGlWindow::MainLoop()
{
  while (!glfwWindowShouldClose(window_))
  {
    UpdateMousePos();
    scene_->Update();

    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    scene_->Render(width, height);
    
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
}

void MyGlWindow::ScrollFun(GLFWwindow* window, double x_pos, double y_pos)
{
  scene_->GetCamera()->IncrementZposition(y_pos);
}

void MyGlWindow::MouseButtonFun(
        GLFWwindow* window,
        int button,
        int action,
        int mods)
{
  if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    mouse_pressed_ = true;
  else if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
    mouse_pressed_ = false;
}

void MyGlWindow::WindowSizeFun(GLFWwindow* window, int width, int height)
{
  scene_->GetCamera()->SetAspectRatio(width / static_cast<float>(height));
}

