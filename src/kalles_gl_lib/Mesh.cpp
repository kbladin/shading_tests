#include "GL/glew.h"
#include "../../include/MyShaderManager.h"

#include "../../include/kalles_gl_lib/Mesh.h"

Mesh::Mesh() {
  vertex_array_id_ = GL_FALSE;
  element_buffer_id_ = GL_FALSE;
  vertex_position_buffer_id_ = GL_FALSE;
  vertex_normal_buffer_id_ = GL_FALSE;
  vertex_uv_buffer_id_ = GL_FALSE;
}

void Mesh::SetupBuffers() {

  // Generate the vertex array
  glGenVertexArrays(1, &vertex_array_id_);
  glBindVertexArray(vertex_array_id_);

  // Generate the element buffer
  glGenBuffers(1, &element_buffer_id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id_);
  glBufferData(
          GL_ELEMENT_ARRAY_BUFFER,
          sizeof(GLushort) * element_data_.size(),
          &element_data_[0],
          GL_STATIC_DRAW);

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

  // Generate the vertex normal buffer
  glGenBuffers(1, &vertex_normal_buffer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_normal_buffer_id_);
  glBufferData(
          GL_ARRAY_BUFFER,
          sizeof(glm::vec3) * vertex_normal_data_.size(),
          &vertex_normal_data_[0],
          GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
          1,                  // attribute, same as in shader.
          3,                  // size
          GL_FLOAT,           // type
          GL_FALSE,           // normalized?
          0,                  // stride
          reinterpret_cast<void*>(0));  // array buffer offset

  // Generate the vertex UV buffer
  glGenBuffers(1, &vertex_uv_buffer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_uv_buffer_id_);
  glBufferData(
          GL_ARRAY_BUFFER,
          sizeof(glm::vec2) * vertex_uv_data_.size(),
          &vertex_uv_data_[0],
          GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(
          2,                  // attribute, same as in shader.
          2,                  // size
          GL_FLOAT,           // type
          GL_FALSE,           // normalized?
          0,                  // stride
          reinterpret_cast<void*>(0));  // array buffer offset

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Mesh::~Mesh() {
  DeleteBuffers();
}

void Mesh::DeleteBuffers() {
  if(vertex_uv_buffer_id_)
    glDeleteBuffers(1, &vertex_uv_buffer_id_);
  if(vertex_normal_buffer_id_)
    glDeleteBuffers(1, &vertex_normal_buffer_id_);
  if(vertex_position_buffer_id_)
    glDeleteBuffers(1, &vertex_position_buffer_id_);
  if(element_buffer_id_)
    glDeleteBuffers(1, &element_buffer_id_);
  if(vertex_array_id_)
    glDeleteVertexArrays(1, &vertex_array_id_);
}
