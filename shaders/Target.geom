#version 450 core

	// input
layout (points) in;



	// output
layout (triangle_strip, max_vertices = 84) out;
out VertexData {
    vec3 color;
} vertexData;



	// instance variable
vec4 positionCenter;



	// uniform
layout (std140) uniform Matrice {
    uniform mat4 viewProjection;
} matrice;
uniform float x;
uniform float y;
uniform float z;
uniform int face;



    // constant
const float bottom	= -0.501953125;
const float top		=  0.501953125;
const float back	= -0.501953125;
const float front	=  0.501953125;
const float left	= -0.501953125;
const float right	=  0.501953125;

const int F_100 =  1;
const int F_010 =  2;
const int F_001 =  4;
const int B_100 = -1;
const int B_010 = -2;
const int B_001 = -4;
const int INSIDE = 0;



	// function forward declaration
void Back();
void Bottom();
void Front();
void Left();
void Right();
void Top();

void TargetBack();
void TargetBottom();
void TargetFront();
void TargetLeft();
void TargetRight();
void TargetTop();



void main()
{
	positionCenter = vec4(x, y, z, 1.0);
    vertexData.color = vec3(1.0, 0.0, 0.0);

	Left();
	Right();
	
	Bottom();
	Top();
	
	Back();
	Front();
    
    vertexData.color = vec3(0.0, 0.0, 1.0);
    switch (face)
    {
    case F_100:
        TargetRight();
        break;
    case F_010:
        TargetTop();
        break;
    case F_001:
        TargetFront();
        break;
    case B_100:
        TargetLeft();
        break;
    case B_010:
        TargetBottom();
        break;
    case B_001:
        TargetBack();
        break;
    }
}



void Back()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, -0.25, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, bottom, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, bottom, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, bottom, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, -0.25, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.25, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, top, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, top, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, top, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.25, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, top, back, 0.0));
    EmitVertex();
    EndPrimitive();
}



void Bottom()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, bottom, -0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, bottom, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, bottom, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, bottom, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, -0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, 0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, bottom, front, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, bottom, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, bottom, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, bottom, 0.25, 0.0));
    EmitVertex();
    EndPrimitive();
}



void Front()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, -0.25, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, bottom, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, bottom, front, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, bottom, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, -0.25, front, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.25, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, top, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, top, front, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, top, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, top, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.25, front, 0.0));
    EmitVertex();
    EndPrimitive();
}



void Left()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, bottom, -0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, -0.25, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, bottom, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.25, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, top, -0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, top, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, top, 0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.25, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, top, front, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, -0.25, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, bottom, 0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, bottom, front, 0.0));
    EmitVertex();
    EndPrimitive();
}



void Right()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, -0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, -0.25, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.25, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, top, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, top, -0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, top, 0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, top, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.25, front, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, -0.25, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, 0.25, 0.0));
    EmitVertex();
    EndPrimitive();
}



void Top()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, top, -0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, top, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, top, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, top, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, top, -0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, top, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, top, 0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, top, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, top, front, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, top, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, top, 0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, top, front, 0.0));
    EmitVertex();
    EndPrimitive();
}



void TargetBack()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, 0.0, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, -0.25, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, -0.25, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, -0.25, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, 0.0, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, -0.25, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, 0.0, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, 0.25, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, 0.25, back, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, 0.25, back, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, 0.0, back, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, 0.25, back, 0.0));
    EmitVertex();
    EndPrimitive();
}



void TargetBottom()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, bottom, 0.0, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, bottom, -0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, bottom, -0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, bottom, -0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, bottom, -0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, bottom, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, bottom, 0.0, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, bottom, 0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, bottom, 0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, bottom, 0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, bottom, 0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, bottom, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();
}



void TargetFront()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, 0.0, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, -0.25, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, -0.25, front, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, -0.25, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, -0.25, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, 0.0, front, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, 0.0, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, 0.25, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, 0.25, front, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, 0.25, front, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, 0.25, front, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, 0.0, front, 0.0));
    EmitVertex();
    EndPrimitive();
}



void TargetLeft()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, -0.25, 0.0, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.0, -0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, -0.25, -0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.0, -0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.25, 0.0, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.25, -0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.25, 0.0, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.0, 0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.25, 0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, 0.0, 0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(left, -0.25, 0.0, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(left, -0.25, 0.25, 0.0));
    EmitVertex();
    EndPrimitive();
}



void TargetRight()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, -0.25, 0.0, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, -0.25, -0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.0, -0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.0, -0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.25, -0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.25, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.25, 0.0, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.25, 0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.0, 0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, 0.0, 0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, -0.25, 0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, -0.25, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();
}



void TargetTop()
{
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, top, 0.0, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, top, -0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, top, -0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, top, -0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, top, 0.0, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, top, -0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, top, 0.0, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, top, 0.25, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(0.25, top, 0.25, 0.0));
    EmitVertex();
    EndPrimitive();
    
	gl_Position = matrice.viewProjection * (positionCenter + vec4(0.0, top, 0.25, 0.0));
    EmitVertex();
	gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, top, 0.0, 0.0));
    EmitVertex();
    gl_Position = matrice.viewProjection * (positionCenter + vec4(-0.25, top, 0.25, 0.0));
    EmitVertex();
    EndPrimitive();
}
