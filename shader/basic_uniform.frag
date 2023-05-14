#version 460

layout(binding=0) uniform sampler2D Tex1;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 VertexPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 eyePos;
uniform int hasTexture;

void main() {
    vec3 color = vec3(0.0);
    if (hasTexture == 1) {
        color = texture(Tex1, TexCoords).rgb;
    } else {
        color = vec3(1.0, 1.0, 1.0);
    }
    vec3 ambient = 0.05 * color;

    vec3 lightDirection = normalize(lightPos - VertexPos);
    vec3 normal = normalize(Normal);

    float diff = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 viewDirection = normalize(eyePos - VertexPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    
    vec3 h = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(viewDirection, h), 0.0), 32.0);

    vec3 specular = vec3(0.1) * spec;

    FragColor = vec4(ambient + diffuse + specular, 1.0);

    // Luminance Magic Numbers
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}