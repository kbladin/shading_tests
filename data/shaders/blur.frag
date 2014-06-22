// All preprocessor code is in separate string
in vec2 UV;

uniform sampler2D texture_sampler;
uniform vec2 pixel_size;

out vec3 color;

const int FILTER_SIZE = 29; // Odd number

void main(){
	vec3 val = vec3(0.0,0.0,0.0);
	// Filter x
	for (int i=0; i<FILTER_SIZE; ++i) {
		val += texture(texture_sampler, vec2(UV.x + (i - FILTER_SIZE/2)*pixel_size.x, UV.y)).xyz;
	}
	// Filter y
	for (int i=0; i<FILTER_SIZE; ++i) {
		val += texture(texture_sampler, vec2(UV.x, UV.y + (i - FILTER_SIZE/2)*pixel_size.y)).xyz;
	}
	color = val / (FILTER_SIZE*2);
}