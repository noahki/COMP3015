#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Framebuffer.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLuint vaoHandle;
    GLSLProgram prog;
    GLSLProgram fullscreenShader;
    GLSLProgram blurShader;
    GLSLProgram bloomShader;
    GLSLProgram nightvisionShader;
    GLSLProgram terrainShader;
    float angle;

    void compile();

    glm::mat4 modelMatrix;
    glm::mat4 projMatrix;

    Camera camera;

    double old_x_pos = -1.0;
    double old_y_pos = -1.0;

    float exposure = 1.0f;

    bool mouse_down = false;

    int max_scenes = 4;
    int current_scene = 0;

    Framebuffer framebuffer = Framebuffer();
    Framebuffer blur_framebuffer = Framebuffer();
    Framebuffer bloom_framebuffer = Framebuffer();

    GLuint terrainTexture;

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);

    void draw_scene();
    void draw_terrain();
    void draw_tree();

    void bloom();
    void nightvision(bool useBloomFramebuffer);

    void handle_key_events(GLFWwindow* window);
    void handle_mouse_events(GLFWwindow* window);
};

#endif // SCENEBASIC_UNIFORM_H
