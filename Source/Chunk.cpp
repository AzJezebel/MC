#include "Chunk.h"
#include "Biome.h"

#pragma warning( push )
#pragma warning( disable : 4701 ) // potentially uninitialized local variable 'Result' used	(glm) func_common.inl L-99
#include <glm/gtc/noise.hpp>
#pragma warning( pop )

using namespace Minecraft;

// * 0.25 is a replacement for / 2 on each size
float const Chunk::s_radius = sqrtf((float)(CHUNK_X_SIZE * CHUNK_X_SIZE + CHUNK_Z_SIZE * CHUNK_Z_SIZE) * 0.25f);

Chunk::Chunk() :
	chunkXPos(0),
	chunkZPos(0),
	blocks()
{
	glCreateBuffers(1, &m_SSBO);
	Generate(nullptr);
}

Chunk::Chunk(int x, int z, Noise* noise, ChunkGenPasses passes) :
	chunkXPos(x),
	chunkZPos(z),
	blocks()
{
	glCreateBuffers(1, &m_SSBO);
	Generate(noise, passes);
	CheckHighestBlock();
}

void Chunk::Generate(Noise* noise, ChunkGenPasses passes)
{
#ifdef GENERATE_FLAT_CHUNKS
	for (int i = 0; i < CHUNK_X_SIZE; i++)
		for (int k = 0; k < CHUNK_Z_SIZE; k++)
			blocks[i][0][k] = BlockId::STONE;
	(void)noise;
	return;
#else

	if (noise == nullptr)
		return;
	/*
		STONE / WATER PASS
	*/

	int highestBlockArray[16][16] = { {0} };

	for (int i = 0; i < CHUNK_X_SIZE; i++)
		for (int k = 0; k < CHUNK_Z_SIZE; k++)
		{
			int height = (int)noise->GetHeight(i, k, chunkXPos, chunkZPos);
			highestBlockArray[i][k] = height;

			for (int j = 0; j < height; j++)
			{
				blocks[i][j][k] = BlockId::STONE;
			}
			if (height < CHUNK_WATER_LEVEL)
				for (int j = height; j < CHUNK_WATER_LEVEL; j++)
					blocks[i][j][k] = BlockId::WATER_SOURCE;
		}
	/*
		BIOME SURFACE PASS
	*/

	if (passes >= ChunkGenPasses::BIOME_SURFACE_PASS)
	{
		for (int i = 0; i < CHUNK_X_SIZE; i++)
			for (int k = 0; k < CHUNK_Z_SIZE; k++)
			{
				int height = highestBlockArray[i][k] - 1;

				const Biome& biome = GetStaticBiome(noise->GetBiome(height));

				for (int j = 0; j < biome.surfaceDepth; j++)
				{
					if (height - j < 0)
						break;
					blocks[i][height - j][k] = biome.surfaceBlock;
				}

				for (int j = biome.surfaceDepth; j < biome.surfaceDepth + biome.subsurfaceDepth; j++)
				{
					if (height - j < 0)
						break;
					blocks[i][height - j][k] = biome.subsurfaceBlock;
				}
			}
	}

	/*
		CAVE / STRUCTURE PASS
	*/
	if (passes >= ChunkGenPasses::CAVE_STRUCTURE_PASS)
	{
		for (int i = 0; i < CHUNK_X_SIZE; i++)
			for (int k = 0; k < CHUNK_Z_SIZE; k++)
				for (int j = 0; j < highestBlockArray[i][k]; j++)
				{
					if (noise->GetCave(i, j, k, chunkXPos, chunkZPos))
						blocks[i][j][k] = BlockId::AIR;
				}
	}

	/*
		FLORA PASS
	*/

	if (passes >= ChunkGenPasses::FLORA_PASS)
	{
		for (int i = 0; i < CHUNK_X_SIZE; i++)
			for (int k = 0; k < CHUNK_Z_SIZE; k++)
			{
				int height = highestBlockArray[i][k] - 1;

				const BiomeType biome = noise->GetBiome(height);

				if ((biome == BiomeType::FOREST || biome == BiomeType::PLAINS) && noise->GetTree(i, k, chunkXPos, chunkZPos))
					GenerateTree(i, k);
			}
	}
#endif

	CheckHighestBlock();
}

Minecraft::Chunk::~Chunk()
{
	glDeleteBuffers(1, &m_SSBO);
}

void Chunk::GenerateTree(int xPos, int zPos)
{
	if (xPos > 1 && xPos < 14 && zPos > 1 && zPos < 14)
	{
		int yPos = GetHighestBlock(xPos, zPos) + 1;

		// Trunk
		int height = rand() % 2 + 4;

		for (int i = 0; i < height; i++)
		{
			blocks[xPos][yPos + i][zPos] = BlockId::OAK_LOG;
		}

		// Leaves (first layer)
		for (int k = 1; k < 3; k++) {
			for (int i = -2; i <= 2; i++)
			{
				for (int j = -2; j <= 2; j++)
				{
					if (k == 1)
						if ((i == -2 || i == 2) && (j == -2 || j == 2))
							continue;

					int currentX = xPos + i;
					int currentZ = zPos + j;

					if (blocks[currentX][yPos + height - k][currentZ] == BlockId::AIR)
						blocks[currentX][yPos + height - k][currentZ] = BlockId::OAK_LEAVES;
				}
			}
		}

		// Leaves (second layer)
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				int currentX = xPos + i;
				int currentZ = zPos + j;

				if (blocks[currentX][yPos + height][currentZ] == BlockId::AIR)
					blocks[currentX][yPos + height][currentZ] = BlockId::OAK_LEAVES;
			}
		}
	}
}

const std::stringstream Chunk::Save() const
{
	std::stringstream ret;

	ret << "#POS\n" << chunkXPos << " " << chunkZPos << "\n";

	int blockCount = 0;
	int lastBlock = blocks[0][0][0];
	ret << "#DATA\n";
	// Save by layer
	for (int i = 0; i < CHUNK_Y_SIZE; i++)
		for (int j = 0; j < CHUNK_X_SIZE; j++)
			for (int k = 0; k < CHUNK_Z_SIZE; k++)
			{
				// force write on last block, even if same
				if (lastBlock != blocks[j][i][k] ||
					(i == CHUNK_Y_SIZE - 1 && j == CHUNK_X_SIZE - 1 && k == CHUNK_Z_SIZE - 1)) {
					ret << (int)lastBlock << " " << blockCount << "\n";
					blockCount = 1;
					lastBlock = blocks[j][i][k];
				}
				else
					blockCount++;
			}
	return ret;
}

int Chunk::GetHighestBlock(int x, int z, int y, bool ignoreNonSolidBlocks)
{
	int highest = 0;
	for (int i = CHUNK_Y_SIZE - 1; i >= y; i--)
		if (ignoreNonSolidBlocks)
		{
			if (blocks[x][i][z] != BlockId::AIR &&
				blocks[x][i][z] != BlockId::WATER_SOURCE &&
				blocks[x][i][z] != BlockId::OAK_LEAVES)
			{
				highest = i;
				break;
			}
		}
		else
			if (blocks[x][i][z] != BlockId::AIR)
			{
				highest = i;
				break;
			}

	return highest;
}

void Chunk::CheckHighestBlock()
{
	highestBlock = 0;
	for (int j = 0; j < CHUNK_X_SIZE; j++)
	{
		for (int k = 0; k < CHUNK_Z_SIZE; k++)
		{
			int y = GetHighestBlock(j, k, highestBlock, false);
			if (highestBlock < y)
				highestBlock = y;
		}
	}
}

void Minecraft::Chunk::Link(int bindingPoint)
{
	if (!m_isUpToDate)
	{
		glNamedBufferData(m_SSBO, sizeof(*this), this, GL_STATIC_DRAW);
		m_isUpToDate = true;
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_SSBO);
}
