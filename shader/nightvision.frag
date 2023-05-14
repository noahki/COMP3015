#version 460
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D sceneTexture;
uniform sampler2D noiseTexture;

float luminance(vec3 color)
{
    return dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
}

void main()
{
    vec3 sceneColor = texture(sceneTexture, TexCoords).rgb;
    vec4 noise = texture(noiseTexture, TexCoords);

    float brightness = luminance(sceneColor);

    vec4 nightvision = vec4(0.0, brightness * clamp(noise.a + 0.25, 0.0, 1.0), 0.0, 1.0);

    // FragColor = vec4(sceneColor, 1.0); 
    FragColor = nightvision;
    // FragColor = vec4(noise.a);
}