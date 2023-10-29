#include "control.h"

float initialFoV = 45.0f;

float speed = 3; // 초당 3

float lasttime = 0;
float currenttime = 0;
float deltaTime = 0;

glm::mat4 ControlOBJ(GLFWwindow* window, glm::vec3& eye) {

	currenttime = glfwGetTime();
	deltaTime = currenttime - lasttime;
	lasttime = currenttime;
	float y = 0, x = 0;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
		x += deltaTime * speed;
	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
		x -= deltaTime * speed;
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) 
		y += deltaTime * speed;
	
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
		y -= deltaTime * speed;
		
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::quat q1(cos(x / 2), sin(x / 2) , 0, 0), q2(cos(y / 2), 0, sin(y / 2), 0);
	
	eye = q2 * q1 * eye;

	glm::mat4 ViewMatrix = glm::lookAt(
		eye, //카메라의 위치
		glm::vec3(0, 0, 0), //바라볼 물체 위치
		glm::vec3(0, 1, 0) //카메라의 up벡터
	);
	//ViewMatrix = ViewMatrix * t;
	glm::mat4 Model(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1)
	);

	return ProjectionMatrix * ViewMatrix * Model;
}