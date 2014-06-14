#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "../include/kalles_gl_lib/Mesh.h"

class RenderTexture : public Mesh
{
public:
	RenderTexture(int width, int height);
	~RenderTexture(){};
  void Render(Camera* camera, glm::mat4 model_transform);
  GLuint GetFrameBuffer();
  int GetWidth();
  int GetHeight();
private:
  void SetupFrameBuffer();
  void SetupVertexData();
  void SetupBuffers();

  GLuint frame_buffer_;
  GLuint rendered_texture_; // Might be moved to a texturemanager later
  GLuint depth_render_buffer_;

  int width_, height_;
};

#endif