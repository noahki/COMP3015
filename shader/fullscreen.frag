#version 460
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D blurredTexture;
uniform sampler2D screenTexture;

float radius = 400.0;

uniform int useBlur;

void main()
{
    vec4 color = texture(screenTexture, TexCoords);
    vec4 blurredColor = texture(blurredTexture, TexCoords);

    if (useBlur == 1) {
        FragColor = blurredColor;
        return;
    }

    float centeredX = 1280 / 2;
    float centeredY = 720 / 2;
    
    float distance = sqrt((gl_FragCoord.x - centeredX) * (gl_FragCoord.x - centeredX) + (gl_FragCoord.y - centeredY) * (gl_FragCoord.y - centeredY));

    if (distance <= radius) {
        FragColor = vec4(color); 
    } else {
        FragColor = vec4(blurredColor);
    }
    
}