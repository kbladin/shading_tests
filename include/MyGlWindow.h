#ifndef MY_GL_WINDOW_H
#define MY_GL_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>

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
  int InitTW();
  void RenderScene(void (Scene::*function)(int, int), Scene* s);
  void UpdateMousePos();
  static void ErrorCallback(int error, const char* description);
  static void KeyCallback(
          GLFWwindow* window,
          int key,
          int scancode,
          int action,
          int mods);
  static void ScrollCallback(GLFWwindow* window, double x_pos, double y_pos);
  static void MouseButtonCallback(
          GLFWwindow* window,
          int button,
          int action,
          int mods);
  static void WindowSizeCallback(GLFWwindow* window, int width, int height);
  static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);
  static void CharCallback(GLFWwindow* window, int codepoint);
  static void TW_CALL LoadButtonCallback(void* client_data);
	
  void (Scene::* RenderFunction)(int, int);

  GLFWwindow* window_;
  static Scene* scene_;
  double prev_cursor_pos_x_;
  double prev_cursor_pos_y_;
  static bool mouse_pressed_;
};

#endif // MY_GL_WINDOW_H