#include "../include/Scene.h"

Scene::Scene(Camera cam)
{
  cam_ = cam;
}

void Scene::Render()
{

}

void Scene::Update()
{

}

void Scene::SetCamera(Camera cam)
{
  cam_ = cam;
}

Camera Scene::GetCamera()
{
  return cam_;
}