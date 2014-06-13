#include "../include/kalles_gl_lib/Camera.h"
#include "../include/kalles_gl_lib/MeshLoader.h"
#include "../include/MyShaderManager.h"

#include "../include/MyMesh.h"

MyMesh::MyMesh() : Mesh()
{
  loadMesh_assimp(
  "../data/meshes/suzanne.obj",
  element_data_,
  vertex_position_data_, 
  vertex_uv_data_, 
  vertex_normal_data_);

  SetupBuffers();
}

void MyMesh::Render(Camera* camera, glm::mat4 model_transform)
{
  // Matrix data
  glm::mat4 V = camera->GetViewMatrix();
  glm::mat4 MV = V * model_transform;
  glm::mat4 P = camera->GetProjectionMatrix();
  glm::mat4 MVP = P * MV;

  // To make sure we use the same name
  const char* shader_name = "Simple"; 
  MyShaderManager::Instance()->UseProgram(shader_name);
  MyShaderManager::Instance()->GetShaderProgramFromName(
          shader_name)->UniformMatrix4fv("MVP", 1, false, &MVP[0][0]);

  glBindVertexArray(vertex_array_id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id_);
  //TextureManager::Instance()->BindTexture(material_.GetDiffuseTextureID());

  glDrawElements(
          GL_TRIANGLES,      // mode
          element_data_.size(),    // count
          GL_UNSIGNED_SHORT,   // type
          reinterpret_cast<void*>(0));  // element array buffer offset

  // Unbind
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}
