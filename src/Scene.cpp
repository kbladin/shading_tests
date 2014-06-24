#include <iostream>

#include <AntTweakBar.h>

#include "../include/MyMesh.h"
#include "../include/RenderTexture.h"
#include "../include/MyShaderManager.h"
#include "../include/SettingsManager.h"

#include "../include/Scene.h"

Scene::Scene(Camera* cam) : cam_(cam)
{
  meshes_.push_back(new MyMesh("../data/meshes/suzanne.obj"));
  render_textures_.push_back(new RenderTexture(4*640,4*480));
  render_textures_.push_back(new RenderTexture(300,200));
  render_textures_.push_back(new RenderTexture(300,200));

  for (int i = 0; i < SettingsManager::Instance()->N_LIGHTSOURCES; ++i)
  {
    light_sources_.push_back(LightSource());
    light_source_meshes_.push_back(new MyMesh("../data/meshes/icosphere.obj"));
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
  for (int i = 0; i < light_source_meshes_.size(); ++i)
  {
    delete light_source_meshes_[i];
  }
  delete cam_;
}

void Scene::Render(int width, int height)
{
  glEnable(GL_CULL_FACE);

  for (int i = 0; i < render_textures_.size(); ++i)
  {
    glActiveTexture(GL_TEXTURE0 + i); // Render to this texture
    glBindTexture(GL_TEXTURE_2D, render_textures_[i]->rendered_texture_);
  }


  #ifdef NOT
  ////////////////////
  // 1. Render toon //
  ////////////////////

  // Render to our framebuffer in our first RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[0]->GetFrameBuffer());
  glViewport(
          0,
          0,
          render_textures_[0]->GetWidth(),
          render_textures_[0]->GetHeight());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (int i = 0; i < meshes_.size(); ++i)
  {
    // Matrix data
    glm::mat4 model_transform = glm::mat4(1.0);
    glm::mat4 V = cam_->GetViewMatrix();
    glm::mat4 MV = V * model_transform;
    glm::mat4 P = cam_->GetProjectionMatrix();
    glm::mat4 MVP = P * MV;

    MyShaderManager::Instance()->UseProgram("Toon");
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Toon")->UniformMatrix4fv("M", 1, false, &model_transform[0][0]);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Toon")->UniformMatrix4fv("V", 1, false, &V[0][0]);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Toon")->UniformMatrix4fv("MV", 1, false, &MV[0][0]);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Toon")->UniformMatrix4fv("MVP", 1, false, &MVP[0][0]);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Toon")->Uniform1fv(
                    "light_data",
                    16 * light_sources_.size() * SettingsManager::Instance()->N_LIGHTSOURCES,
                    (float*)(&light_sources_[0]));

    meshes_[i]->Render();
  }
  glUseProgram(0);

  //////////////////////////////
  // 1.5 Render Light sources //
  //////////////////////////////

  // Render to our framebuffer in our first RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[0]->GetFrameBuffer());
  glViewport(
          0,
          0,
          render_textures_[0]->GetWidth(),
          render_textures_[0]->GetHeight());
  for (int i = 0; i < light_sources_.size(); ++i)
  {
    // Matrix data
    glm::mat4 model_transform = glm::translate(
            glm::mat4(1.0f),
                    glm::vec3(
                              light_sources_[i].position.x,
                              light_sources_[i].position.y,
                              light_sources_[i].position.z));
    glm::mat4 V = cam_->GetViewMatrix();
    glm::mat4 MV = V * model_transform;
    glm::mat4 P = cam_->GetProjectionMatrix();
    glm::mat4 MVP = P * MV;

    MyShaderManager::Instance()->UseProgram("One_Color");
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "One_Color")->Uniform3f(
                    "color_in",
                    light_sources_[i].color.r,
                    light_sources_[i].color.g,
                    light_sources_[i].color.b);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "One_Color")->UniformMatrix4fv("MVP", 1, false, &MVP[0][0]);
    MyShaderManager::Instance()->UseProgram("One_Color");

    light_source_meshes_[i]->Render();
  }
  glUseProgram(0);

  /////////////////////////////
  // 2. Render Normal colors //
  /////////////////////////////

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
    // Matrix data
    glm::mat4 model_transform = glm::mat4(1.0);
    glm::mat4 V = cam_->GetViewMatrix();
    glm::mat4 MV = V * model_transform;
    glm::mat4 P = cam_->GetProjectionMatrix();
    glm::mat4 MVP = P * MV;

    MyShaderManager::Instance()->UseProgram("Normal_Color");
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Normal_Color")->UniformMatrix4fv("MVP", 1, false, &MVP[0][0]);

    meshes_[i]->Render();
  }
  glUseProgram(0);

  //////////////////////////////
  // 2.5 Render Light sources //
  //////////////////////////////

  // Render to our framebuffer in our first RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[1]->GetFrameBuffer());
  glViewport(
          0,
          0,
          render_textures_[1]->GetWidth(),
          render_textures_[1]->GetHeight());
  for (int i = 0; i < light_sources_.size(); ++i)
  {
    // Matrix data
    glm::mat4 model_transform = glm::translate(
            glm::mat4(1.0f),
                    glm::vec3(
                              light_sources_[i].position.x,
                              light_sources_[i].position.y,
                              light_sources_[i].position.z));
    glm::mat4 V = cam_->GetViewMatrix();
    glm::mat4 MV = V * model_transform;
    glm::mat4 P = cam_->GetProjectionMatrix();
    glm::mat4 MVP = P * MV;

    MyShaderManager::Instance()->UseProgram("One_Color");
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "One_Color")->Uniform3f(
                    "color_in",
                    light_sources_[i].color.r,
                    light_sources_[i].color.g,
                    light_sources_[i].color.b);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "One_Color")->UniformMatrix4fv("MVP", 1, false, &MVP[0][0]);
    MyShaderManager::Instance()->UseProgram("One_Color");

    light_source_meshes_[i]->Render();
  }
  glUseProgram(0);


  /////////////////////
  // 3. Render Edges //
  /////////////////////

  MyShaderManager::Instance()->UseProgram("Edge_Detector");
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Edge_Detector")->Uniform1i("rendered_texture_sampler", 1);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Edge_Detector")->Uniform2f("pixel_size",
                  1.0/static_cast<float>
                  (render_textures_[1]->GetWidth()),
                  1.0/static_cast<float>
                  (render_textures_[1]->GetHeight()));
  // Render to our framebuffer in our second RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[2]->GetFrameBuffer());
  glViewport(
          0,
          0,
          render_textures_[2]->GetWidth(),
          render_textures_[2]->GetHeight());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  quad_.Render();
  glUseProgram(0);


  ///////////////////////////////////////
  // 4. Render Combined Toon and Edges //
  ///////////////////////////////////////

  MyShaderManager::Instance()->UseProgram("Texture_Combiner");
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Texture_Combiner")->Uniform1i("texture_sampler1", 0);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Texture_Combiner")->Uniform1i("texture_sampler2", 2);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Texture_Combiner")->Uniform1f("multiplier1", 1.0);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Texture_Combiner")->Uniform1f("multiplier2", -1.0);
  // Render to screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, width * 2, height * 2); // *2 Because of retina screen.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  quad_.Render();

  #endif

  /////////////////////
  // 1. Render Phong //
  /////////////////////

  // Render to our framebuffer in our first RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[0]->GetFrameBuffer());
  glViewport(
          0,
          0,
          render_textures_[0]->GetWidth(),
          render_textures_[0]->GetHeight());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (int i = 0; i < meshes_.size(); ++i)
  {
    // Matrix data
    glm::mat4 model_transform = glm::mat4(1.0);
    glm::mat4 V = cam_->GetViewMatrix();
    glm::mat4 MV = V * model_transform;
    glm::mat4 P = cam_->GetProjectionMatrix();
    glm::mat4 MVP = P * MV;

    MyShaderManager::Instance()->UseProgram("Phong");
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Phong")->UniformMatrix4fv("M", 1, false, &model_transform[0][0]);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Phong")->UniformMatrix4fv("V", 1, false, &V[0][0]);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Phong")->UniformMatrix4fv("MV", 1, false, &MV[0][0]);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Phong")->UniformMatrix4fv("MVP", 1, false, &MVP[0][0]);
    MyShaderManager::Instance()->UseProgram("Phong");
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Phong")->Uniform1fv(
                    "light_data",
                    16 * light_sources_.size() * SettingsManager::Instance()->N_LIGHTSOURCES,
                    (float*)(&light_sources_[0]));

    meshes_[i]->Render();
  }
  glUseProgram(0);


  //////////////////////////////
  // 1.5 Render Light sources //
  //////////////////////////////

  // Render to our framebuffer in our first RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[0]->GetFrameBuffer());
  glViewport(
          0,
          0,
          render_textures_[0]->GetWidth(),
          render_textures_[0]->GetHeight());
  for (int i = 0; i < light_sources_.size(); ++i)
  {
    // Matrix data
    glm::mat4 model_transform = glm::translate(
            glm::mat4(1.0f),
                    glm::vec3(
                              light_sources_[i].position.x,
                              light_sources_[i].position.y,
                              light_sources_[i].position.z));
    glm::mat4 V = cam_->GetViewMatrix();
    glm::mat4 MV = V * model_transform;
    glm::mat4 P = cam_->GetProjectionMatrix();
    glm::mat4 MVP = P * MV;

    glm::vec3 c = light_sources_[i].color;

    c *= (light_sources_[i].intensity + 50) / 100.0f;

    MyShaderManager::Instance()->UseProgram("One_Color");
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "One_Color")->Uniform3f(
                    "color_in",
                    c.r,
                    c.g,
                    c.b);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "One_Color")->UniformMatrix4fv("MVP", 1, false, &MVP[0][0]);
    MyShaderManager::Instance()->UseProgram("One_Color");

    light_source_meshes_[i]->Render();
  }
  glUseProgram(0);

  ////////////////////////////
  // 2. Render Bright Light //
  ////////////////////////////

  MyShaderManager::Instance()->UseProgram("Bright_Light");
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Bright_Light")->Uniform1i("rendered_texture_sampler", 0);
  // Render to our framebuffer in our second RenderTexture
  glBindFramebuffer(GL_FRAMEBUFFER, render_textures_[1]->GetFrameBuffer());
  glViewport(
          0,
          0,
          render_textures_[1]->GetWidth(),
          render_textures_[1]->GetHeight());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  quad_.Render();
  glUseProgram(0);


  ////////////////////////////////////
  // 3. Render Blurred Bright Light //
  ////////////////////////////////////

  // Ping pong
  int texture_to_sample = 1;
  int texture_to_render = 2;
  for (int ping_pong = 0; ping_pong < SettingsManager::Instance()->n_blur_loops; ++ping_pong)
  {
    MyShaderManager::Instance()->UseProgram("Blur");
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Blur")->Uniform1i("texture_sampler", texture_to_sample);
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Blur")->Uniform2f("pixel_size",
                    1.0/static_cast<float>
                    (render_textures_[texture_to_sample]->GetWidth()),
                    1.0/static_cast<float>
                    (render_textures_[texture_to_sample]->GetHeight()));
    MyShaderManager::Instance()->GetShaderProgramFromName(
            "Blur")->Uniform1i("filter_size", SettingsManager::Instance()->filter_size);
    // Render to our framebuffer in our second/third RenderTexture
    glBindFramebuffer(
            GL_FRAMEBUFFER,
            render_textures_[texture_to_render]->GetFrameBuffer());
    glViewport(
            0,
            0,
            render_textures_[texture_to_render]->GetWidth(),
            render_textures_[texture_to_render]->GetHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    quad_.Render();

    int tmp = texture_to_render;
    texture_to_render = texture_to_sample;
    texture_to_sample = tmp;
  }

  //////////////////////////////////////////
  // 4. Render Combined Phong and Blurred //
  //////////////////////////////////////////

  MyShaderManager::Instance()->UseProgram("Texture_Combiner");
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Texture_Combiner")->Uniform1i("texture_sampler1", 0);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Texture_Combiner")->Uniform1i("texture_sampler2", texture_to_sample);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Texture_Combiner")->Uniform1f("multiplier1", SettingsManager::Instance()->multiplier1);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          "Texture_Combiner")->Uniform1f("multiplier2", SettingsManager::Instance()->multiplier2);
  // Render to screen
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, width * 2, height * 2); // *2 Because of retina screen.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  quad_.Render();
}

void Scene::Update()
{
  cam_->UpdateMatrices();

}

Camera* Scene::GetCamera()
{
  return cam_;
}
