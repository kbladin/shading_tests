// All preprocessor code is in separate string

// Input data
layout(location = 0) in vec3 vertexPosition_modelspace;
// Uniforms
uniform mat4 MVP;

// Output

void main(){
  // Output position of the vertex
  gl_Position = MVP * vec4(vertexPosition_modelspace,1);
}