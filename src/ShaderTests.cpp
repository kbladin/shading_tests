#include "../include/MyShaderManager.h"
#include "../include/MyGlWindow.h"

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "../include/ShaderTests.h"

ShaderTests::ShaderTests()
{
  MyShaderManager::CreateInstance();
  
  //my_gl_window_ = new MyGlWindow;

  my_gl_window_.MainLoop();
}

ShaderTests::~ShaderTests()
{

}