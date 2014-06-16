#include "../include/MyShaderManager.h"
#include "../include/MyGlWindow.h"

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "../include/ShaderTests.h"

ShaderTests::ShaderTests()
{
  my_gl_window_ = new MyGlWindow();
  MyShaderManager::CreateInstance();
  my_gl_window_->MainLoop();
}

ShaderTests::~ShaderTests()
{
	delete my_gl_window_;
}