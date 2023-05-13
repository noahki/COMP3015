#pragma once

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Framebuffer {
private:

public:
    Framebuffer();
    ~Framebuffer() {
        glDeleteFramebuffers(1, &fbo);
    };

    GLuint fbo;
    GLuint texture;
    GLuint depthTexture;
    GLuint rbo;

    void init();
    void bind();
    void unbind();
};