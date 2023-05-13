#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "Model.h"

using glm::vec3;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f) {}

Model clouds = Model();
Model plane = Model();

unsigned int fbo;
unsigned int textureColorbuffer;

unsigned int quadVAO;
unsigned int quadVBO;
unsigned int rbo;

void SceneBasic_Uniform::initScene()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER);

    compile();

    std::cout << std::endl;

    prog.printActiveUniforms();

    clouds.load_from_file("./clouds.obj");
    clouds.transformation = glm::rotate(clouds.transformation, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    clouds.transformation = glm::rotate(clouds.transformation, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    clouds.transformation = glm::translate(clouds.transformation, glm::vec3(-2.7f, 2.5f, 0.0f));
    clouds.transformation = glm::scale(clouds.transformation, glm::vec3(0.2f));

    plane.load_from_file("./plane.obj");
    plane.load_texture("./GroundTexture.png");
    plane.transformation = glm::rotate(plane.transformation, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    cameraMatrix = glm::lookAt(
        glm::vec3(1.0f, 0.0f, -5.0f),
        glm::vec3(0.5f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );
    projMatrix = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 1.0f, 10.0f);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


    // Full screen quad to render framebuffer onto.
    float quadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    fullscreenShader.use();
    fullscreenShader.setUniform("screenTexture", 0);
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();

        fullscreenShader.compileShader("shader/fullscreen.vert");
        fullscreenShader.compileShader("shader/fullscreen.frag");
        fullscreenShader.link();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

glm::vec3 lightPos = glm::vec3(3.0f, 4.0f, -2.0f);

void SceneBasic_Uniform::update( float t )
{
    plane.transformation = glm::scale(plane.transformation, glm::vec3(1.0f));
}

void SceneBasic_Uniform::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //create the rotation matrix here and update the uniform in the shader
    prog.use();
    prog.setUniform("cameraMatrix", cameraMatrix);
    prog.setUniform("projMatrix", projMatrix);
    prog.setUniform("modelMatrix", clouds.transformation);

    prog.setUniform("lightPos", lightPos);
    prog.setUniform("eyePos", glm::vec3(0.0f));
    // prog.setUniform("Lights[0].Intensity", glm::vec3(0.5f));
    prog.setUniform("hasTexture", clouds.hasTexture ? 1 : 0);
    clouds.render();

    prog.setUniform("modelMatrix", plane.transformation);
    prog.setUniform("hasTexture", plane.hasTexture ? 1 : 0);
    plane.render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    fullscreenShader.use();
    glBindVertexArray(quadVAO);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.    
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}
