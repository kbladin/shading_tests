// All preprocessor code is in separate string
in vec2 UV;

uniform sampler2D texture_sampler1;
uniform sampler2D texture_sampler2;

out vec3 color;

void main(){
	color = texture(texture_sampler1, UV).xyz +
			texture(texture_sampler2, UV).xyz;
}