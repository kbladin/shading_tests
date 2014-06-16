#include "GL/glew.h"
#include "GL/glfw3.h"

#include "../include/kalles_gl_lib/Camera.h"
#include "../include/MyShaderManager.h"

#include "../include/RenderTexture.h"

RenderTexture::RenderTexture(int width, int height) : Mesh()
{
  width_ = width;
  height_ = height;
  SetupVertexData();
  SetupBuffers();
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

void RenderTexture::SetupBuffers()
{
  // The fullscreen quad's FBO
  glGenVertexArrays(1, &vertex_array_id_);
  glBindVertexArray(vertex_array_id_);
  // Generate the vertex position buffer
  glGenBuffers(1, &vertex_position_buffer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer_id_);
  glBufferData(
          GL_ARRAY_BUFFER,
          sizeof(glm::vec3) * vertex_position_data_.size(),
          &vertex_position_data_[0],
          GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
          0,                  // attribute, same as in shader.
          3,                  // size
          GL_FLOAT,           // type
          GL_FALSE,           // normalized?
          0,                  // stride
          reinterpret_cast<void*>(0));  // array buffer offset
  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  SetupFrameBuffer();
}

void RenderTexture::SetupVertexData()
{
  vertex_position_data_.resize(6);
  // Setup the quad, two triangles.
  vertex_position_data_[0] = (glm::vec3(-1.0f, -1.0f, 0.0f));
  vertex_position_data_[1] = (glm::vec3(1.0f, -1.0f, 0.0f));
  vertex_position_data_[2] = (glm::vec3(-1.0f, 1.0f, 0.0f));
  vertex_position_data_[3] = (glm::vec3(-1.0f, 1.0f, 0.0f));
  vertex_position_data_[4] = (glm::vec3(1.0f, -1.0f, 0.0f));
  vertex_position_data_[5] = (glm::vec3(1.0f, 1.0f, 0.0f));
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

void RenderTexture::Render(Camera* camera, glm::mat4 model_transform)
{
  //glBindTexture(GL_TEXTURE_2D, rendered_texture_);
  glBindVertexArray(vertex_array_id_);
  // Draw the triangles !
  glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
  glBindVertexArray(0);
}
