#ifndef MY_GL_WINDOW_H
#define MY_GL_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../include/Scene.h"

class MyGlWindow
{
public:
	MyGlWindow(Scene* scene);
	~MyGlWindow();

  void MainLoop();
private:
  int InitGLFW();
  int InitOpenGL();
	GLFWwindow* window;
  Scene* scene_;
};

#endif // MY_GL_WINDOW_H