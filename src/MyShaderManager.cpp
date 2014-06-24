#include "../include/MyShaderManager.h"
#include "../include/SettingsManager.h"

MyShaderManager::MyShaderManager() : ShaderManager()
{
  AddAllShaders();
  AddAllShaderPrograms();
}

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
  std::stringstream preprocessor_phong_frag;
  preprocessor_phong_frag <<
          "#version 330 core \n" <<
          "#define N_LIGHTSOURCES " << SettingsManager::Instance()->N_LIGHTSOURCES << "\n";

  // Create shaders
  Shader* simple_mvp_vert = new Shader(
      "../data/shaders/simple.vert",
      preprocessor.str().c_str(),
      GL_VERTEX_SHADER);

  Shader* pass_through_vert = new Shader(
      "../data/shaders/pass_through.vert",
      preprocessor.str().c_str(),
      GL_VERTEX_SHADER);

  Shader* phong_vert = new Shader(
      "../data/shaders/phong.vert",
      preprocessor.str().c_str(),
      GL_VERTEX_SHADER);

  Shader* mvp_vert = new Shader(
      "../data/shaders/mvp.vert",
      preprocessor.str().c_str(),
      GL_VERTEX_SHADER);

  Shader* simple_mvp_frag = new Shader(
      "../data/shaders/simple.frag",
      preprocessor.str().c_str(),
      GL_FRAGMENT_SHADER);

  Shader* normal_colors_frag = new Shader(
      "../data/shaders/normal_colors.frag",
      preprocessor.str().c_str(),
      GL_FRAGMENT_SHADER);

  Shader* edge_detection_frag = new Shader(
      "../data/shaders/edge_detection.frag",
      preprocessor.str().c_str(),
      GL_FRAGMENT_SHADER);

  Shader* texture_combiner_frag = new Shader(
      "../data/shaders/texture_combiner.frag",
      preprocessor.str().c_str(),
      GL_FRAGMENT_SHADER);

  Shader* phong_frag = new Shader(
      "../data/shaders/phong.frag",
      preprocessor_phong_frag.str().c_str(),
      GL_FRAGMENT_SHADER);

  Shader* bright_light_frag = new Shader(
      "../data/shaders/bright_light.frag",
      preprocessor_phong_frag.str().c_str(),
      GL_FRAGMENT_SHADER);

  Shader* blur_frag = new Shader(
      "../data/shaders/blur.frag",
      preprocessor_phong_frag.str().c_str(),
      GL_FRAGMENT_SHADER);

  Shader* toon_frag = new Shader(
      "../data/shaders/toon.frag",
      preprocessor_phong_frag.str().c_str(),
      GL_FRAGMENT_SHADER);

  Shader* one_color_frag = new Shader(
      "../data/shaders/one_color.frag",
      preprocessor_phong_frag.str().c_str(),
      GL_FRAGMENT_SHADER);

  // Put shaders in the map
  shaders_.insert(StringShaderPair("Simple_Vert", simple_mvp_vert));
  shaders_.insert(StringShaderPair("Pass_Through_Vert", pass_through_vert));
  shaders_.insert(StringShaderPair("Phong_Vert", phong_vert));
  shaders_.insert(StringShaderPair("MVP_Vert", mvp_vert));

  shaders_.insert(StringShaderPair("Simple_Frag", simple_mvp_frag));
  shaders_.insert(StringShaderPair("Normal_Colors_Frag", normal_colors_frag));
  shaders_.insert(StringShaderPair("Edge_Detection_Frag", edge_detection_frag));
  shaders_.insert(StringShaderPair("Texture_Combiner_Frag", texture_combiner_frag));
  shaders_.insert(StringShaderPair("Phong_Frag", phong_frag));
  shaders_.insert(StringShaderPair("Bright_Light_Frag", bright_light_frag));
  shaders_.insert(StringShaderPair("Blur_Frag", blur_frag));
  shaders_.insert(StringShaderPair("Toon_Frag", toon_frag));
  shaders_.insert(StringShaderPair("One_Color_Frag", one_color_frag));
}

void MyShaderManager::AddAllShaderPrograms()
{
  AddSimpleShaderProgram();
  AddNormalColorShaderProgram();
  AddEdgeDetectorShaderProgram();
  AddTextureCombinerShaderProgram();
  AddPhongShaderProgram();
  AddBrightLightShaderProgram();
  AddBlurShaderProgram();
  AddToonShaderProgram();
  AddOneColorShaderProgram();
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

void MyShaderManager::AddPhongShaderProgram()
{
    // Create ShaderProgram
  ShaderProgram* shader_program =
  new ShaderProgram(shaders_["Phong_Vert"],
                    shaders_["Phong_Frag"]);
  // The name with which to refer to the ShaderProgram
  const char* shader_program_name = "Phong";
  // Put ShaderProgram in the map
  shader_programs_.insert(StringShaderProgPair(
      shader_program_name,
      shader_program) );
  // Create all locations
  shader_programs_[shader_program_name]->CreateUniformLocation("MVP");
  shader_programs_[shader_program_name]->CreateUniformLocation("MV");
  shader_programs_[shader_program_name]->CreateUniformLocation("M");
  shader_programs_[shader_program_name]->CreateUniformLocation("V");
  shader_programs_[shader_program_name]->CreateUniformLocation(
          "light_data"); // Containt all lights*/
  /*
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
  */
}

void MyShaderManager::AddToonShaderProgram()
{
    // Create ShaderProgram
  ShaderProgram* shader_program =
  new ShaderProgram(shaders_["Phong_Vert"],
                    shaders_["Toon_Frag"]);
  // The name with which to refer to the ShaderProgram
  const char* shader_program_name = "Toon";
  // Put ShaderProgram in the map
  shader_programs_.insert(StringShaderProgPair(
      shader_program_name,
      shader_program) );
  // Create all locations
  shader_programs_[shader_program_name]->CreateUniformLocation("MVP");
  shader_programs_[shader_program_name]->CreateUniformLocation("MV");
  shader_programs_[shader_program_name]->CreateUniformLocation("M");
  shader_programs_[shader_program_name]->CreateUniformLocation("V");
  shader_programs_[shader_program_name]->CreateUniformLocation(
          "light_data"); // Containt all lights*/
  /*
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
  */
}

void MyShaderManager::AddNormalColorShaderProgram()
{
    // Create ShaderProgram
  ShaderProgram* shader_program =
  new ShaderProgram(shaders_["Simple_Vert"],
                    shaders_["Normal_Colors_Frag"]);
  // The name with which to refer to the ShaderProgram
  const char* shader_program_name = "Normal_Color";
  // Put ShaderProgram in the map
  shader_programs_.insert(StringShaderProgPair(
      shader_program_name,
      shader_program) );

  shader_programs_[shader_program_name]->CreateUniformLocation("MVP");
}

void MyShaderManager::AddEdgeDetectorShaderProgram()
{
    // Create ShaderProgram
  ShaderProgram* shader_program =
  new ShaderProgram(shaders_["Pass_Through_Vert"],
                    shaders_["Edge_Detection_Frag"]);
  // The name with which to refer to the ShaderProgram
  const char* shader_program_name = "Edge_Detector";
  // Put ShaderProgram in the map
  shader_programs_.insert(StringShaderProgPair(
      shader_program_name,
      shader_program) );

  shader_programs_[shader_program_name]->CreateUniformLocation("rendered_texture_sampler");
  shader_programs_[shader_program_name]->CreateUniformLocation("pixel_size");
}

void MyShaderManager::AddTextureCombinerShaderProgram()
{
    // Create ShaderProgram
  ShaderProgram* shader_program =
  new ShaderProgram(shaders_["Pass_Through_Vert"],
                    shaders_["Texture_Combiner_Frag"]);
  // The name with which to refer to the ShaderProgram
  const char* shader_program_name = "Texture_Combiner";
  // Put ShaderProgram in the map
  shader_programs_.insert(StringShaderProgPair(
      shader_program_name,
      shader_program) );

  shader_programs_[shader_program_name]->CreateUniformLocation("texture_sampler1");
  shader_programs_[shader_program_name]->CreateUniformLocation("texture_sampler2");
  shader_programs_[shader_program_name]->CreateUniformLocation("multiplier1");
  shader_programs_[shader_program_name]->CreateUniformLocation("multiplier2");
}

void MyShaderManager::AddBrightLightShaderProgram()
{
    // Create ShaderProgram
  ShaderProgram* shader_program =
  new ShaderProgram(shaders_["Pass_Through_Vert"],
                    shaders_["Bright_Light_Frag"]);
  // The name with which to refer to the ShaderProgram
  const char* shader_program_name = "Bright_Light";
  // Put ShaderProgram in the map
  shader_programs_.insert(StringShaderProgPair(
      shader_program_name,
      shader_program) );

  shader_programs_[shader_program_name]->CreateUniformLocation("rendered_texture_sampler");
}

void MyShaderManager::AddBlurShaderProgram()
{
    // Create ShaderProgram
  ShaderProgram* shader_program =
  new ShaderProgram(shaders_["Pass_Through_Vert"],
                    shaders_["Blur_Frag"]);
  // The name with which to refer to the ShaderProgram
  const char* shader_program_name = "Blur";
  // Put ShaderProgram in the map
  shader_programs_.insert(StringShaderProgPair(
      shader_program_name,
      shader_program) );

  shader_programs_[shader_program_name]->CreateUniformLocation("texture_sampler");
  shader_programs_[shader_program_name]->CreateUniformLocation("pixel_size");
  shader_programs_[shader_program_name]->CreateUniformLocation("filter_size");
}

void MyShaderManager::AddOneColorShaderProgram()
{
    // Create ShaderProgram
  ShaderProgram* shader_program =
  new ShaderProgram(shaders_["MVP_Vert"],
                    shaders_["One_Color_Frag"]);
  // The name with which to refer to the ShaderProgram
  const char* shader_program_name = "One_Color";
  // Put ShaderProgram in the map
  shader_programs_.insert(StringShaderProgPair(
      shader_program_name,
      shader_program) );

  shader_programs_[shader_program_name]->CreateUniformLocation("MVP");
  shader_programs_[shader_program_name]->CreateUniformLocation("color_in");
}