#include "NoiseGen.h"

GLuint NoiseGen::generate_noise_texture(unsigned int width, unsigned int height)
{
	GLubyte* textureData = new GLubyte[width * height * 4];

	float xFactor = 1.0f / (width - 1);
	float yFactor = 1.0f / (height - 1);

	for (unsigned int row = 0; row < height; row++) {
		for (unsigned int col = 0; col < width; col++) {
			float x = xFactor * col;
			float y = yFactor * row;

			float initialFrequency = 50.0f;
			float initialScale = 0.5f;

			float sum = 0.0f;
			float frequency = initialFrequency;
			float scale = initialScale;

			for (int oct = 0; oct < 4; oct++) {
				glm::vec2 p(x * frequency, y * frequency);
				float noiseVal = glm::perlin(p) / scale;
				sum += noiseVal;

				float result = (sum + 1.0f) / 2.0f;

				textureData[((row * width + col) * 4) + oct] = (GLubyte)(result * 255.0f);
				frequency *= 2.0f;
				scale *= initialScale;
			}
		}
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	delete[] textureData;

	return textureId;
}