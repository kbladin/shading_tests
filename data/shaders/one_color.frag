// All preprocessor code is in separate string

uniform vec3 color_in;

// Ouput data
out vec3 color;

void main()
{
  vec3 material_diffuse_color = vec3(1.0,0.8,0.8);

  color = color_in;
}
