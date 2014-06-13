#ifndef MY_MESH_H
#define MY_MESH_H

#include "../include/kalles_gl_lib/Mesh.h"

class MyMesh : public Mesh
{
  friend class Renderer;
public:
	MyMesh(const char* file_path);
	~MyMesh(){};
  void Render(Camera* camera, glm::mat4 model_transform);
};

#endif