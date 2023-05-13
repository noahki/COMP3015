#version 460
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    vec2 texSize = textureSize(screenTexture, 0).xy;
    vec2 texCoord = gl_FragCoord.xy / texSize;

    int size = 2;

    float separation = 1.0;
    float count = 0.0;

    vec4 color = vec4(0.0);

    for (int i = -size; i <= size; ++i) {
        for (int j = -size; j <= size; ++j) {
            color += texture(screenTexture, (gl_FragCoord.xy + (vec2(i, j) * separation)) / texSize);

            count += 1.0;
        }
    }

    color /= count;

    FragColor = color;

}