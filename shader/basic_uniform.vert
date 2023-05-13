#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 TextureCoords;

out vec3 VertexPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;
uniform mat4 projMatrix;

void main() {
    VertexPos = (modelMatrix * vec4(VertexPosition, 1.0)).xyz;
    Normal = VertexNormal;
    TexCoords = TextureCoords;

    gl_Position = projMatrix * cameraMatrix * modelMatrix * vec4(VertexPosition, 1.0);
}