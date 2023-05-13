#pragma once

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model {
private:
	float* vertexData;
	GLuint vboHandles[4];
	GLuint posBufferHandle;
	GLuint normalBufferHandle;
	GLuint texBufferHandle;
	GLuint indicesBufferHandle;

public:
	Model();
	~Model();
		
	int load_from_file(const char*);
	int create_from_vertices(float vertices[]);

	int load_texture(const char*);

	void render();

	int indexCount;

	glm::mat4 transformation;
	GLuint texture;
	bool hasTexture = false;
	
	GLuint vaoHandle;
};