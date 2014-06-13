#include "../include/MyShaderManager.h"

MyShaderManager* MyShaderManager::Instance()
{
  if (!instance_)
  {
    std::cout << "Error: Calling Instance() before instance created!"
            << std::endl;
  }
  return reinterpret_cast<MyShaderManager*>(instance_);
}

void MyShaderManager::CreateInstance()
{
  if (!instance_)
  {
    instance_ = new MyShaderManager();
  }
}

void MyShaderManager::AddAllShaders()
{

  std::stringstream preprocessor;
  preprocessor << "#version 330 core \n";

  std::stringstream preprocessor_basic_frag;
  // Create shaders
  Shader* simple_mvp_vert = new Shader(
      "../data/shaders/simple.vert",
      preprocessor.str().c_str(),
      GL_VERTEX_SHADER);

  Shader* simple_mvp_frag = new Shader(
      "../data/shaders/simple.frag",
      preprocessor.str().c_str(),
      GL_FRAGMENT_SHADER);

  // Put shaders in the map
  shaders_.insert(StringShaderPair("Simple_Vert", simple_mvp_vert));
  shaders_.insert(StringShaderPair("Simple_Frag", simple_mvp_frag));
}

void MyShaderManager::AddAllShaderPrograms()
{
  AddSimpleShaderProgram();
}

void MyShaderManager::AddSimpleShaderProgram(){
    // Create ShaderProgram
  ShaderProgram* shader_program =
  new ShaderProgram(shaders_["Simple_Vert"],
                    shaders_["Simple_Frag"]);
  // The name with which to refer to the ShaderProgram
  const char* shader_program_name = "Simple";
  // Put ShaderProgram in the map
  shader_programs_.insert(StringShaderProgPair(
      shader_program_name,
      shader_program) );
}

void MyShaderManager::AddBasicShaderProgram()
{
    // Create ShaderProgram
  ShaderProgram* shader_program =
  new ShaderProgram(shaders_["Basic_Vert"],
                    shaders_["Basic_Frag"]);
  // The name with which to refer to the ShaderProgram
  const char* shader_program_name = "Basic";
  // Put ShaderProgram in the map
  shader_programs_.insert(StringShaderProgPair(
      shader_program_name,
      shader_program) );
  // Create all locations
  shader_programs_[shader_program_name]->CreateUniformLocation("MVP");
  shader_programs_[shader_program_name]->CreateUniformLocation("MV");
  shader_programs_[shader_program_name]->CreateUniformLocation("M");
  shader_programs_[shader_program_name]->CreateUniformLocation("V");
  shader_programs_[shader_program_name]->CreateUniformLocation("far_clipping");

  shader_programs_[shader_program_name]->CreateUniformLocation(
          "light_data"); // Containt all lights
  shader_programs_[shader_program_name]->CreateUniformLocation(
          "texture_sampler2D");
  shader_programs_[shader_program_name]->CreateUniformLocation(
          "material.reflectance");
  shader_programs_[shader_program_name]->CreateUniformLocation(
          "material.specularity");
  shader_programs_[shader_program_name]->CreateUniformLocation(
          "material.shinyness");
    shader_programs_[shader_program_name]->CreateUniformLocation(
          "material.texture_type");
}