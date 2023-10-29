#include "light.h"

Light SetLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec4 position) {
	Light light;
	light.ambient = ambient;
	light.diffuse = diffuse;
	light.specular = specular;
	light.position = position;
	light.color = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	light.direction = glm::vec4(1, 1, 1, 0);

	return light;
}

void SetLightDir(GLFWwindow* window, Light& light) {
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		light.direction += 0.005f * glm::vec4(0, 1, 0, 0);

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		light.direction -= 0.005f * glm::vec4(0, 1, 0, 0);

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		light.direction += 0.005f * glm::vec4(1, 0, 0, 0);

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		light.direction -= 0.005f * glm::vec4(1, 0, 0, 0);
}