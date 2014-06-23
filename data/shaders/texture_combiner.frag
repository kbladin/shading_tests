// All preprocessor code is in separate string
in vec2 UV;

uniform sampler2D texture_sampler1;
uniform sampler2D texture_sampler2;

uniform float multiplier1;
uniform float multiplier2;

out vec3 color;

void main(){
	color = multiplier1 * texture(texture_sampler1, UV).xyz +
			multiplier2 * texture(texture_sampler2, UV).xyz;
}