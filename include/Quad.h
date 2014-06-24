#ifndef QUAD_H
#define QUAD_H

#include "../include/kalles_gl_lib/Mesh.h"

class Quad : public Mesh
{
public:
	Quad();
	~Quad(){};
  void Render();
private:
  void SetupVertexData();
  void SetupBuffers();
};

#endif