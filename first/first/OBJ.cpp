#define _CRT_SECURE_NO_WARNINGS
#include "OBJ.h"
#include <il.h>

OBJ::OBJ() {

}

void OBJ::Init(int size) {
	glCreateVertexArrays(1, &this->VertexArrayID);
	glBindVertexArray(this->VertexArrayID);

	for (int i = 0; i < size; ++i) {
		GLuint texture = -1;
		textureID.push_back(texture);
		glGenTextures(1, &this->textureID[i]);
		MapID.push_back(-1);
	}

	glGenBuffers(1, &this->vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &this->colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * 2 * sizeof(GLfloat), &this->uvs[0], GL_STATIC_DRAW);
	glGenBuffers(1, &this->normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, this->normals.size() * 3 * sizeof(GLfloat), &this->normals[0], GL_STATIC_DRAW);
	glGenBuffers(1, &this->tangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->tangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, this->tangents.size() * 3 * sizeof(GLfloat), &this->tangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->IA);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IA);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->idx.size() * sizeof(GLushort), &this->idx[0], GL_STATIC_DRAW);
}

OBJ::OBJ(const char* path, std::vector<const char*> tpath, OBJType type, float scale, glm::vec3 local) {
	this->mMatrix = glm::mat4(
		glm::vec4(scale, 0, 0, 0),
		glm::vec4(0, scale, 0, 0),
		glm::vec4(0, 0, scale, 0),
		glm::vec4(local.x, local.y, local.z, 1)
	);
	LoadOBJ(path);
	Init(tpath.size());
	LoadTexture(tpath);
}

bool OBJ::LoadOBJ(const char* path) {
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	std::vector<glm::vec2> _uv;
	std::vector<glm::vec3> _n;
	//int cnt = 0;
	//int vtxcnt = 0;
	//int uvcnt = 0;
	//int ncnt = 0;
	while (1) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		
		if (res == EOF)
			break;

		//if ((int)lineHeader[0] == (int)'o') {
		//	cnt += fscanf(file, "%s", lineHeader);
		//	continue;
		//}
		//
		//if (cnt > 1) {
		//	if (strcmp(lineHeader, "v") == 0)
		//		++vtxcnt;
		//	else if (strcmp(lineHeader, "vt") == 0)
		//		++uvcnt;
		//	else if (strcmp(lineHeader, "vn") == 0)
		//		++ncnt;
		//	continue;
		//}
		
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			this->vertices.push_back(vertex);
		}

		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			_uv.push_back(uv);
		}
		
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			_n.push_back(normal);
		}

		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", //%d/%d/%d\n", 
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				//&vertexIndex[3], &uvIndex[3], &normalIndex[3]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}

			idx.push_back(vertexIndex[0] - 1);
			idx.push_back(vertexIndex[1] - 1);
			idx.push_back(vertexIndex[2] - 1);
			//idx.push_back(vertexIndex[3] - 1 - vtxcnt);
			uvs.push_back(_uv[uvIndex[0] - 1]);
			uvs.push_back(_uv[uvIndex[1] - 1]);
			uvs.push_back(_uv[uvIndex[2] - 1]);
			//uvs.push_back(_uv[uvIndex[3] - 1 - uvcnt]);
			normals.push_back(_n[normalIndex[0] - 1]);
			normals.push_back(_n[normalIndex[1] - 1]);
			normals.push_back(_n[normalIndex[2] - 1]);
			//normals.push_back(_n[normalIndex[3] - 1 - ncnt]);
			glm::vec3 deltaPos1 = vertices[vertexIndex[1] - 1]
				- vertices[vertexIndex[0] - 1];
			glm::vec3 deltaPos2 = vertices[vertexIndex[2] - 1]
				- vertices[vertexIndex[0] - 1];
			glm::vec2 deltaUV1 = _uv[uvIndex[1] - 1]
				- _uv[uvIndex[0] - 1];
			glm::vec2 deltaUV2 = _uv[uvIndex[2] - 1]
				- _uv[uvIndex[0] - 1];

			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			tangents.push_back(tangent);
			tangents.push_back(tangent);
			tangents.push_back(tangent);
		}
	}

	return true;
}

void OBJ::LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {
	// 쉐이더들 생성
	VertShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//glShaderSource(VertexShaderID, 1, &vertex_file_path, NULL);

	// 버텍스 쉐이더 코드를 파일에서 읽기
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("파일 %s 를 읽을 수 없음. 정확한 디렉토리를 사용 중입니까 ? FAQ 를 우선 읽어보는 걸 잊지 마세요!\n", vertex_file_path);
		getchar();
	}

	// 프래그먼트 쉐이더 코드를 파일에서 읽기
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// 버텍스 쉐이더를 컴파일
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertShaderID);

	// 버텍스 쉐이더를 검사
	glGetShaderiv(VertShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// 프래그먼트 쉐이더를 컴파일
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragShaderID);

	// 프래그먼트 쉐이더를 검사
	glGetShaderiv(FragShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}
	this->programID.push_back(glCreateProgram());

	glAttachShader(this->programID[programID.size() - 1], VertShaderID);
	glAttachShader(this->programID[programID.size() - 1], FragShaderID);
	glLinkProgram(this->programID[programID.size() - 1]);

	// 프로그램 검사
	glGetProgramiv(this->programID[programID.size() - 1], GL_LINK_STATUS, &Result);
	glGetProgramiv(this->programID[programID.size() - 1], GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(this->programID[programID.size() - 1], InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(this->programID[programID.size() - 1], VertShaderID);
	glDetachShader(this->programID[programID.size() - 1], FragShaderID);

	glDeleteShader(VertShaderID);
	glDeleteShader(FragShaderID);
	this->MatrixID = glGetUniformLocation(this->programID[programID.size() - 1], "MVP");
	this->WorldMatrixID = glGetUniformLocation(this->programID[programID.size() - 1], "W");
	this->EyePosID = glGetUniformLocation(this->programID[programID.size() - 1], "eyePos");
	this->matSpecID = glGetUniformLocation(this->programID[programID.size() - 1], "matSpec");
	this->matAmbiID = glGetUniformLocation(this->programID[programID.size() - 1], "matAmbi");
	this->matEmitID = glGetUniformLocation(this->programID[programID.size() - 1], "matEmit");
	this->srcDiffID = glGetUniformLocation(this->programID[programID.size() - 1], "srcDiff");
	this->srcSpecID = glGetUniformLocation(this->programID[programID.size() - 1], "srcSpec");
	this->srcAmbiID = glGetUniformLocation(this->programID[programID.size() - 1], "srcAmbi");
	this->matShID = glGetUniformLocation(this->programID[programID.size() - 1], "matSh");
	this->lightDirID = glGetUniformLocation(this->programID[programID.size() - 1], "lightDir");
	this->MapID[0] = glGetUniformLocation(this->programID[programID.size() - 1], "colorMap");
	if (MapID.size() > 1)
		this->MapID[1] = glGetUniformLocation(this->programID[programID.size() - 1], "normalMap");
	this->pos_attr = glGetAttribLocation(this->programID[programID.size() - 1], "vertexPosition_modelspace");
	this->nor_attr = glGetAttribLocation(this->programID[programID.size() - 1], "vertexNormal");
	this->uv_attr = glGetAttribLocation(this->programID[programID.size() - 1], "vertexColor");
	this->tan_attr = glGetAttribLocation(this->programID[programID.size() - 1], "vertexTangent");
	this->LMVPID = glGetUniformLocation(this->programID[programID.size() - 1], "LMVP");
	this->ShadowMapID = glGetUniformLocation(this->programID[programID.size() - 1], "shadowMap");
}

void OBJ::LoadShadowShaders(const char* vertex_file_path, const char* fragment_file_path) {
	// 쉐이더들 생성
	Shadow = new ShadowShader();
	Shadow->ShadowVertID = glCreateShader(GL_VERTEX_SHADER);
	Shadow->ShadowFragID = glCreateShader(GL_FRAGMENT_SHADER);

	//glShaderSource(VertexShaderID, 1, &vertex_file_path, NULL);

	// 버텍스 쉐이더 코드를 파일에서 읽기
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("파일 %s 를 읽을 수 없음. 정확한 디렉토리를 사용 중입니까 ? FAQ 를 우선 읽어보는 걸 잊지 마세요!\n", vertex_file_path);
		getchar();
	}

	// 프래그먼트 쉐이더 코드를 파일에서 읽기
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// 버텍스 쉐이더를 컴파일
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(Shadow->ShadowVertID, 1, &VertexSourcePointer, NULL);
	glCompileShader(Shadow->ShadowVertID);

	// 버텍스 쉐이더를 검사
	glGetShaderiv(Shadow->ShadowVertID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(Shadow->ShadowVertID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(Shadow->ShadowVertID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// 프래그먼트 쉐이더를 컴파일
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(Shadow->ShadowFragID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(Shadow->ShadowFragID);

	// 프래그먼트 쉐이더를 검사
	glGetShaderiv(Shadow->ShadowFragID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(Shadow->ShadowFragID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(Shadow->ShadowFragID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	printf("Linking program\n");
	this->programID.push_back(glCreateProgram());

	glAttachShader(this->programID[0], Shadow->ShadowVertID);
	glAttachShader(this->programID[0], Shadow->ShadowFragID);
	glLinkProgram(this->programID[0]);

	// 프로그램 검사
	glGetProgramiv(this->programID[0], GL_LINK_STATUS, &Result);
	glGetProgramiv(this->programID[0], GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(this->programID[0], InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(this->programID[0], Shadow->ShadowVertID);
	glDetachShader(this->programID[0], Shadow->ShadowFragID);
	Shadow->mMatrixID = glGetUniformLocation(this->programID[0], "M");
	Shadow->VPMatrixID = glGetUniformLocation(this->programID[0], "VP");
	Shadow->PosAttrID = glGetAttribLocation(programID[0], "vertexPosition_modelspace");
}

void OBJ::LoadTexture(std::vector<const char*> path) {
	for (int i = 0; i < path.size(); ++i) {
		ILuint img;
		ilInit();
		ilGenImages(1, &img);

		if (!ilLoadImage((const wchar_t*)path[i])) {
			ilDeleteImages(1, &img);
			printf("Can`t load Img\n");
			return;
		}
		tex.width = ilGetInteger(IL_IMAGE_WIDTH);
		tex.height = ilGetInteger(IL_IMAGE_HEIGHT);
		tex.texels = new char[tex.width * tex.height * 4];
		ilCopyPixels(0, 0, 0, tex.width, tex.height, 1, IL_RGBA, IL_UNSIGNED_BYTE, tex.texels);
		ilDeleteImages(1, &img);

		glBindTexture(GL_TEXTURE_2D, textureID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, tex.texels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
}

void OBJ::Draw(const glm::vec3& eye, const glm::mat4& mvp, const Light& light) {
	glm::mat4 depthProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10.0f);
	glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(light.position), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthVP = depthProjectionMatrix * depthViewMatrix;
	//어떤 쉐이더를 렌더링할지 = 쉐이더 활성화
	//glUseProgram(this->programID[0]);
	//
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
		1024, 1024, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	
	//GLuint fbo;
	//glGenFramebuffers(1, &fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	return;
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glViewport(0, 0, 640, 480);
	//glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	//glUniformMatrix4fv(Shadow->mMatrixID, 1, GL_FALSE, &depthModelMatrix[0][0]);
	//glUniformMatrix4fv(Shadow->VPMatrixID, 1, GL_FALSE, &depthVP[0][0]);
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
	//glVertexAttribPointer(
	//	0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glDrawElements(GL_TRIANGLES, this->idx.size(), GL_UNSIGNED_SHORT, (GLvoid*)0);
	//glDisableVertexAttribArray(0);
	

	glUseProgram(this->programID[programID.size() - 1]);
	//바인딩된 쉐이더에게 변환된 매트릭스를 넘김
	glm::mat4 LMVP = depthVP * depthModelMatrix;
	glUniformMatrix4fv(this->LMVPID, 1, GL_FALSE, &LMVP[0][0]);
	glUniformMatrix4fv(this->MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(this->WorldMatrixID, 1, GL_FALSE, &this->mMatrix[0][0]);
	
	glUniform3fv(lightDirID, 1, &light.direction[0]);
	glUniform3fv(srcDiffID, 1, &light.color[0]);
	glUniform3fv(srcSpecID, 1, &light.color[0]);
	glUniform3fv(EyePosID, 1, &eye[0]);
	glUniform3fv(matSpecID, 1, &glm::vec3(0.5f, 0.5f, 0.5f)[0]);
	glUniform1f(matShID, 2.0f);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->normalbuffer);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->colorbuffer);
	glVertexAttribPointer(
		2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->tangentbuffer);
	glVertexAttribPointer(
		3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glUniform1i(MapID[0], 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID[0]);
	if (MapID.size() > 1) {
		glUniform1i(MapID[1], 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID[1]);
	}
	glUniform1i(ShadowMapID, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IA);
	glDrawElements(GL_TRIANGLES, this->idx.size(), GL_UNSIGNED_SHORT, (GLvoid*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}