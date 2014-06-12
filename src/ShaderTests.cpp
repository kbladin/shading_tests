#include "../include/MyShaderManager.h"
#include "../include/MyGlWindow.h"

#include <GLFW/glfw3.h>

#include "../include/ShaderTests.h"

ShaderTests::ShaderTests()
{
  scene_ = new Scene(Camera());
  my_gl_window_ = new MyGlWindow(scene_);
  MyShaderManager::CreateInstance();
  my_gl_window_->MainLoop();
}

ShaderTests::~ShaderTests()
{
  delete my_gl_window_;
  delete scene_;
}