#version 460
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform int blurStrength;

void main()
{ 
    vec2 texSize = textureSize(screenTexture, 0).xy;
    vec2 texCoord = gl_FragCoord.xy / texSize;

    float separation = 1.0;
    float count = 0.0;

    vec4 color = vec4(0.0);

    for (int i = -blurStrength; i <= blurStrength; ++i) {
        for (int j = -blurStrength; j <= blurStrength; ++j) {
            color += texture(screenTexture, (gl_FragCoord.xy + (vec2(i, j) * separation)) / texSize);

            count += 1.0;
        }
    }

    color /= count;

    FragColor = color;

}