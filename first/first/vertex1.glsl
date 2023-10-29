#version 450 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexColor;
layout(location = 3) in vec3 vertexTangent;

uniform mat4 MVP;
uniform mat4 W;
uniform vec3 eyePos;
uniform vec3 lightDir;
uniform mat4 LMVP;

out vec3 v_normal, v_view, v_pos, v_light;
out vec2 v_texcoord;
out vec4 v_shadowCoord;

const mat4 tMat = mat4(
  0.5, 0.0, 0.0, 0.0,
  0.0, 0.5, 0.0, 0.0,
  0.0, 0.0, 0.5, 0.0,
  0.5, 0.5, 0.5, 1.0
);

void main(){
  gl_Position = MVP * W * vec4(vertexPosition_modelspace, 1);
  v_pos = (W * vec4(vertexPosition_modelspace, 1)).xyz;
  v_normal = normalize(transpose(inverse(mat3(W))) * vertexNormal);
  vec3 v_tangent = normalize(transpose(inverse(mat3(W))) * vertexTangent);
  vec3 v_bitanget = cross(v_normal, v_tangent);
  mat3 tbnMat = transpose(mat3(v_tangent, v_bitanget, v_normal));

  vec3 worldPos = (W * vec4(vertexPosition_modelspace, 1)).xyz;

  v_view = tbnMat * normalize(eyePos - worldPos);
  v_light = tbnMat * normalize(lightDir);
  v_texcoord = vertexColor;
  v_shadowCoord = tMat * LMVP * vec4(v_pos, 1.0);
}