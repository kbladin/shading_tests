// All preprocessor code is in separate string

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;

void main(){
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);
}

