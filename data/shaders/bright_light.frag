// All preprocessor code is in separate string

in vec2 UV;

uniform sampler2D rendered_texture_sampler;
uniform vec2 pixel_size;
 
out vec3 color;

void main(){
	vec3 color_sample = texture(rendered_texture_sampler, UV).xyz;
	float intensity = 1.0/3.0 * dot(color_sample, vec3(1));
	float step_intensity = step(0.7, intensity);

	color = vec3(step_intensity);
}