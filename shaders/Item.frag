#version 450 core

// input
in VertexData {
    vec2 texel;
} vertexData;

// uniform
uniform sampler2D item;

// output
out vec4 color;

void main()
{
    color = texture(item, vertexData.texel);
}