#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../include/Scene.h"

#include "../include/MyGlWindow.h"

MyGlWindow::MyGlWindow(Scene* scene)
{
  scene_ = scene;
  InitGLFW();
  InitOpenGL();
}

MyGlWindow::~MyGlWindow()
{

}

int MyGlWindow::InitGLFW()
{
	/* Initialize the library */
  if (!glfwInit())
      return -1;

  // Modern OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
      glfwTerminate();
      return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  return 0;
}

int MyGlWindow::InitOpenGL()
{ 
  // Initialize GLEW
  glewExperimental=true; // Needed in core profile
  if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize GLEW\n");
      return false;
  }
  return 0;
}

void MyGlWindow::MainLoop()
{
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }
  glfwTerminate();
}