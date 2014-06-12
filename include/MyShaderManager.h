#ifndef MY_SHADER_MANAGER_H
#define MY_SHADER_MANAGER_H

#include "kalles_gl_lib/ShaderManager.h"

class MyShaderManager : public ShaderManager
{
public:
  static MyShaderManager* Instance();
  static void CreateInstance();
private:
  MyShaderManager() : ShaderManager()
  {
  	AddAllShaders();
  	AddAllShaderPrograms();
  };

  void AddAllShaders();
  void AddAllShaderPrograms();

  // ShaderPrograms
  void AddSimpleShaderProgram();
  void AddBasicShaderProgram();
};

#endif // MY_SHADER_MANAGER_H