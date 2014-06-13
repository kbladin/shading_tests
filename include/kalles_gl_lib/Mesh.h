#ifndef MESH_H
#define MESH_H

#include <vector>

// External
#ifndef Q_MOC_RUN
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#endif
#include "GL/glew.h"
#include "GL/glfw3.h"

#include "../../include/kalles_gl_lib/Camera.h"

class Mesh
{
  friend class Renderer;
public:
	Mesh();
	virtual ~Mesh();
  void SetupBuffers();
  virtual void Render(Camera* camera, glm::mat4 model_transform) = 0;
  void DeleteBuffers();
protected:
  GLuint vertex_array_id_;
  GLuint element_buffer_id_;
  GLuint vertex_position_buffer_id_;
  GLuint vertex_normal_buffer_id_;
  GLuint vertex_uv_buffer_id_;

  std::vector<glm::vec3> vertex_position_data_;
  std::vector<glm::vec3> vertex_normal_data_;
  std::vector<glm::vec2> vertex_uv_data_;
  std::vector<GLushort> element_data_;

  //Material material_;
};

#endif // MESH_H