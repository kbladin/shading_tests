#ifndef MY_SHADER_MANAGER_H
#define MY_SHADER_MANAGER_H

#include "kalles_gl_lib/ShaderManager.h"

class MyShaderManager : public ShaderManager
{
public:
  static MyShaderManager* Instance();
  static void CreateInstance();
private:
  MyShaderManager();

  void AddAllShaders();
  void AddAllShaderPrograms();

  // ShaderPrograms
  void AddSimpleShaderProgram();
  void AddNormalColorShaderProgram();
  void AddEdgeDetectorShaderProgram();
  void AddTextureCombinerShaderProgram();
  void AddPhongShaderProgram();
  void AddBrightLightShaderProgram();
  void AddBlurShaderProgram();
  void AddToonShaderProgram();
  void AddOneColorShaderProgram();

};

#endif // MY_SHADER_MANAGER_H