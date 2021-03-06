#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "../include/kalles_gl_lib/Camera.h"
#include "../include/kalles_gl_lib/Mesh.h"
#include "../include/MyMesh.h"
#include "../include/RenderTexture.h"
#include "../include/Quad.h"

struct AmbientLight {
  float intensity;
  glm::vec3 color;

  AmbientLight(){
    intensity = 0.2f;
    color = glm::vec3(1.0f,1.0f,1.0f);
  }
};

struct LightSource {
	// 0
  float intensity;
  glm::vec3 color;
  // 1
  glm::vec4 position;
  // 2
  float constant_attenuation, linear_attenuation, quadratic_attenuation;
  float spot_cutoff;
  float spot_exponent;
  // 3
  glm::vec3 spot_direction;

  LightSource(){
    intensity = 50.0f;
    color = glm::vec3(1.0f, 0.8f, 0.6f);
    position = glm::vec4(5.0f,5.0f,5.0f,1.0f); // w == 0.0 ? => directional
    constant_attenuation = 0.0f;
    linear_attenuation = 0.0f;
    quadratic_attenuation = 1.0f;
    spot_cutoff = 100.0f; // spot_cutoff > 90.0 ? => point light
    spot_exponent = 10.0f;
    spot_direction = glm::vec3(0.0f,-1.0f,0.0f);
  }
};

class Scene
{
public:
  Scene(Camera* cam);
  ~Scene();
  void AddMesh(MyMesh* mesh);
  int GetNumberOfMeshes();
  void RenderToon(int width, int height);
  void Render(int width, int height);
  void Update();
  Camera* GetCamera();
  std::vector<LightSource> light_sources_;
  AmbientLight amb_light_;
private:
  Camera* cam_;
  std::vector<MyMesh*> meshes_;
  std::vector<RenderTexture*> render_textures_;
  std::vector<MyMesh*> light_source_meshes_;
  Quad quad_;
};

#endif // SCENE_H