#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "Camera.h"
#include "Model.h"
#include "NoiseGen.h"
#include <vector>

using glm::vec3;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f) {}

std::vector<Model> clouds = std::vector<Model>();

Model plane = Model();
Model tree = Model();

unsigned int quadVAO;
unsigned int quadVBO;
unsigned int quadEBO;

bool useBlurShader = false;

NoiseGen noiseGen = NoiseGen();

GLuint noiseTexture;

void SceneBasic_Uniform::initScene()
{
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_FRAMEBUFFER);
    glEnable(GL_FRAMEBUFFER_SRGB);

    compile();

    std::cout << std::endl;

    prog.printActiveUniforms();

    for (int i = 0; i < 10; i++) {
        Model cloud;
        cloud.load_from_file("./clouds.obj");
        cloud.transformation = glm::translate(cloud.transformation, glm::vec3(-5.0f + rand() % 10, 2.0f + rand() % 2, (float)-i * 2));
        cloud.transformation = glm::scale(cloud.transformation, glm::vec3(0.2f));

        clouds.push_back(cloud);
    }

    plane.load_from_file("./plane.obj");
    plane.load_texture("./GroundTexture.png");

    tree.load_from_file("./tree.obj");
    tree.load_texture("./TreeTexture.png");
    tree.transformation = glm::scale(tree.transformation, glm::vec3(0.07f));

    camera = Camera();
    projMatrix = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 1.0f, 100.0f);

    framebuffer.init();
    blur_framebuffer.init();
    bloom_framebuffer.init();

    GLfloat quadVertices[] = {
        // Positions   // Texture coordinates
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    GLuint quadIndices[] = { 0, 1, 2, 0, 2, 3 };

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glGenBuffers(1, &quadEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    fullscreenShader.use();
    fullscreenShader.setUniform("screenTexture", 0);
    prog.use();

    noiseTexture = noiseGen.generate_noise_texture(256, 256);
    terrainTexture = noiseGen.generate_noise_texture(64, 64);
}

void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
        prog.link();

        fullscreenShader.compileShader("shader/fullscreen.vert");
        fullscreenShader.compileShader("shader/fullscreen.frag");
        fullscreenShader.link();

        blurShader.compileShader("shader/blur.vert");
        blurShader.compileShader("shader/blur.frag");
        blurShader.link();

        bloomShader.compileShader("shader/bloom_fullscreen.vert");
        bloomShader.compileShader("shader/bloom_fullscreen.frag");
        bloomShader.link();

        nightvisionShader.compileShader("shader/nightvision.vert");
        nightvisionShader.compileShader("shader/nightvision.frag");
        nightvisionShader.link();

        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

glm::vec3 lightPos = glm::vec3(3.0f, 5.0f, 1.0f);

float ticks = 0.0f;
void SceneBasic_Uniform::update(float t)
{
    plane.transformation = glm::scale(plane.transformation, glm::vec3(1.0f));

    for (int i = 0; i < clouds.size(); i++) {
        Model* cloud = &clouds[i];
        cloud->transformation = glm::translate(cloud->transformation, glm::vec3(0.001f, 0.0f, 0.0f));
    }
    ticks += 0.1f;
}

void SceneBasic_Uniform::draw_scene()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    prog.use();
    prog.setUniform("cameraMatrix", camera.get_camera_transformation());
    prog.setUniform("lightPos", lightPos);
    prog.setUniform("eyePos", glm::vec3(0.0f));
    prog.setUniform("Lights[0].Intensity", glm::vec3(0.1f));
    
    prog.setUniform("modelMatrix", plane.transformation);
    prog.setUniform("hasTexture", plane.hasTexture ? 1 : 0);

    prog.setUniform("projMatrix", projMatrix);

    for (int i = 0; i < clouds.size(); i++) {
        Model* cloud = &clouds[i];
        prog.setUniform("modelMatrix", cloud->transformation);
        prog.setUniform("hasTexture", cloud->hasTexture ? 1 : 0);
        cloud->render();
    }
}

void SceneBasic_Uniform::draw_tree()
{
    prog.use();
    prog.setUniform("cameraMatrix", camera.get_camera_transformation());
    prog.setUniform("lightPos", lightPos);
    prog.setUniform("eyePos", glm::vec3(0.0f));
    prog.setUniform("Lights[0].Intensity", glm::vec3(0.5f));

    prog.setUniform("modelMatrix", tree.transformation);
    prog.setUniform("hasTexture", tree.hasTexture ? 1 : 0);

    prog.setUniform("projMatrix", projMatrix);
    tree.render();
}

void SceneBasic_Uniform::draw_terrain()
{
    prog.use();
    prog.setUniform("cameraMatrix", camera.get_camera_transformation());
    prog.setUniform("lightPos", lightPos);
    prog.setUniform("eyePos", glm::vec3(0.0f));
    prog.setUniform("Lights[0].Intensity", glm::vec3(0.5f));

    prog.setUniform("modelMatrix", plane.transformation);
    prog.setUniform("hasTexture", plane.hasTexture ? 1 : 0);

    prog.setUniform("projMatrix", projMatrix);

    plane.render();
}

void SceneBasic_Uniform::render()
{
    // Draw scene to framebuffer.
    framebuffer.bind();
    draw_scene();
    draw_tree();
    draw_terrain();
    framebuffer.unbind();
    
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    
    if (current_scene == 0)
        bloom();
    else if (current_scene == 1)
        nightvision(false);
    else if (current_scene == 2) {
        bloom();
        nightvision(true);
    }
    else if (current_scene == 3) {
        plane.texture = framebuffer.textures[0];
        bloom();
    }
}

void SceneBasic_Uniform::bloom()
{
    blur_framebuffer.bind();
    blurShader.use();
    blurShader.setUniform("screenTexture", 0);
    blurShader.setUniform("exposure", exposure);
    blurShader.setUniform("blurStrength", 5);

    glBindTexture(GL_TEXTURE_2D, framebuffer.textures[1]);
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    blur_framebuffer.unbind();
    bloom_framebuffer.bind();
    bloomShader.use();
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebuffer.textures[0]);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, blur_framebuffer.textures[0]);

    bloomShader.setUniform("sceneTexture", 0);
    bloomShader.setUniform("brightTexture", 1);
    bloomShader.setUniform("exposure", exposure);
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    bloom_framebuffer.unbind();

    fullscreenShader.use();
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bloom_framebuffer.textures[0]);

    fullscreenShader.setUniform("screenTexture", 0);
    fullscreenShader.setUniform("exposure", exposure);
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SceneBasic_Uniform::nightvision(bool useBloomFramebuffer)
{
    nightvisionShader.use();
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    if (useBloomFramebuffer) {
        glBindTexture(GL_TEXTURE_2D, bloom_framebuffer.textures[0]);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, framebuffer.textures[0]);
    }

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);

    nightvisionShader.setUniform("sceneTexture", 0);
    nightvisionShader.setUniform("noiseTexture", 1);

    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

void SceneBasic_Uniform::handle_key_events(GLFWwindow* window) {
    float camera_speed = 0.05f; // TODO: This speed will vary depending on the speed of the PC.
    if (glfwGetKey(window, GLFW_KEY_W))
        camera.camera_pos += camera_speed * camera.camera_front;
    if (glfwGetKey(window, GLFW_KEY_S))
        camera.camera_pos -= camera_speed * camera.camera_front;
    if (glfwGetKey(window, GLFW_KEY_D))
        camera.camera_pos += glm::normalize(glm::cross(camera.camera_front, camera.camera_up)) * camera_speed;
    if (glfwGetKey(window, GLFW_KEY_A))
        camera.camera_pos -= glm::normalize(glm::cross(camera.camera_front, camera.camera_up)) * camera_speed;

    if (glfwGetKey(window, GLFW_KEY_SPACE))
        camera.camera_pos.y += camera_speed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        camera.camera_pos.y -= camera_speed;

    float exposure_delta = 0.02f;
    if (glfwGetKey(window, GLFW_KEY_P))
        exposure += exposure_delta;
    if (glfwGetKey(window, GLFW_KEY_O))
        exposure -= exposure_delta;
}

void SceneBasic_Uniform::handle_mouse_events(GLFWwindow* window) {
    double x_pos;
    double y_pos;

    glfwGetCursorPos(window, &x_pos, &y_pos);

    float x_diff = 0.0f;
    float y_diff = 0.0f;

    if (old_x_pos != -1.0 && old_y_pos != -1.0) {
        x_diff = x_pos - old_x_pos;
        y_diff = old_y_pos - y_pos;
    }

    old_x_pos = x_pos;
    old_y_pos = y_pos;

    camera.apply_mouse_movements(x_diff, y_diff);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !mouse_down) {
        mouse_down = true;
        current_scene++;
        if (current_scene >= max_scenes)
            current_scene = 0;
        std::cout << "Current Scene: " << current_scene << std::endl;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
        mouse_down = false;
}
