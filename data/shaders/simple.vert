// All preprocessor code is in separate string

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec3 vertex_normal_modelspace;
layout(location = 2) in vec2 vertex_uv;

uniform mat4 MVP;

out vec3 normal;

void main(){
	normal = vertex_normal_modelspace;
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = MVP * vec4(vertex_position_modelspace,1);
}

