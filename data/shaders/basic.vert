// All preprocessor code is in separate string

// Input data
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV_modelspace;

// Uniforms
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform mat4 V;

uniform vec3 light_position_worldspace;

// Output data
out vec3 position_worldspace;
out vec3 position_viewspace;
out vec3 position_modelspace;
//out vec3 normal_viewspace;
out vec3 normal_worldspace;
out vec2 uv;

//out vec3 view_direction_to_fragment_viewspace;
out vec3 view_direction_to_fragment_worldspace;
//out vec3 light_position_viewspace;
//out vec3 light_direction_to_fragment_viewspace;

void main(){
  
  uv = vertexUV_modelspace;

  position_worldspace = vec3(M * vec4(vertexPosition_modelspace,1));

  vec3 vertex_position_viewspace =
          vec3( MV * vec4(vertexPosition_modelspace,1));
  vec3 vertex_position_worldspace =
          vec3( M * vec4(vertexPosition_modelspace,1));
  /*view_direction_to_fragment_viewspace =
          vec3(0,0,0) + vertex_position_viewspace;*/
  view_direction_to_fragment_worldspace = vec3(inverse(V) * (vec4(0.0,0.0,0.0,1.0) - vec4(vertex_position_viewspace, 1.0)));

  position_modelspace = vertexPosition_modelspace;

  position_viewspace = vec3(MV * vec4(vertexPosition_modelspace,1));

  //light_position_viewspace = vec3( V * vec4(light_position_worldspace,1));


  //normal_viewspace = vec3(MV * vec4(vertexNormal_modelspace,0));
  normal_worldspace = vec3(M * vec4(vertexNormal_modelspace,0));
	
  // Output position of the vertex
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);
}