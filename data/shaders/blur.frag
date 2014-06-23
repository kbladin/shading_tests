// All preprocessor code is in separate string
in vec2 UV;

uniform sampler2D texture_sampler;
uniform vec2 pixel_size;
uniform int filter_size;

out vec3 color;

void main(){
	vec3 val = vec3(0.0,0.0,0.0);
	// Filter x
	for (int i=0; i<filter_size; ++i) {
		val += texture(texture_sampler, vec2(UV.x + (i - filter_size/2)*pixel_size.x, UV.y)).xyz;
	}
	// Filter y
	for (int i=0; i<filter_size; ++i) {
		val += texture(texture_sampler, vec2(UV.x, UV.y + (i - filter_size/2)*pixel_size.y)).xyz;
	}
	color = val / (filter_size*2);
}