#include "GL/glew.h"
#include "GL/glfw3.h"
#ifndef Q_MOC_RUN
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#endif

#include "../include/kalles_gl_lib/Camera.h"
#include "../include/MyShaderManager.h"

#include "../include/RenderTexture.h"

RenderTexture::RenderTexture(int width, int height)
{
  width_ = width;
  height_ = height;
  SetupFrameBuffer();
}

void RenderTexture::SetupFrameBuffer()
{
  glGenFramebuffers(1, &frame_buffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);

  glGenTextures(1, &rendered_texture_);
  glBindTexture(GL_TEXTURE_2D, rendered_texture_);
  // Give an empty image to OpenGL ( the last "0" )
  glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RGB,
          width_,
          height_,
          0,
          GL_RGB,
          GL_UNSIGNED_BYTE,
          0);
  // Poor filtering. Needed !
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glGenRenderbuffers(1, &depth_render_buffer_);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
  glFramebufferRenderbuffer(
          GL_FRAMEBUFFER,
          GL_DEPTH_ATTACHMENT,
          GL_RENDERBUFFER,
          depth_render_buffer_);
  // Set "renderedTexture" as our colour attachement #0
  glFramebufferTexture(
          GL_FRAMEBUFFER,
          GL_COLOR_ATTACHMENT0,
          rendered_texture_,
          0);
  // Set the list of draw buffers.
  GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

  // Always check that our framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR: RenderTexture, SetupFrameBuffer()!" << std::endl;
}

GLuint RenderTexture::GetFrameBuffer()
{
  return frame_buffer_;
}

int RenderTexture::GetWidth()
{
  return width_;
}

int RenderTexture::GetHeight()
{
  return height_;
}
