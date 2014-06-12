#ifndef SCENE_H
#define SCENE_H

#include "../include/Camera.h"

class Scene {
public:
  Scene(Camera cam);
  ~Scene(){};

  void Render();
  void Update();

  void SetCamera(Camera cam);
  Camera GetCamera();

private:
  Camera cam_;
  
};

#endif  // SCENE_H
