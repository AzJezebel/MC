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

uniform int highestBlock;

layout (std140) uniform Matrice {
    uniform mat4 viewProjection;
} matrice;



	// storage buffer
layout(std430, binding = 0) buffer chunk
{
    ivec2 chunkPosition;
    int chunkData[65536];
};
layout(std430, binding = 1) buffer leftChunk
{
    ivec2 leftChunkPosition;
    int leftChunkData[65536];
};
layout(std430, binding = 2) buffer rightChunk
{
    ivec2 rightChunkPosition;
    int rightChunkData[65536];
};
layout(std430, binding = 3) buffer frontChunk
{
    ivec2 frontChunkPosition;
    int frontChunkData[65536];
};
layout(std430, binding = 4) buffer backChunk
{
    ivec2 backChunkPosition;
    int backChunkData[65536];
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
//const int WATER = 8; // use WATER_SOURCE instead
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

const float spriteWidth  = 0.03125;// 1 / 32
const float spriteHeight = 0.12500;// 1 / 8
//const float pixelWidth =  0.001953125;// spriteWidth / 16
//const float pixelHeight = 0.007812500;// spriteHeight / 16
const float sixteenth = 0.0625;				// Block Width
const float eighth = 0.125;					// Block Height
const float sevenSixteenth = 0.4375;		// Block Width

const float frameEighth = 0.078125;				// Frame Height
const float frameFiveEighth = 0.078125;			// Frame Height
const float frameFifteenSixteenth = 0.1171875;	// Frame Height
const float frameSevenSixteenth = 0.013671875;	// Block Width
const float frameSixteenth = 0.001953125;		// Frame Width


	// instance variable
int blockId;
vec4 positionCenter;
float textureCenter;

float bottom	= -0.5;
float top		=  0.5;
float back		= -0.5;
float front		=  0.5;
float left		= -0.5;
float right		=  0.5;

float faceTop		= 7.5;
float faceFront		= 6.0;
float faceRight		= 5.0;
float faceBack		= 4.0;
float faceLeft		= 3.0;
float faceBottom	= 2.5;

float frameHalfWidth	= 0.015625;
float frameHalfLenght	= 0.015625;
float frameHeight		= 0.12500;



	// function forward declaration
int Block(int x, int y, int z);
bool IsSeeThrough(int block);
bool IsSkip(int block);
bool IsPartial(int block);

void SetPhysicalCorner();
void SetGraphicCorner();

void Back();
void Bottom();
void Front();
void Left();
void Right();
void Top();



void main()
{
	int index = vertexShader[0].index;
	
	
	int x = index / (chunkWidth * (highestBlock));
	int y = (index / chunkDepth) % (highestBlock);
	int z = index % chunkDepth;

	blockId = Block(x, y, z);
	if (IsSkip(Block(x, y, z)))
	{
		return;
	}
	
	positionCenter = vec4(x + chunkPosition.x * chunkWidth, y, z + chunkPosition.y * chunkDepth, 1.0);
	
	bool isPartial = IsPartial(index);

	SetPhysicalCorner();
	SetGraphicCorner();

	if (isPartial || IsSeeThrough(Block(x - 1, y, z)))
		Left();
	if (isPartial || IsSeeThrough(Block(x + 1, y, z)))
		Right();
	
	if (isPartial || IsSeeThrough(Block(x, y - 1, z)))
		Bottom();
	if (isPartial || IsSeeThrough(Block(x, y + 1, z)))
		Top();
	
	if (isPartial || IsSeeThrough(Block(x, y, z - 1)))
		Back();
	if (isPartial || IsSeeThrough(Block(x, y, z + 1)))
		Front();
}



void SetPhysicalCorner()
{
	switch (blockId)
	{
		case PATH:
			top		=  sevenSixteenth;
			break;
		case FARMLAND_DRY:
			top		=  sevenSixteenth;
			break;
		case FARMLAND_MOIST:
			top		=  sevenSixteenth;
			break;
		case TORCH:
			top		=  eighth;
			back	= -sixteenth;
			front	=  sixteenth;
			left	= -sixteenth;
			right	=  sixteenth;
			break;
		case CHEST:
			top		=  sevenSixteenth;
			back	= -sevenSixteenth;
			front	=  sevenSixteenth;
			left	= -sevenSixteenth;
			right	=  sevenSixteenth;
			break;
//		case LAVA:
//			top		=  sevenSixteenth;
//			break;
	}
}

void SetGraphicCorner()
{
	textureCenter = blockId * spriteWidth - frameHalfWidth;

	switch (blockId)
	{
		case PATH:
			frameHeight		= frameFifteenSixteenth;
			break;
		case FARMLAND_DRY:
			frameHeight		= frameFifteenSixteenth;
			break;
		case FARMLAND_MOIST:
			frameHeight		= frameFifteenSixteenth;
			break;
		case TORCH:
			frameHeight		= frameEighth;
			frameHalfLenght	= frameSixteenth;
			frameHalfWidth	= frameSixteenth;
			break;
		case CHEST:
			frameHeight		= frameFifteenSixteenth;
			frameHalfLenght	= frameSevenSixteenth;
			frameHalfWidth	= frameSevenSixteenth;
			break;
//		case LAVA:
//			frameHeight		= frameFifteenSixteenth;
//			break;
	}
}



void Back()
{
	vertexData.normal = vec3(0, 0, -1);
	
	vertexData.texel = vec2(textureCenter + frameHalfWidth, faceBack * spriteHeight + frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,    top,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter - frameHalfWidth, faceBack * spriteHeight + frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,    top,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(textureCenter + frameHalfWidth, faceBack * spriteHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter - frameHalfWidth, faceBack * spriteHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom,  back, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void Bottom()
{
	vertexData.normal = vec3(0, -1, 0);
	
	vertexData.texel = vec2(textureCenter - frameHalfWidth, faceBottom * spriteHeight + frameHalfLenght * 4.0);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom, front, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(textureCenter - frameHalfWidth, faceBottom * spriteHeight - frameHalfLenght * 4.0);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter + frameHalfWidth, faceBottom * spriteHeight + frameHalfLenght * 4.0);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter + frameHalfWidth, faceBottom * spriteHeight - frameHalfLenght * 4.0);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom,  back, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void Front()
{
	vertexData.normal = vec3(0, 0, 1);
	
	vertexData.texel = vec2(textureCenter - frameHalfWidth, faceFront * spriteHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter + frameHalfWidth, faceFront * spriteHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(textureCenter - frameHalfWidth, faceFront * spriteHeight + frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,    top, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter + frameHalfWidth, faceFront * spriteHeight + frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,    top, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void Left()
{
	vertexData.normal = vec3(-1, 0, 0);
	
	vertexData.texel = vec2(textureCenter - frameHalfLenght, faceLeft * spriteHeight + frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,    top,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(textureCenter - frameHalfLenght, faceLeft * spriteHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter + frameHalfLenght, faceLeft * spriteHeight + frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4( left,    top, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter + frameHalfLenght, faceLeft * spriteHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4( left, bottom, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void Right()
{
	vertexData.normal = vec3(1, 0, 0);
	
	vertexData.texel = vec2(textureCenter + frameHalfLenght, faceRight * spriteHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(textureCenter + frameHalfLenght, faceRight * spriteHeight + frameHeight);
	gl_Position = matrice.viewProjection * (positionCenter + vec4(right,    top,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter - frameHalfLenght, faceRight * spriteHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right, bottom, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter - frameHalfLenght, faceRight * spriteHeight + frameHeight);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,    top, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



void Top()
{
	vertexData.normal = vec3(0, 1, 0);
	
	vertexData.texel = vec2(textureCenter - frameHalfWidth, faceTop * spriteHeight + frameHalfLenght * 4.0);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,    top,  back, 0.0));
    EmitVertex();
	
	vertexData.texel = vec2(textureCenter - frameHalfWidth, faceTop * spriteHeight - frameHalfLenght * 4.0);
	gl_Position = matrice.viewProjection * (positionCenter + vec4( left,    top, front, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter + frameHalfWidth, faceTop * spriteHeight + frameHalfLenght * 4.0);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,    top,  back, 0.0));
    EmitVertex();

	vertexData.texel = vec2(textureCenter + frameHalfWidth, faceTop * spriteHeight - frameHalfLenght * 4.0);
    gl_Position = matrice.viewProjection * (positionCenter + vec4(right,    top, front, 0.0));
    EmitVertex();
    
    EndPrimitive();
}



bool IsSeeThrough(int block)
{
	if (block < 0)
	{
		return true;
	}
	
	return IsSkip(block)
		|| IsPartial(block);
}



bool IsSkip(int block)
{
	return block == AIR
		|| block > LAST;
}



bool IsPartial(int block)
{
	return block == PATH
		|| block == FARMLAND_DRY
		|| block == FARMLAND_MOIST
		|| block == TORCH
		|| block == CHEST;
}



int Block(int x, int y, int z)
{
	if (y < 0 || y >= chunkHeight)
	{
		return -1;
	}
	else if (x < 0)
	{
		int index = 15 * chunkHeight * chunkDepth + y * chunkDepth + z;
		return leftChunkData[index];
	}
	else if (x >= chunkWidth)
	{
		int index = 0 * chunkHeight * chunkDepth + y * chunkDepth + z;
		return rightChunkData[index];
	}
	else if (z < 0)
	{
		int index = x * chunkHeight * chunkDepth + y * chunkDepth + 15;
		return frontChunkData[index];
	}
	else if (z >= chunkDepth)
	{
		int index = x * chunkHeight * chunkDepth + y * chunkDepth + 0;
		return backChunkData[index];
	}
	
	int index = x * chunkHeight * chunkDepth + y * chunkDepth + z;
	return chunkData[index];
}
