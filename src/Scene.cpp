#include <iostream>

#include "../include/MyMesh.h"

#include "../include/Scene.h"

Scene::Scene(Camera cam) : cam_(cam)
{
  meshes_.push_back(new MyMesh());
}

Scene::~Scene()
{
  for (int i = 0; i < meshes_.size(); ++i)
  {
    delete meshes_[i];
  }
}

void Scene::Render()
{
  glClear(GL_COLOR_BUFFER_BIT);
  for (int i = 0; i < meshes_.size(); ++i)
  {
    meshes_[i]->Render(&cam_, glm::mat4(1.0));
  }
}

void Scene::Update()
{
  cam_.UpdateMatrices();
}

void Scene::SetCamera(Camera cam)
{
  cam_ = cam;
}

Camera Scene::GetCamera()
{
  return cam_;
}