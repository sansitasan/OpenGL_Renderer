#include "control.h"
#include "MyOpenGL.h"
#include "OBJ.h"
#include "light.h"

//정점 배열
GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   0.0f,  0.0f, -1.0f,
};
//컬러 배열
GLfloat g_color_buffer_data[] = {
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f
};

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewInit();

	const char* s = "object/Cube.obj";
	std::vector<const char*> x;
	x.push_back("Texture/tree_default_Albedo.png");
	x.push_back("Texture/tree_default_Normal.png");

	OBJ* obj = new OBJ(s, x, IndexArray, 1, glm::vec3(0, 0, 0));

	obj->LoadShaders("vertex1.glsl", "fragment1.glsl");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glm::vec3 eyePos(0, 0, 10);
	Light dirlight = SetLight(glm::vec4(0, 0, 0, 1), glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1), glm::vec4(0, 0, 0, 0));

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0, 0.5f, 0.5f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 MVP = ControlOBJ(window, eyePos);

		SetLightDir(window, dirlight);
		obj->Draw(eyePos, MVP, dirlight);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}