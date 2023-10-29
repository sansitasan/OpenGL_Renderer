#pragma once
#include "MyOpenGL.h"
struct Light {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec4 direction;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
};

Light SetLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec4 position);
void SetLightDir(GLFWwindow* window, Light& light);