#version 330 core

precision mediump float;
precision mediump sampler2DShadow;

uniform sampler2DShadow shadowMap;
uniform sampler2D colorMap, normalMap;
uniform vec3 matSpec, matAmbi, matEmit;
uniform float matSh;
uniform vec3 srcDiff, srcSpec, srcAmbi;
//uniform vec3 lightDir;

in vec3 v_normal, v_view, v_pos, v_light;
in vec2 v_texcoord;
in vec4 v_shadowCoord;

out vec4 color;

const float offset = 0.005;

void main(){
  vec4 x = texture(colorMap, v_texcoord).rgba;
  //if(x.a < 0.5)
	//discard;
  vec3 normal = normalize(2 * texture(normalMap, v_texcoord).xyz - 1);
  vec3 view = normalize(v_view);
  vec3 light = normalize(v_light);
  vec3 diff = (max(dot(normal, light), 0.0) + 0.5) * srcDiff * x.rgb;

  vec3 refl = 2 * dot(normal, light) * normal - light;
  vec3 spec = pow(max(dot(refl, view), 0.0), matSh) * srcSpec * matSpec;

  //vec3 ambi = srcAmbi * matAmbi;
  vec4 offsetVec = vec4(0.0, 0.0, offset * v_shadowCoord.w, 0.0);
  float visibility = textureProj(shadowMap, v_shadowCoord - offsetVec);
  
  color = vec4(visibility * (diff + spec), 1.0);
  //color = vec4(1, 1, 1, 1);
}