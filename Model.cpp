#include "Model.h"

#include "OBJLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "helper/stb/stb_image.h"

Model::Model() {
	transformation = glm::mat4(1.0f);
}

Model::~Model() {
	delete vertexData;
}

void Model::render() {
	glBindTexture(GL_TEXTURE_2D, 0);

	if (hasTexture) {
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Model::create_from_vertices(float vertices[]) {
	assert(sizeof(vertices) > 0);
	int length = sizeof(vertices) / sizeof(vertices[0]);

	float colorData[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f };

	glGenBuffers(4, vboHandles);
	posBufferHandle = vboHandles[0];
	normalBufferHandle = vboHandles[1];
	texBufferHandle = vboHandles[2];
	indicesBufferHandle = vboHandles[3];

	glBindBuffer(GL_ARRAY_BUFFER, posBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

	 glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	 glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, length * 2 * sizeof(float), colorData, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexBuffer(0, posBufferHandle, 0, sizeof(GLfloat) * 3);
	glBindVertexBuffer(1, normalBufferHandle, 0, sizeof(GLfloat) * 3);
	glBindVertexBuffer(2, texBufferHandle, 0, sizeof(GLfloat) * 2);

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);

	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);

	glBindVertexArray(0);

	return 1;
}

int Model::load_from_file(const char* filename)
{
	std::vector<float> vertices = std::vector<float>();
	std::vector<float> textureCoords = std::vector<float>();
	std::vector<int> indices = std::vector<int>();
	std::vector<float> normals = std::vector<float>();

	objl::Loader loader;

	bool loaded = loader.LoadFile(filename);

	if (!loaded) {
		std::cout << "Couldnt load file" << std::endl;
		return -1;
	}

	if (loaded && loader.LoadedMeshes.size() > 0) {
		auto mesh = loader.LoadedMeshes[0];

		for (auto i = 0; i < mesh.Vertices.size(); i++) {
			vertices.push_back(mesh.Vertices[i].Position.X);
			vertices.push_back(mesh.Vertices[i].Position.Y);
			vertices.push_back(mesh.Vertices[i].Position.Z);

			textureCoords.push_back(mesh.Vertices[i].TextureCoordinate.X);
			textureCoords.push_back(mesh.Vertices[i].TextureCoordinate.Y);

			indices.push_back(mesh.Indices[i]);

			normals.push_back(mesh.Vertices[i].Normal.X);
			normals.push_back(mesh.Vertices[i].Normal.Y);
			normals.push_back(mesh.Vertices[i].Normal.Z);
		}
	}

	glGenBuffers(4, vboHandles);
	posBufferHandle = vboHandles[0];
	normalBufferHandle = vboHandles[1];
	texBufferHandle = vboHandles[2];
	indicesBufferHandle = vboHandles[3];

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glBindBuffer(GL_ARRAY_BUFFER, posBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

	indexCount = indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(float), &textureCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexBuffer(0, posBufferHandle, 0, sizeof(GLfloat) * 3);
	glBindVertexBuffer(1, normalBufferHandle, 0, sizeof(GLfloat) * 3);
	glBindVertexBuffer(2, texBufferHandle, 0, sizeof(GLfloat) * 2);

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);

	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);

	glBindVertexArray(0);

	return 1;
}

int Model::load_texture(const char* filename) {
	int width, height, composition;
	const uint8_t* image = stbi_load(filename, &width, &height, &composition, 3);

	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureStorage2D(texture, 1, GL_RGB8, width, height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB,  GL_UNSIGNED_BYTE, image);
	glBindTextures(0, 1, &texture);

	hasTexture = true;

	return true;
}