#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec3 vertexColor;

uniform mat4 MVP;
uniform mat4 W;
uniform vec3 eyePos;

out vec3 color;
out vec3 v_normal, v_view, v_pos;

void main(){
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
  v_pos = (W * vec4(vertexPosition_modelspace, 1)).xyz;
  v_normal = normalize(transpose(inverse(mat3(W))) * vertexPosition_modelspace);
  vec3 worldPos = (W * vec4(vertexPosition_modelspace, 1)).xyz;
  v_view = normalize(eyePos - worldPos);
  color = vertexColor;
}