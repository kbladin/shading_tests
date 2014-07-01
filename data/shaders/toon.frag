// All preprocessor code is in separate string
/*struct Material {
  float reflectance;
  float specularity;
  float shinyness;
  int texture_type;
};*/

struct LightSource {
  float intensity;
  vec3 color;
  vec4 position;
  float constant_attenuation, linear_attenuation, quadratic_attenuation;
  float spot_cutoff, spot_exponent;
  vec3 spot_direction;
};

// Internal data
LightSource lights[N_LIGHTSOURCES];

// Input data
in vec3 position_worldspace;
in vec3 position_modelspace;
in vec3 position_viewspace;
in vec3 normal_worldspace;

in vec3 view_direction_to_fragment_worldspace;
in vec2 uv;


in vec3 view_direction_to_fragment_viewspace;
in vec3 normal_viewspace;
in vec3 light_position_viewspace;
in vec3 light_direction_to_fragment_viewspace;

// Uniforms
uniform float far_clipping;
uniform LightSource light;
uniform sampler2D texture_sampler2D;
//uniform Material material;

uniform float light_data[16 * N_LIGHTSOURCES]; // 16 floats for each light
uniform float ambient_brightness;
uniform vec3 ambient_color;
// Ouput data
out vec3 color;

float stair(float val, int n_vals) {
  float step_length = 1.0 / n_vals;
  for (int i=0; i<n_vals; ++i) {
    if (step_length*i < val && val < step_length*(i+1)) {
      return (i+1)*step_length;// step((i+1)*step_length, val);
    }
  }
  return 0;
}

void main()
{
  // Diffuse color
  vec3 material_diffuse_color = vec3(1.0,0.8,0.8);

  // Convert light data to LightSource struct
  // (readability is better with structs but it can be changed for efficiency)
  for (int i = 0; i < N_LIGHTSOURCES; ++i){
    lights[i] = LightSource(
      light_data[i*16 + 0],
      vec3(light_data[i*16 + 1], light_data[i*16 + 2], light_data[i*16 + 3]),
      vec4(light_data[i*16 + 4],light_data[i*16 + 5],light_data[i*16 + 6],light_data[i*16 + 7]),
      light_data[i*16 + 8], light_data[i*16 + 9], light_data[i*16 + 10],
      light_data[i*16 + 11], light_data[i*16 + 12],
      vec3(light_data[i*16 + 13], light_data[i*16 + 14], light_data[i*16 + 15])
      );
  }  

  // Directional vectors
  vec3 n = normalize(normal_worldspace);
  vec3 v = normalize(view_direction_to_fragment_worldspace);
  vec3 l; // Light direction to fragment
  float attenuation;

  // ----- Ambient light -----
  vec3 ambient = ambient_color * material_diffuse_color * ambient_brightness;
  // initialize total lighting with ambient lighting
  vec3 total_lighting = ambient;

  for (int i = 0; i < N_LIGHTSOURCES; ++i){ // For all light sources
    if (lights[i].position.w == 0.0){
      attenuation = 1.0; // No attenuation
      l = normalize(vec3(lights[i].position));
    }
    else { // Point light or spot light
      vec3 position_to_lightsource = vec3(vec3(lights[i].position) - position_worldspace);
      float distance_to_light = length(position_to_lightsource);
      l = normalize(position_to_lightsource);
      attenuation = 1.0 /
            (lights[i].constant_attenuation
           + lights[i].linear_attenuation * distance_to_light
           + lights[i].quadratic_attenuation * distance_to_light * distance_to_light);
      if (lights[i].spot_cutoff <= 90.0) { // Spotlight, else it is a point light
        float clamped_cosine = max(0.0, dot(-l, normalize(lights[i].spot_direction)));
        if (clamped_cosine < cos(radians(lights[i].spot_cutoff))) {// outside of spotlight cone?
          attenuation = 0.0;
        }
        else {
          attenuation = attenuation * pow(clamped_cosine, lights[i].spot_exponent);
        }
      }
    }
    // ----- Diffuse light -----
    vec3 diffuse = attenuation * lights[i].color * material_diffuse_color *
            stair(dot(n,l),1);

    // ----- Specular light -----
    vec3 specular;
    if (dot(n,l) < 0.0) // Light source on wrong side
      specular = vec3(0.0, 0.0, 0.0);
    else {// Light source on right side
    vec3 r = reflect(l,n);
    float cos_alpha = clamp( dot( v,-r ), 0,1 );
    specular =
            lights[i].color *
            step(0.5, pow(cos_alpha, 32)) * // shine
            attenuation *
            1.0; // spec
    }
    total_lighting = total_lighting + lights[i].intensity * (diffuse + specular);
  }
  // Total light
  color = 1.0 * total_lighting;
}
