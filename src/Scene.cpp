#include <iostream>

#include "../include/MyMesh.h"
#include "../include/RenderTexture.h"
#include "../include/MyShaderManager.h"

#include "../include/Scene.h"

Scene::Scene(Camera* cam) : cam_(cam)
{
  //Material material;
  //material.SetDiffuseTexture("test_texture1");
  meshes_.push_back(new MyMesh("../data/meshes/suzanne.obj"));
  render_textures_.push_back(new RenderTexture(640*2,480*2)); // *2 since it's retina
  render_textures_.push_back(new RenderTexture(640*2,480*2)); // *2 since it's retina
}

Scene::~Scene()
{
  for (int i = 0; i < meshes_.size(); ++i)
  {
    delete meshes_[i];
  }
  delete cam_;
}

void Scene::Render(int width, int height)
{
  // Render to our framebuffer in our first RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[0]->GetFrameBuffer());
  glActiveTexture(GL_TEXTURE0); // Render to this texture
  glViewport(0,0,render_textures_[0]->GetWidth(),render_textures_[0]->GetHeight());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (int i = 0; i < meshes_.size(); ++i)
  {
    meshes_[i]->Render(cam_, glm::mat4(1.0));
  }

  // Render to our framebuffer in our second RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[1]->GetFrameBuffer());
  glViewport(0, 0, render_textures_[1]->GetWidth(), render_textures_[1]->GetHeight()); // Because of retina screen =/
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // To make sure we use the same name
  const char* shader_name = "Edge_Detector";
  MyShaderManager::Instance()->UseProgram(shader_name);
  //glUniform1i(texID, 0);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          shader_name)->Uniform1i("rendered_texture_sampler", 1);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          shader_name)->Uniform2f("pixel_size",
                  0.5/static_cast<float>(width), // 0.5 since it's retina
                  0.5/static_cast<float>(height));
  glActiveTexture(GL_TEXTURE1); // Render to this texture
  render_textures_[0]->Render(cam_, glm::mat4(1.0));


  // Render to screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, width * 2, height * 2); // Because of retina screen =/
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // To make sure we use the same name
  const char* shader_name2 = "Texture_Combiner";
  MyShaderManager::Instance()->UseProgram(shader_name2);
  //glUniform1i(texID, 0);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          shader_name2)->Uniform1i("texture_sampler", 0);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          shader_name2)->Uniform1i("texture_sampler2", 1);

  render_textures_[0]->Render(cam_, glm::mat4(1.0));
}

void Scene::Update()
{
  cam_->UpdateMatrices();
}

Camera* Scene::GetCamera()
{
  return cam_;
}