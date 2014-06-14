// All preprocessor code is in separate string

in vec2 UV;

uniform sampler2D rendered_texture_sampler;
uniform vec2 pixel_size;
 
out vec3 color;

void main(){
	// Sobel filter x
    vec3 grad_x =
    	(
		- texture(rendered_texture_sampler, vec2(UV.x - pixel_size.x, UV.y)).xyz * 2/8 +
		- texture(rendered_texture_sampler, vec2(UV.x - pixel_size.x, UV.y + pixel_size.y)).xyz * 1/8 + 
		- texture(rendered_texture_sampler, vec2(UV.x - pixel_size.x, UV.y - pixel_size.y)).xyz * 1/8 +
		texture(rendered_texture_sampler, vec2(UV.x + pixel_size.x, UV.y)).xyz * 2/8 +
		texture(rendered_texture_sampler, vec2(UV.x + pixel_size.x, UV.y + pixel_size.y)).xyz * 1/8 + 
		texture(rendered_texture_sampler, vec2(UV.x + pixel_size.x, UV.y - pixel_size.y)).xyz * 1/8
		);
	// Sobel filter y
    vec3 grad_y =
    	(
		- texture(rendered_texture_sampler, vec2(UV.x, UV.y - pixel_size.y)).xyz * 2/8 +
		- texture(rendered_texture_sampler, vec2(UV.x + pixel_size.x, UV.y - pixel_size.y)).xyz * 1/8 + 
		- texture(rendered_texture_sampler, vec2(UV.x - pixel_size.x, UV.y - pixel_size.y)).xyz * 1/8 +
		texture(rendered_texture_sampler, vec2(UV.x, UV.y + pixel_size.y)).xyz * 2/8 +
		texture(rendered_texture_sampler, vec2(UV.x + pixel_size.x, UV.y + pixel_size.y)).xyz * 1/8 + 
		texture(rendered_texture_sampler, vec2(UV.x - pixel_size.x, UV.y + pixel_size.y)).xyz * 1/8
		);
	vec3 abs_val = abs(grad_x) + abs(grad_y);

	// Intensity as threshold value, 1 == edge, 0 == not edge.
	float intensity = step(0.1,1.0/3.0 * (abs_val.x + abs_val.y + abs_val.z));
	color = vec3(intensity, intensity, intensity);
}