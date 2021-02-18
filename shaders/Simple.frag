#version 450 core

// input
in VertexData {
    vec3 color;
} vertexData;

// output
out vec4 color;

void main()
{
    color = vec4(vertexData.color, 1.0);
}