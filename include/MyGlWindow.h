#ifndef MY_GL_WINDOW_H
#define MY_GL_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../include/Scene.h"

class MyGlWindow
{
public:
	MyGlWindow();
	~MyGlWindow();
  void MainLoop();
private:
  int InitGLFW();
  int InitOpenGL();
  void UpdateMousePos();
  static void ErrorCallback(int error, const char* description);
  static void KeyCallback(
          GLFWwindow* window,
          int key,
          int scancode,
          int action,
          int mods);
  static void ScrollFun(GLFWwindow* window, double x_pos, double y_pos);
  static void MouseButtonFun(
          GLFWwindow* window,
          int button,
          int action,
          int mods);
  static void WindowSizeFun(GLFWwindow* window, int width, int height);
	
  GLFWwindow* window_;
  static Scene* scene_;
  double prev_cursor_pos_x_;
  double prev_cursor_pos_y_;
  static bool mouse_pressed_;
};

#endif // MY_GL_WINDOW_H