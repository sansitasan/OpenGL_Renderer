#version 330 core

uniform vec3 matSpec, matAmbi, matEmit;
uniform float matSh;
uniform vec3 srcDiff, srcSpec, srcAmbi;
uniform vec3 lightDir;

in vec3 color;
in vec3 v_normal, v_view, v_pos;

out vec3 Color;

void main(){
  vec3 normal = normalize(v_normal);
  vec3 view = normalize(v_view);
  vec3 light = normalize(lightDir);

  vec3 diff = max(dot(normal, light), 0.0) * srcDiff * color;

  vec3 refl = 2 * dot(normal, light) * normal - light;
  vec3 spec = pow(max(dot(refl, view), 0.0), matSh) * srcSpec * matSpec;
  Color =  v_pos;//diff + spec;
}