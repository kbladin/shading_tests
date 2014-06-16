#include <iostream>

#include "../include/MyMesh.h"
#include "../include/RenderTexture.h"
#include "../include/MyShaderManager.h"

#include "../include/Scene.h"

Scene::Scene(Camera* cam) : cam_(cam)
{
  meshes_.push_back(new MyMesh("../data/meshes/suzanne.obj"));
  render_textures_.push_back(new RenderTexture(640*4,480*4)); // *2 since it's retina
  render_textures_.push_back(new RenderTexture(640*4,480*4)); // *2 since it's retina
  render_textures_.push_back(new RenderTexture(640*4,480*4)); // *2 since it's retina

  glBindTexture(GL_TEXTURE_2D, render_textures_[0]->rendered_texture_);
  glActiveTexture(GL_TEXTURE1); // Render to this texture
  glBindTexture(GL_TEXTURE_2D, render_textures_[1]->rendered_texture_);
  glActiveTexture(GL_TEXTURE2); // Render to this texture
  glBindTexture(GL_TEXTURE_2D, render_textures_[2]->rendered_texture_);
  glActiveTexture(GL_TEXTURE3); // Render to this texture

  for (int i = 0; i < 1; ++i) // Should be as many as in MyShaderManager
  {
    light_sources_.push_back(LightSource());
  }

}

Scene::~Scene()
{
  for (int i = 0; i < meshes_.size(); ++i)
  {
    delete meshes_[i];
  }
  for (int i = 0; i < render_textures_.size(); ++i)
  {
    delete render_textures_[i];
  }
  delete cam_;
}

void Scene::Render(int width, int height)
{
/*  // Render to our framebuffer in our first RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[0]->GetFrameBuffer());
  glViewport(
          0,
          0,
          render_textures_[0]->GetWidth(),
          render_textures_[0]->GetHeight());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (int i = 0; i < meshes_.size(); ++i)
  {
    meshes_[i]->RenderPhong(cam_, glm::mat4(1.0));
  }
*/
  const char* shader_name = "Phong"; 
  MyShaderManager::Instance()->UseProgram(shader_name);
  MyShaderManager::Instance()->GetShaderProgramFromName(
        "Phong")->Uniform1fv("light_data", 16 * light_sources_.size() * MyShaderManager::Instance()->N_LIGHTSOURCES, (float*)(&light_sources_[0]));

  // Render to our framebuffer in our second RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[1]->GetFrameBuffer());
  glViewport(
          0,
          0,
          render_textures_[1]->GetWidth(),
          render_textures_[1]->GetHeight());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < meshes_.size(); ++i)
  {
    meshes_[i]->RenderPhong(cam_, glm::mat4(1.0));
  }


  // Render to our framebuffer in our last RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[2]->GetFrameBuffer());
  glViewport(
          0,
          0,
          render_textures_[2]->GetWidth(),
          render_textures_[2]->GetHeight());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  MyShaderManager::Instance()->UseProgram("Bright_Light");
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Bright_Light")->Uniform1i("rendered_texture_sampler", 1);
/*  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Edge_Detector")->Uniform2f("pixel_size",
                  0.5/static_cast<float>(width), // 0.5 since it's retina
                  0.5/static_cast<float>(height));
  */
  render_textures_[1]->Render(cam_, glm::mat4(1.0));




  // Render to screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, width * 2, height * 2); // *2 Because of retina screen.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  MyShaderManager::Instance()->UseProgram("Texture_Combiner");
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Texture_Combiner")->Uniform1i("texture_sampler1", 1);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Texture_Combiner")->Uniform1i("texture_sampler2", 2);

  render_textures_[2]->Render(cam_, glm::mat4(1.0));
}

void Scene::Update()
{
  cam_->UpdateMatrices();
}

Camera* Scene::GetCamera()
{
  return cam_;
}