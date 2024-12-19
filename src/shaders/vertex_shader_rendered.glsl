#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Shadow mapping
uniform mat4 lightSpaceMatrix;

void main() {
    vec4 worldPosition = model * vec4(aPosition, 1.0);
    FragPos = worldPosition.xyz;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    // Calculate fragment position in light space
    FragPosLightSpace = lightSpaceMatrix * worldPosition;

    gl_Position = projection * view * worldPosition;
}
