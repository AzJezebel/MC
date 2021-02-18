#version 450 core

// input
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texel;

// output
out VertexData {
    vec2 texel;
} vertexData;

// uniform
uniform mat4 movement;

void main()
{
    gl_Position = movement * vec4(position, 1.0);

    vertexData.texel = texel;
}