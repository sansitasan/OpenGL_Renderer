#version 450 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 M, VP;

void main(){
  gl_Position = VP * M * vec4(vertexPosition_modelspace, 1);
}