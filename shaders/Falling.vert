#version 450 core

// input
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texel;
layout (location = 2) in vec3 normal;

// output
out VertexData {
    vec2 texel;
    vec3 normal;
} vertexData;

// uniform
layout (std140) uniform Matrice {
    uniform mat4 viewProjection;
} matrice;

void main()
{
    gl_Position = matrice.viewProjection * vec4(position, 1.0);

    vertexData.texel = texel;
    vertexData.normal = normal;
}
