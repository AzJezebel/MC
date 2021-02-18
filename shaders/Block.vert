#version 450 core

// output
out VertexShader {
    int index;
} vertexShader;

void main()
{
	vertexShader.index = gl_VertexID;
}
