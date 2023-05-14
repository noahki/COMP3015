#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

class NoiseGen
{
public:
    NoiseGen() {};

    GLuint generate_noise_texture(unsigned int width = 256, unsigned int height = 256);
private:
};