#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "GL/glew.h"
#include "GL/glfw3.h"

class RenderTexture
{
public:
	RenderTexture(int width, int height);
	~RenderTexture(){};
  GLuint GetFrameBuffer();
  int GetWidth();
  int GetHeight();

  GLuint frame_buffer_;
  GLuint rendered_texture_; // Might be moved to a texturemanager later
  GLuint depth_render_buffer_;
private:
  void SetupFrameBuffer();
  int width_, height_;
};

#endif