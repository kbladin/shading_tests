#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "../include/kalles_gl_lib/Camera.h"
#include "../include/kalles_gl_lib/Mesh.h"

class Scene
{
public:
  Scene(Camera cam);
  ~Scene();

  void Render();
  void Update();

  void SetCamera(Camera cam);
  Camera GetCamera();

  Camera cam_;
private:
  std::vector<Mesh*> meshes_;
};

#endif  // SCENE_H