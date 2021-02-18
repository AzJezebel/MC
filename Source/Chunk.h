#pragma once

#include "BlockId.h"
#include "Noise.h"
#include "define.h"

#include <iostream>
#include <sstream>
#include <glad\glad.h>

enum class ChunkGenPasses : int
{
	STONE_WATER_PASS, // Generate a world heightmap and add water level
	BIOME_SURFACE_PASS, // Perform general terrain pass, generate biomes and add blocks to the surface of the world
	CAVE_STRUCTURE_PASS, // Perform general terrain pass, Biome and surface block pass, and generate caves, ores, structures, etc.
	FLORA_PASS, // Perform all generation passes, including generating trees, flowers, etc.

	ALL_PASSES // Perform all generation passes. Technically useless but it's nice to have for clarity
};

namespace Minecraft
{
	class Chunk final
	{
	public:
		Chunk();
		Chunk(int x, int z, Noise* noise, ChunkGenPasses passes = ChunkGenPasses::ALL_PASSES);
		void Generate(Noise* noise, ChunkGenPasses passes = ChunkGenPasses::ALL_PASSES);

		~Chunk();

		const std::stringstream Save() const;
		//void Load();

		void CheckHighestBlock();
		void Link(int bindingPoint);

		// Find the highest block in a given x/z
		// Will only look above a certain y if specified
		// Will treat water as a solid block if ignoreWater is set to false
		// returns 0 if none is found
		int GetHighestBlock(int x, int z, int y = 0, bool ignoreNonSolidBlocks = true);

		void SetBlock(int x, int y, int z, int block)
		{
			blocks[x][y][z] = block;
			if (y > highestBlock)
				highestBlock = y;
			m_isUpToDate = false;
		}

		int GetBlock(int x, int y, int z)
		{
			if (0 <= x && x < CHUNK_X_SIZE &&
				0 <= y && y < CHUNK_Y_SIZE &&
				0 <= z && z < CHUNK_Z_SIZE)
				return blocks[x][y][z];
			else
				return BlockId::NO_BLOCK;
		}

		int chunkXPos;
		int chunkZPos;

	private:
		int blocks[CHUNK_X_SIZE][CHUNK_Y_SIZE][CHUNK_Z_SIZE];
		GLuint m_SSBO = 0;
		bool m_isUpToDate = false;

		void GenerateTree(int xPos, int zPos);

	public:
		int highestBlock = 0;

		static float const s_radius;
	};
}