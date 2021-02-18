#pragma once

#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "Chunk.h"
#include "Physics.h"
#include "Noise.h"
#include "Mesh.h"
#include "DynamicBlock.h"
#include "Trigo.h"

#include <irrKlang.h>
#include <map>
#include <list>

namespace Minecraft
{
	class World
	{
	public:
		World() = delete;
		World(float fieldOfView, float aspectRatio, float far);
		~World() = default;

		void UpdateWorld(float dT);
		void Save(const std::string& saveFile);
		void Load(const std::string& saveFile);

		void ToggleCreativeMode() { m_creativeMode = !m_creativeMode; }
		bool InCreativeMode() const { return m_creativeMode; }

		Mesh& GetMesh() { return m_mesh; }
		Mesh const& GetMesh() const { return m_mesh; }
		Player const& GetPlayer() const { return m_player; }
		Player& GetPlayer() { return m_player; }
		void SetPlayer(const Player& player) { m_player = player; }
		Camera& GetCamera() { return m_camera; }
		std::vector<Enemy>& GetEnemies() { return m_enemies; }
		std::vector<Enemy> const& GetEnemies() const { return m_enemies; }

		// Refreshes the player chunk, checking if new chunks need to be loaded
		void CheckPlayerCenter();

		// Updates the rendered chunk array, both offsets can be -1, 0, or 1 to define what direction to shift the array
		void UpdateRenderedChunks(const int xOffset, const int zOffset);

		// Gets the chunk position that the player is in
		ivec2 GetPlayerChunkPos();

		// Gets the chunk that the player is in
		Chunk& GetPlayerChunk();
		Chunk* GetChunk(int chunkX, int chunkZ);
		std::vector<DynamicBlock> const& GetFallingBlocks() const { return m_fallingBlocks; }
		int GetBlock(vec3 pos);
		int GetBlock(int x, int y, int z);
		void SetBlock(int x, int y, int z, int block);

		Chunk* GenerateNewChunk(int x, int z);

		void GenerateWorld();
		Chunk* renderedChunks[CHUNK_ARRAY_SIZE][CHUNK_ARRAY_SIZE];

		std::map<int, std::map<int, Chunk*>> allChunks;

		Noise m_noise;

		irrklang::ISoundEngine* GetSoundEngine();

		ChunkGenPasses passes = ChunkGenPasses::ALL_PASSES;
		bool m_forcePause = false;
	private:
		void UpdateFallingBlockList(float dT);
		void AddToFallingBlocks(int x, int y, int z, int block);

		void UpdateFlowingBlockList(float dT);
		void AddToFlowingBlocks(int x, int y, int z, int block, float flowSpeed);
		void UpdateWaterBlock(DynamicBlock const& current);
		int GetSelfWaterLevel(int blockOver, int blockSide1, int blockSide2, int blockSide3, int blockSide4) const;
		void Overflow(int sourceX, int sourceY, int sourceZ, int sourceBlock, int targetBlock, Face direction);
		void UpdateLavaBlock(DynamicBlock const& current);
		int GetHighestSpawnable(int x, int z);
		void GenerateMob();

		Player m_player;
		Camera m_camera;
		std::vector<Enemy> m_enemies;
		std::vector<DynamicBlock> m_fallingBlocks;
		std::vector<DynamicBlock> m_blockWaitingToFall;
		std::list<DynamicBlock> m_flowingBlocks;

		Mesh m_mesh;
		Physics m_physics;
		bool m_creativeMode = false;
		irrklang::ISoundEngine* m_soundEngine;

		float m_nextMobTimer = 0.0f;


	public:
		Physics const& physics = m_physics;
	};
}
