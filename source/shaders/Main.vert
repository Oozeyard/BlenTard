#version 150
in vec4 vertex;
in vec3 normal;

out vec3 v_position;
out vec3 v_normal;

uniform mat4 mvp_matrix;
uniform mat3 normal_matrix;
void main() {
    v_position = vertex.xyz;
    v_normal = normal_matrix * normal;

    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vertex;
}