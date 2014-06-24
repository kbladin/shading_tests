#include "GL/glew.h"
#include "GL/glfw3.h"

#include "../include/MyShaderManager.h"

#include "../include/Quad.h"

Quad::Quad() : Mesh()
{
  SetupVertexData();
  SetupBuffers();
}

void Quad::SetupBuffers()
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
}

void Quad::SetupVertexData()
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

void Quad::Render()
{
  glBindVertexArray(vertex_array_id_);
  // Draw the triangles !
  glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
  glBindVertexArray(0);
}
