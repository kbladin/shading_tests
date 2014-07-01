#ifndef MY_MESH_H
#define MY_MESH_H

#include "../include/kalles_gl_lib/Mesh.h"

class MyMesh : public Mesh
{
public:
  MyMesh(const char* file_path);
  ~MyMesh(){};
  glm::mat4 GetModelTransform();

  void Render();
  bool IsCorrupt(){ return corrupt_; };
  
  glm::vec3 position_;
  glm::fquat quaternion_;
private:
  bool corrupt_;
};

#endif