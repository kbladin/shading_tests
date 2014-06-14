#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "../include/kalles_gl_lib/Camera.h"
#include "../include/kalles_gl_lib/Mesh.h"
#include "../include/RenderTexture.h"

struct LightSource {
	// 1
  glm::vec3 color;
  float intensity;
  // 2
  glm::vec4 position;
  // 3
  float constant_attenuation, linear_attenuation, quadratic_attenuation;
  float spot_cutoff;
  // 4
  glm::vec3 spot_direction;
  float spot_exponent;
  LightSource() {
    intensity = 50.0f;
    color = glm::vec3(1.0f, 0.9f, 0.8f);
    position = glm::vec4(10.0f,5.0f,20.0f,1.0f); // w == 0.0 ? => directional
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
  void Render(int width, int height);
  void Update();
  Camera* GetCamera();
private:
  Camera* cam_;
  std::vector<Mesh*> meshes_;
  std::vector<RenderTexture*> render_textures_;
  std::vector<LightSource> light_sources_;
};

#endif // SCENE_H