#ifndef MY_SHADER_MANAGER_H
#define MY_SHADER_MANAGER_H

#include "kalles_gl_lib/ShaderManager.h"

class MyShaderManager : public ShaderManager
{
public:
  static MyShaderManager* Instance();
  static void CreateInstance();
  const int N_LIGHTSOURCES; 
private:
  MyShaderManager();

  void AddAllShaders();
  void AddAllShaderPrograms();

  // ShaderPrograms
  void AddSimpleShaderProgram();
  void AddNormalColorShaderProgram();
  void AddRenderTextureShaderProgram();
  void AddTextureCombinerShaderProgram();
  void AddPhongShaderProgram();
  void AddBrightLightShaderProgram();
  void AddGaussianBlurShaderProgram();

};

#endif // MY_SHADER_MANAGER_H