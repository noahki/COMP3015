#version 460
layout (location = 0) in vec2 VertexPos;
layout (location = 1) in vec2 TexCoordsIN;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(VertexPos.x, VertexPos.y, 0.0, 1.0); 
    TexCoords = TexCoordsIN;
} 