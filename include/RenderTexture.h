#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "../include/kalles_gl_lib/Mesh.h"

class RenderTexture : public Mesh
{
public:
	RenderTexture(int width, int height);
	~RenderTexture(){};
  void Render();
  GLuint GetFrameBuffer();
  int GetWidth();
  int GetHeight();


  GLuint frame_buffer_;
  GLuint rendered_texture_; // Might be moved to a texturemanager later
  GLuint depth_render_buffer_;

private:
  void SetupFrameBuffer();
  void SetupVertexData();
  void SetupBuffers();

  int width_, height_;
};

#endif