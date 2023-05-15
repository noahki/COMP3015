#version 460
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float exposure;

// https://learnopengl.com/Advanced-Lighting/HDR
vec4 mapExposure(vec4 hdrColor) {
    const float gamma = 1.2;

    vec3 mapped = vec3(1.0) - exp(-hdrColor.xyz * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));

    return vec4(mapped, 1.0);
}

void main()
{
    vec4 color = texture(screenTexture, TexCoords).rgba;
    FragColor = color; 
}