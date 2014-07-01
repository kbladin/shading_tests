#include "GL/glew.h"
#include "GL/glfw3.h"

#include "../include/kalles_gl_lib/Camera.h"
#include "../include/kalles_gl_lib/MeshLoader.h"
#include "../include/MyShaderManager.h"

#include "../include/MyMesh.h"

MyMesh::MyMesh(const char* file_path) : Mesh()
{
  if (!loadMesh_assimp(
  file_path,
  element_data_,
  vertex_position_data_, 
  vertex_uv_data_, 
  vertex_normal_data_))
  {
    corrupt_ = true;
  } else
  {
    corrupt_ = false;
    SetupBuffers();
  }
}

glm::mat4 MyMesh::GetModelTransform()
{
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), position_);
  glm::mat4 rotation = glm::mat4_cast(quaternion_);

  return translation * rotation;
}

void MyMesh::Render()
{
  glBindVertexArray(vertex_array_id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id_);
  //MyTextureManager::Instance()->BindTexture(material_.GetDiffuseTextureID());

  glDrawElements(
          GL_TRIANGLES,      // mode
          element_data_.size(),    // count
          GL_UNSIGNED_SHORT,   // type
          reinterpret_cast<void*>(0));  // element array buffer offset

  // Unbind
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}