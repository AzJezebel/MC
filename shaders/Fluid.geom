#version 450 core

	// input
layout (points) in;
in VertexShader {
    int index;
} vertexShader[];



	// output
layout (triangle_strip, max_vertices = 24) out;
out VertexData {
    vec2 texel;
	vec3 normal;// For fragment shader
} vertexData;



	// uniform
uniform int chunkWidth;
uniform int chunkHeight;
uniform int chunkDepth;

layout (std140) uniform Matrice {
    uniform mat4 viewProjection;
} matrice;
uniform int highestBlock;
uniform int ticks;



	// storage buffer
layout(std430, binding = 0) buffer chunk
{
    ivec2 chunkPosition;
    int chunkData[65536];
};



	// constante
const int NO_BLOCK = -1;
const int AIR = 0;
const int SAND = 1;
const int GRAVEL = 2;
const int DIRT = 3;
const int GRASS = 4;
const int PATH = 5;
const int FARMLAND_DRY = 6;
const int FARMLAND_MOIST = 7;
const int WATER = 8; // use WATER_SOURCE instead
const int MELON = 9;
const int PUMPKIN = 10;
const int PUMPKIN_CARVED = 11;
const int PUMPKIN_LANTERN = 12;
const int TORCH = 13;
const int OAK_LEAVES = 14;
const int OAK_LOG = 15;
const int OAK_PLANK = 16;
const int CRAFTING_TABLE = 17;
const int CHEST = 18;
const int BIRCH_PLANK = 19;
const int BIRCH_LOG = 20;
const int BIRCH_LEAVES = 21;
const int STONE = 22;
const int IRON = 23;
const int COAL = 24;
const int DIAMOND = 25;
const int COBBLESTONE = 26;
const int FURNACE_OFF = 27;
const int FURNACE_ON = 28;
const int STONE_BRICK = 29;
//const int LAVA = 30; // use LAVA_SOURCE instead
const int OBSIDIAN = 31;
const int BEDROCK = 32;
const int WATER_1 = 128;
const int WATER_2 = 129;
const int WATER_3 = 130;
const int WATER_4 = 131;
const int WATER_5 = 132;
const int WATER_6 = 133;
const int WATER_7 = 134;
const int WATERFALL = 135;
const int WATER_SOURCE = 136;
const int LAVA_1 = 137;
const int LAVA_2 = 138;
const int LAVA_3 = 139;
const int LAVA_WATERFALL = 140;
const int LAVA_SOURCE = 141;
const int LAST = 32;

const float oneEighth		= 0.125; // 1 / 8
const float sevenSixteenth	= 0.4375; // 7 / 16

const float frameWidth		= 0.03125; // 1 / 32
const float frameHeight		= 0.12500; // 1 / 8
const float frameLeft		= (WATER - 1) * frameWidth;
const float frameRight		= WATER * frameWidth;
const float frameHeightTop	= 0.12500;



	// instance variable
vec4 positionCenter;

const float back	= -0.5;
const float bottom	= -0.5;
const float front	=  0.5;
const float left	= -0.5;
const float right	=  0.5;
float topLB;
float topLF;
float topRB;
float topRF;

float faceStill		= 7.0;
float faceFlow		= 3.5;
const float faceBottom	= 2.0;
vec2 frameTopLB;
vec2 frameTopLF;
vec2 frameTopRB;
vec2 frameTopRF;

float frameHeightLB;
float frameHeightLF;
float frameHeightRB;
float frameHeightRF;
const float pixelHeight = 0.0625; // 1 / 16



	// function forward declaration
bool IsWater(int block);
int Block(int x, int y, int z);

void SetPhysicalCorner(int x, int y, int z);
void SetGraphicCorner();
float GetPhysicalHeight(int block);
float GetGraphicHeight(float physicalHeight);

void Back();
void Bottom();
void Front();
void Left();
void Right();
void Top();

void UnderwaterBack();
void UnderwaterFront();
void UnderwaterLeft();
void UnderwaterRight();
void UnderwaterTop();

void StillTop();



void main()
{
	int index = vertexShader[0].index;
	
	int x = index / (chunkWidth * (highestBlock));
	int y = (index / chunkDepth) % (highestBlock);
	int z = index % chunkDepth;

	if (!IsWater(Block(x, y, z)))
	{
		return;
	}
	
	positionCenter = vec4(x + chunkPosition.x * chunkWidth, y, z + chunkPosition.y * chunkDepth, 1.0);
	SetPhysicalCorner(x, y, z);
	
	SetGraphicCorner();

	if (IsWater(Block(x, y+1, z)))
		UnderwaterTop();
	else if (topLB == topLF && topLB == topRB && topLB == topRF)
		StillTop();
	else
		Top();

	if (IsWater(Block(x, y, z+1)))
		UnderwaterFront();
	else
		Front();

	if (IsWater(Block(x+1, y, z)))
		UnderwaterRight();
	else
		Right();

	if (IsWater(Block(x, y, z-1)))
		UnderwaterBack();
	else
		Back();

	if (IsWater(Block(x-1, y, z)))
		UnderwaterLeft();
	else
		Left();

	Bottom();
}



void SetPhysicalCorner(int x, int y, int z)
{
	if (IsWater(Block(x, y+1, z)))
	{
		topLB = 0.5;
		topLF = 0.5;
		topRB = 0.5;
		topRF = 0.5;
		return;
	}

	float centerHeight = GetPhysicalHeight(Block(x, y, z));
	topLB = centerHeight;
	topLF = centerHeight;
	topRB = centerHeight;
	topRF = centerHeight;

	float leftHeight = GetPhysicalHeight(Block(x-1, y, z));
	float leftFrontHeight = GetPhysicalHeight(Block(x-1, y, z+1));
	float leftBackHeight = GetPhysicalHeight(Block(x-1, y, z-1));
	float frontHeight = GetPhysicalHeight(Block(x, y, z+1));
	float backHeight = GetPhysicalHeight(Block(x, y, z-1));
	float rightHeight = GetPhysicalHeight(Block(x+1, y, z));
	float rightFrontHeight = GetPhysicalHeight(Block(x+1, y, z+1));
	float rightBackHeight = GetPhysicalHeight(Block(x+1, y, z-1));

	if (topLB < leftHeight)
	{
		topLB = leftHeight < leftBackHeight ? leftBackHeight : leftHeight;
		topLF = leftHeight < leftFrontHeight ? leftFrontHeight : leftHeight;
	}
	
	if (topRB < rightHeight)
	{
		topRB = rightHeight < rightBackHeight ? rightBackHeight : rightHeight;
		topRF = rightHeight < rightFrontHeight ? rightFrontHeight : rightHeight;
	}

	if (topLB < backHeight)
		topLB = backHeight < leftBackHeight ? leftBackHeight : backHeight;

	if (topLF < frontHeight)
		topLF = frontHeight < leftFrontHeight ? leftFrontHeight : frontHeight;

	if (topRB < backHeight)
		topRB = backHeight < rightBackHeight ? rightBackHeight : backHeight;

	if (topRF < frontHeight)
		topRF = frontHeight < rightFrontHeight ? rightFrontHeight : frontHeight;
}



void SetGraphicCorner()
{
	faceFlow += ticks * pixelHeight;
	int still = ticks / 2;
	if (still % 2 == 1)
		faceStill -= pixelHeight;
	else if (still % 4 == 0)
		faceStill -= 2 * pixelHeight;

	frameHeightLB = GetGraphicHeight(topLB);
	frameHeightLF = GetGraphicHeight(topLF);
	frameHeightRB = GetGraphicHeight(topRB);
	frameHeightRF = GetGraphicHeight(topRF);

	if (topLB == topLF)
	{
		if (topLB < topRB)
		{
			frameTopLB = vec2(frameLeft, faceFlow * frameHeight);
			frameTopLF = vec2(frameRight, faceFlow * frameHeight);
			frameTopRB = vec2(frameLeft, faceFlow * frameHeight + frameHeightTop);
			frameTopRF = vec2(frameRight, faceFlow * frameHeight + frameHeightTop);
		}
		else
		{
			frameTopLB = vec2(frameLeft, faceFlow * frameHeight + frameHeightTop);
			frameTopLF = vec2(frameRight, faceFlow * frameHeight + frameHeightTop);
			frameTopRB = vec2(frameLeft, faceFlow * frameHeight);
			frameTopRF = vec2(frameRight, faceFlow * frameHeight);
		}
	}
	else
	{
		if (topLB < topLF)
		{
			frameTopLB = vec2(frameLeft, faceFlow * frameHeight);
			frameTopLF = vec2(frameLeft, faceFlow * frameHeight + frameHeightTop);
			frameTopRB = vec2(frameRight, faceFlow * frameHeight);
			frameTopRF = vec2(frameRight, faceFlow * frameHeight + frameHeightTop);
		}
		else
		{
			frameTopLB = vec2(frameLeft, faceFlow * frameHeight + frameHeightTop);
			frameTopLF = vec2(frameLeft, faceFlow * frameHeight);
			frameTopRB = vec2(frameRight, faceFlow * frameHeight + frameHeightTop);
			frameTopRF = vec2(frameRight, faceFlow * frameHeight);
		}
	}

//	Diagonal
//
//		frameTopLB = vec2(frameLeft, faceFlow * frameHeight);
//		frameTopLF = vec2(frameRight, faceFlow * frameHeight + frameHeightTop);
//		frameTopRB = vec2(frameRight, faceFlow * frameHeight);
//		frameTopRF = vec2(frameLeft, faceFlow * frameHeight + frameHeightTop);
//	}
//	else
//	{
//		frameTopLB = vec2(frameRight, faceFlow * frameHeight + frameHeightTop);
//		frameTopLF = vec2(frameLeft, faceFlow * frameHeight);
//		frameTopRB = vec2(frameLeft, faceFlow * frameHeight + frameHeightTop);
//		frameTopRF = vec2(frameRight, faceFlow * frameHeight);
}



float GetPhysicalHeight(int block)
{
	if (block == WATER_SOURCE)
		return sevenSixteenth;
	return IsWater(block) ? -sevenSixteenth + (block - WATER_1) * oneEighth : -0.5;
}



float GetGraphicHeight(float physicalHeight)
{
	return (physicalHeight + 0.5) * frameHeight;
}



void Back()
{
	vertexData.normal = vec3(0, 0, -1);
	
	vertexData.texel = vec2(frameRight, faceFlow * frameHeight + frameHeightLB);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLB,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameLeft, faceFlow * frameHeight + frameHeightRB);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRB,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameRight, faceFlow * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameLeft, faceFlow * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom,  back, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void UnderwaterBack()
{
	vertexData.normal = vec3(0, 0, -1);
	
	vertexData.texel = vec2(frameRight, faceBottom * frameHeight + frameHeightLB);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLB,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight + frameHeightRB);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRB,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameRight, faceBottom * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom,  back, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void Bottom()
{
	vertexData.normal = vec3(0, -1, 0);
	
	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight + frameHeightTop);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom, front, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceBottom * frameHeight + frameHeightTop);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceBottom * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom,  back, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void Front()
{
	vertexData.normal = vec3(0, 0, 1);
	
	vertexData.texel = vec2(frameLeft, faceFlow * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceFlow * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameLeft, faceFlow * frameHeight + frameHeightLF);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLF, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceFlow * frameHeight + frameHeightRF);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRF, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void UnderwaterFront()
{
	vertexData.normal = vec3(0, 0, 1);
	
	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceBottom * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight + frameHeightLF);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLF, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceBottom * frameHeight + frameHeightRF);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRF, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void Left()
{
	vertexData.normal = vec3(-1, 0, 0);
	
	vertexData.texel = vec2(frameLeft, faceFlow * frameHeight + frameHeightLB);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLB,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameLeft, faceFlow * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceFlow * frameHeight + frameHeightLF);
    gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLF, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceFlow * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void UnderwaterLeft()
{
	vertexData.normal = vec3(-1, 0, 0);
	
	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight + frameHeightLB);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLB,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceBottom * frameHeight + frameHeightLF);
    gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLF, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceBottom * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void Right()
{
	vertexData.normal = vec3(1, 0, 0);
	
	vertexData.texel = vec2(frameRight, faceFlow * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameRight, faceFlow * frameHeight + frameHeightRB);
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRB,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameLeft, faceFlow * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameLeft, faceFlow * frameHeight + frameHeightRF);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRF, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void UnderwaterRight()
{
	vertexData.normal = vec3(1, 0, 0);
	
	vertexData.texel = vec2(frameRight, faceBottom * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameRight, faceBottom * frameHeight + frameHeightRB);
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRB,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight + frameHeightRF);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRF, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void Top()
{
	vertexData.normal = vec3(0, 1, 0);
	
	vertexData.texel = frameTopLB;
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLB,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = frameTopLF;
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLF, front, 0.0));
    EmitVertex();

	vertexData.texel = frameTopRB;
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRB,  back, 0.0));
    EmitVertex();

	vertexData.texel = frameTopRF;
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRF, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void StillTop()
{
	vertexData.normal = vec3(0, 1, 0);
	
	vertexData.texel = vec2(frameLeft, faceStill * frameHeight + frameHeightTop);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLB,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameLeft, faceStill * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLF, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceStill * frameHeight + frameHeightTop);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRB,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceStill * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRF, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void UnderwaterTop()
{
	vertexData.normal = vec3(0, 1, 0);
	
	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight + frameHeightTop);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLB,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(frameLeft, faceBottom * frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,  topLF, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceBottom * frameHeight + frameHeightTop);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRB,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(frameRight, faceBottom * frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,  topRF, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



bool IsWater(int block)
{
	return block >= WATER_1
		&& block <= WATER_SOURCE;
}



int Block(int x, int y, int z)
{
	if (x < 0
		|| y < 0
		|| z < 0
		|| x >= chunkWidth
		|| y >= chunkHeight
		|| z >= chunkDepth)
	{
		return NO_BLOCK;
	}
	
	return chunkData[x * chunkHeight * chunkDepth + y * chunkDepth + z];
}
