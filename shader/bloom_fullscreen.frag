#version 460

layout (location=0) out vec4 FragColor;
layout (location=1) out vec4 _ColorAttachment2;
  
in vec2 TexCoords;

uniform sampler2D sceneTexture;
uniform sampler2D brightTexture;

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
    vec3 sceneColor = texture(sceneTexture, TexCoords).rgb;
    vec3 brightColor = texture(brightTexture, TexCoords).rgb;

    sceneColor += brightColor;

    FragColor = mapExposure(vec4(sceneColor, 1.0)); 
    _ColorAttachment2 = vec4(0.0);
}