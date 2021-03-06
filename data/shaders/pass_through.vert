// All preprocessor code is in separate string

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex_position_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){
	gl_Position =  vec4(vertex_position_modelspace,1);
	UV = (vertex_position_modelspace.xy+vec2(1,1))/2.0;
}

