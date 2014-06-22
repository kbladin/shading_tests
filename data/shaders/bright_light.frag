// All preprocessor code is in separate string

in vec2 UV;

uniform sampler2D rendered_texture_sampler;
uniform vec2 pixel_size;
 
out vec3 color;

void main(){
	vec3 color_sample = texture(rendered_texture_sampler, UV).xyz;
	float intensity = 1.0/3.0 * dot(color_sample, vec3(1));
	color = (step(0.5, intensity) == 0) ? vec3(0,0,0) : color_sample;
}