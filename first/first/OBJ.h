#pragma once
#include "MyOpenGL.h"
#include "light.h"

enum OBJType {
	VertexArray,
	IndexArray
};

struct TexData {
	char* texels;
	GLsizei width;
	GLsizei height;
};

struct ShadowShader {
	GLuint ShadowVertID = -1;
	GLuint ShadowFragID = -1;
	GLuint mMatrixID = -1;
	GLuint VPMatrixID = -1;
	GLuint PosAttrID = -1;
};

struct Shader {

};

class OBJ {
private:
	bool LoadOBJ(const char* path);
	void LoadTexture(std::vector<const char*> path);
	ShadowShader* Shadow = NULL;
	GLuint VertexArrayID;
	GLuint vertexbuffer;
	GLuint IA;
	GLuint colorbuffer;
	GLuint normalbuffer;
	GLuint tangentbuffer;

	std::vector<GLuint> programID;

	GLuint MatrixID;
	GLuint WorldMatrixID;
	GLuint EyePosID;
	GLuint matSpecID;
	GLuint matAmbiID;
	GLuint matEmitID;
	GLuint srcDiffID;
	GLuint srcSpecID;
	GLuint srcAmbiID;
	GLuint matShID;
	GLuint lightDirID;
	GLuint VertShaderID;
	GLuint FragShaderID;
	GLuint LMVPID;
	GLuint ShadowMapID;

	std::vector<GLuint> textureID;
	std::vector<GLuint> MapID;

	GLint pos_attr;
	GLint nor_attr;
	GLint uv_attr;
	GLint tan_attr;

	glm::mat4 mvp;
	glm::mat4 mMatrix;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;
	std::vector<GLushort> idx;
	TexData tex;
public:
	OBJ();
	void Init(int );
	OBJ(const char* path, std::vector<const char*> tpath, OBJType type, float, glm::vec3);
	void LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
	void LoadShadowShaders(const char* vertex_file_path, const char* fragment_file_path);
	void Draw(const glm::vec3&, const glm::mat4& mvp, const Light&);
};
