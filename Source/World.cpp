#include "World.h"
#include <iostream>
#include <fstream>
#include <time.h>

#include "define.h"
#include "BlockLookup.h"

namespace Minecraft
{
	World::World(float fieldOfView, float aspectRatio, float far) :
		m_player(*this, { 0, 3, 0 }, { Trigo::DEGREE_TO_RADIAN * 225.f, 0 }), m_physics(*this),
		m_camera(m_player.GetPosition(), m_player.GetRotation(), m_player.GetHeight(), fieldOfView, aspectRatio, far),
		renderedChunks(),
		m_soundEngine(irrklang::createIrrKlangDevice())
	{
	}

	void World::UpdateWorld(float dT)
	{
		GetSoundEngine()->setListenerPosition(irrklang::vec3df(GetPlayer().GetPosition().x, GetPlayer().GetPosition().y, GetPlayer().GetPosition().z),
			irrklang::vec3df(GetPlayer().GetRotation().x, 0, GetPlayer().GetRotation().y));

		UpdateFallingBlockList(dT);
		UpdateFlowingBlockList(dT);

		m_nextMobTimer -= dT;

		if (m_nextMobTimer <= 0.f && m_enemies.size() < MAX_ENEMIES)
		{
			GenerateMob();
			m_nextMobTimer = (rand() % 100 / (MAX_BEFORE_SPAWN - MIN_BEFORE_SPAWN)) + MIN_BEFORE_SPAWN;
		}

		for (size_t index = 0; index < m_enemies.size(); index++)
		{
			Enemy& currEnemy = m_enemies[index];
			currEnemy.Update(m_player, dT);

			currEnemy.EditInWater() = BlockLookup::IsWater(this->GetBlock(m_enemies[index].GetPosition()));
			currEnemy.EditUnderWater() = BlockLookup::IsWater(this->GetBlock({ currEnemy.GetPosition().x, currEnemy.GetPosition().y + 1, currEnemy.GetPosition().z }));

			if (currEnemy.EditUnderWater())
				currEnemy.Drown(dT);

			if (currEnemy.GetLife() <= 0)
			{
				m_enemies.erase(m_enemies.begin() + index);
				index--;
			}
		}
	}

	void World::Save(const std::string& saveFile)
	{
		std::ofstream file;

		file.open(saveFile);
		file << "#PLAYER\n";

		Minecraft::vec3 pos = m_player.GetPosition();
		file << pos.x << " " << pos.y << " " << pos.z << "\n";

		Minecraft::vec2 rot = m_player.GetRotation();

		file << rot.x << " " << rot.y << "\n";

		for (int i = 0; i < 27; i++)
		{
			file << (int)m_player.GetInventory().GetItemInventoryArray()[i].GetItemNameEnum() << " ";
			file << m_player.GetInventory().GetItemInventoryArray()[i].GetCountItem() << "\n";
		}

		for (int i = 0; i < 9; i++)
		{
			file << (int)m_player.GetInventory().GetItemInventoryBarArray()[i].GetItemNameEnum() << " ";
			file << m_player.GetInventory().GetItemInventoryBarArray()[i].GetCountItem() << "\n";
		}

		file << "#CHUNKS\n";

		for (auto& xc : allChunks)
		{
			for (auto& zc : xc.second)
			{
				file << zc.second->Save().rdbuf();
			}
		}

		file.close();
	}

	void World::Load(const std::string& saveFile)
	{
		std::fstream file;
		std::string output;

		file.open(saveFile);

		while (file >> output)
		{
			if (output == "#PLAYER")
			{
				file >> output;
				float playerXPos = stof(output);
				file >> output;
				float playerYPos = stof(output);
				file >> output;
				float playerZPos = stof(output);

				m_player.SetPosition(Minecraft::vec3(playerXPos, playerYPos, playerZPos));

				file >> output;
				float playerXRot = stof(output);
				file >> output;
				float playerYRot = stof(output);

				m_player.SetRotation(playerXRot, playerYRot);

				std::string itemName;
				for (int i = 0; i < 27; i++)
				{
					file >> itemName;
					file >> output;

					m_player.GetInventory().GetItemInventoryArray()[i] = Item((ItemNameAvailable)stoi(itemName), stoi(output));
				}

				for (int i = 0; i < 9; i++)
				{
					file >> itemName;
					file >> output;

					m_player.GetInventory().GetItemInventoryBarArray()[i] = Item(itemName, stoi(output));
				}
			}
			if (output == "#POS")
			{
				while (output == "#POS")
				{
					Chunk* c = new Chunk();
					file >> output;
					c->chunkXPos = stoi(output);
					file >> output;
					c->chunkZPos = stoi(output);
					file >> output;

					int type = BlockId::AIR;
					int idx = 0;

					for (int i = 0; i < CHUNK_Y_SIZE; i++)
					{
						for (int j = 0; j < CHUNK_X_SIZE; j++)
						{
							for (int k = 0; k < CHUNK_Z_SIZE; k++)
							{
								if (idx == 0)
								{
									file >> output;
									if (output == "#POS")
										break;
									type = stoi(output);
									file >> output;
									idx = stoi(output);
								}

								c->SetBlock(j, i, k, type);
								idx--;
							}
							if (output == "#POS")
								break;
						}
						if (output == "#POS")
							break;
					}

					c->CheckHighestBlock();
					allChunks[c->chunkXPos][c->chunkZPos] = c;
				}
			}
		}

		file.close();

		Minecraft::ivec2 pos = GetPlayerChunkPos();

		for (int i = 0; i < CHUNK_ARRAY_SIZE; i++)
			for (int j = 0; j < CHUNK_ARRAY_SIZE; j++)
				renderedChunks[i][j] = GenerateNewChunk(pos.x - CHUNK_LOAD_RADIUS + i, pos.y - CHUNK_LOAD_RADIUS + j);
	}

	void World::CheckPlayerCenter()
	{
		ivec2 pos = GetPlayerChunkPos();

		ivec2 centerChunkPos(renderedChunks[CHUNK_LOAD_RADIUS][CHUNK_LOAD_RADIUS]->chunkXPos, renderedChunks[CHUNK_LOAD_RADIUS][CHUNK_LOAD_RADIUS]->chunkZPos);

		UpdateRenderedChunks(pos.x - centerChunkPos.x, pos.y - centerChunkPos.y);
	}

	Chunk* World::GenerateNewChunk(int x, int z)
	{
		Chunk* c = GetChunk(x, z);

		if (c == nullptr)
		{
			c = new Chunk(x, z, &m_noise, passes);
			allChunks[x][z] = c;
		}

		return c;
	}

	// Offset > 0 -> going into positive, shift all chunks -1 x/z

	void World::UpdateRenderedChunks(const int xOffset, const int zOffset)
	{
		if (xOffset < 0)
		{
			for (int i = CHUNK_ARRAY_SIZE - 1; i > 0; i--)
				for (int j = 0; j < CHUNK_ARRAY_SIZE; j++)
					renderedChunks[i][j] = renderedChunks[i - 1][j];

			for (int i = 0; i < CHUNK_ARRAY_SIZE; i++)
			{
				int xPos = renderedChunks[1][i]->chunkXPos - 1;
				int zPos = renderedChunks[1][i]->chunkZPos;

				renderedChunks[0][i] = GenerateNewChunk(xPos, zPos);
			}
		}

		if (zOffset < 0)
		{
			for (int i = CHUNK_ARRAY_SIZE - 1; i > 0; i--)
				for (int j = 0; j < CHUNK_ARRAY_SIZE; j++)
					renderedChunks[j][i] = renderedChunks[j][i - 1];

			for (int i = 0; i < CHUNK_ARRAY_SIZE; i++)
			{
				int xPos = renderedChunks[i][1]->chunkXPos;
				int zPos = renderedChunks[i][1]->chunkZPos - 1;

				renderedChunks[i][0] = GenerateNewChunk(xPos, zPos);
			}
		}

		if (xOffset > 0)
		{
			for (int i = 1; i < CHUNK_ARRAY_SIZE; i++)
				for (int j = 0; j < CHUNK_ARRAY_SIZE; j++)
					renderedChunks[i - 1][j] = renderedChunks[i][j];

			for (int i = 0; i < CHUNK_ARRAY_SIZE; i++)
			{
				int xPos = renderedChunks[CHUNK_ARRAY_SIZE - 2][i]->chunkXPos + 1;
				int zPos = renderedChunks[CHUNK_ARRAY_SIZE - 2][i]->chunkZPos;

				renderedChunks[CHUNK_ARRAY_SIZE - 1][i] = GenerateNewChunk(xPos, zPos);
			}
		}

		if (zOffset > 0)
		{
			for (int i = 1; i < CHUNK_ARRAY_SIZE; i++)
				for (int j = 0; j < CHUNK_ARRAY_SIZE; j++)
					renderedChunks[j][i - 1] = renderedChunks[j][i];

			for (int i = 0; i < CHUNK_ARRAY_SIZE; i++)
			{
				int xPos = renderedChunks[i][CHUNK_ARRAY_SIZE - 2]->chunkXPos;
				int zPos = renderedChunks[i][CHUNK_ARRAY_SIZE - 2]->chunkZPos + 1;

				renderedChunks[i][CHUNK_ARRAY_SIZE - 1] = GenerateNewChunk(xPos, zPos);
			}
		}
	}

	ivec2 World::GetPlayerChunkPos()
	{
		vec3 pos = m_player.GetPosition();

		ivec2 chunkPos;

		pos.x = pos.x < 0 ? pos.x - 15 : pos.x;
		pos.z = pos.z < 0 ? pos.z - 15 : pos.z;

		chunkPos.x = (int)pos.x / 16;
		chunkPos.y = (int)pos.z / 16;

		return chunkPos;
	}

	Chunk& World::GetPlayerChunk()
	{
		ivec2 pos = GetPlayerChunkPos();
		return *(allChunks.find(pos.x)->second.find(pos.y)->second);
	}

	Chunk* World::GetChunk(int chunkX, int chunkZ)
	{
		auto itx = allChunks.find(chunkX);

		if (itx != allChunks.end())
		{
			auto itz = itx->second.find(chunkZ);
			if (itz != itx->second.end())
				return itz->second;
		}
		return nullptr;
	}

	int World::GetBlock(vec3 pos)
	{
		return GetBlock((int)floorf(pos.x + 0.5f), (int)floorf(pos.y + 0.5f), (int)floorf(pos.z + 0.5f));
	}

	int World::GetBlock(int x, int y, int z)
	{
		int chunkX = x >= 0 ? (x / CHUNK_X_SIZE) : ((x + 1) / CHUNK_X_SIZE - 1);
		int chunkZ = z >= 0 ? (z / CHUNK_Z_SIZE) : ((z + 1) / CHUNK_Z_SIZE - 1);

		x = x - chunkX * CHUNK_X_SIZE;
		z = z - chunkZ * CHUNK_Z_SIZE;

		if (x == 16)
			x = 0;
		if (z == 16)
			z = 0;

		Chunk* chunk(this->GetChunk(chunkX, chunkZ));
		if (chunk != nullptr)
			return chunk->GetBlock(x, y, z);

		else return BlockId::NO_BLOCK;
	}

	void World::SetBlock(int const x, int const y, int const z, int const block)
	{
		int chunkX = x >= 0 ? (x / CHUNK_X_SIZE) : ((x + 1) / CHUNK_X_SIZE - 1);
		int chunkZ = z >= 0 ? (z / CHUNK_Z_SIZE) : ((z + 1) / CHUNK_Z_SIZE - 1);

		int blockX = x - chunkX * CHUNK_X_SIZE;
		int blockZ = z - chunkZ * CHUNK_Z_SIZE;

		if (blockX == 16)
			blockX = 0;
		if (blockZ == 16)
			blockZ = 0;

		Chunk* chunk(this->GetChunk(chunkX, chunkZ));
		int oldBlock = chunk->GetBlock(blockX, y, blockZ);
		if (chunk != nullptr)
			chunk->SetBlock(blockX, y, blockZ, block);

		int blockUnder = chunk->GetBlock(blockX, y - 1, blockZ);
		int blockOver = chunk->GetBlock(blockX, y + 1, blockZ);

		//  check for falling block
		if (BlockLookup::IsFalling(block))
		{
			if (!BlockLookup::IsSolid(blockUnder) && blockUnder != BlockId::TORCH)
				AddToFallingBlocks(x, y, z, block);
		}
		if (BlockLookup::IsSolid(oldBlock))
		{
			if (BlockLookup::IsFalling(blockOver))
				AddToFallingBlocks(x, y + 1, z, blockOver);
		}

		//  check for flowing water/lava
		float flowSpeed = BlockLookup::FlowingSpeedOf(block);
		if (flowSpeed > 0)
			AddToFlowingBlocks(x, y, z, block, flowSpeed);

		flowSpeed = BlockLookup::FlowingSpeedOf(blockOver);
		if (flowSpeed > 0)
			AddToFlowingBlocks(x, y + 1, z, blockOver, flowSpeed);

		if (BlockLookup::IsWaterfall(blockUnder))
			AddToFlowingBlocks(x, y - 1, z, blockUnder, BlockLookup::FlowingSpeedOf(blockUnder));

		int sideBlock = GetBlock(x + 1, y, z);
		flowSpeed = BlockLookup::FlowingSpeedOf(sideBlock);
		if (flowSpeed > 0)
			AddToFlowingBlocks(x + 1, y, z, sideBlock, flowSpeed);

		sideBlock = GetBlock(x - 1, y, z);
		flowSpeed = BlockLookup::FlowingSpeedOf(sideBlock);
		if (flowSpeed > 0)
			AddToFlowingBlocks(x - 1, y, z, sideBlock, flowSpeed);

		sideBlock = GetBlock(x, y, z + 1);
		flowSpeed = BlockLookup::FlowingSpeedOf(sideBlock);
		if (flowSpeed > 0)
			AddToFlowingBlocks(x, y, z + 1, sideBlock, flowSpeed);

		sideBlock = GetBlock(x, y, z - 1);
		flowSpeed = BlockLookup::FlowingSpeedOf(sideBlock);
		if (flowSpeed > 0)
			AddToFlowingBlocks(x, y, z - 1, sideBlock, flowSpeed);
	}

	void World::GenerateWorld()
	{
		for (auto xc : allChunks)
		{
			for (auto zc : xc.second)
			{
				delete zc.second;
			}
		}

		allChunks.clear();

		int xPos = -CHUNK_LOAD_RADIUS + GetPlayerChunkPos().x;
		int zPos = -CHUNK_LOAD_RADIUS + GetPlayerChunkPos().y;

		for (int i = 0; i < CHUNK_ARRAY_SIZE; i++, xPos++)
		{
			for (int j = 0; j < CHUNK_ARRAY_SIZE; j++, zPos++)
				renderedChunks[i][j] = new Chunk(xPos, zPos, &m_noise, passes);
			zPos = -CHUNK_LOAD_RADIUS + GetPlayerChunkPos().y;
		}

		for (const auto& xc : renderedChunks)
		{
			for (const auto& zc : xc)
			{
				allChunks[zc->chunkXPos][zc->chunkZPos] = zc;
			}
		}

		if (!m_creativeMode)
			m_player.SetPosition(Minecraft::vec3(0, GetChunk(0, 0)->GetHighestBlock(0, 0) + 1, 0));

		// TODO: generate enemy elsewhere
		//m_enemies.emplace_back(*this, vec3{ (4.f, GetChunk(0, 0)->GetHighestBlock(0, 0) + 1.f, 4.f) }, vec2{ 0.f, 0.f });
		//m_enemies[0].SetPosition(Minecraft::vec3(4.f, GetChunk(0, 0)->GetHighestBlock(0, 0) + 1.f, 4.f));
	}

	irrklang::ISoundEngine* World::GetSoundEngine()
	{
		return m_soundEngine;
	}

	void World::UpdateFallingBlockList(float dT)
	{
		for (size_t index = 0; index < m_blockWaitingToFall.size(); index++)
		{
			DynamicBlock& current = m_blockWaitingToFall[index];
			current.currValue += GRAVITY * dT;
			if (current.currValue < 0)
			{
				current.currValue = 0.f;
				m_fallingBlocks.push_back(current);

				SetBlock(current.startPos.x, (int)ceilf(current.currPos.y), current.startPos.z, BlockId::AIR);

				m_blockWaitingToFall.erase(m_blockWaitingToFall.begin() + index);
				index--;
			}
		}

		for (size_t index = 0; index < m_fallingBlocks.size(); index++)
		{
			DynamicBlock& current = m_fallingBlocks[index];
			current.currValue += GRAVITY * dT;
			current.currPos.y += current.currValue * dT;

			if (BlockLookup::IsSolid(GetBlock(current.startPos.x, (int)ceilf(current.currPos.y) - 1, current.startPos.z)))
			{
				SetBlock(current.startPos.x, (int)ceilf(current.currPos.y), current.startPos.z, current.currBlock);

				m_fallingBlocks.erase(m_fallingBlocks.begin() + index);
				index--;
			}
		}
	}

	void World::AddToFallingBlocks(int x, int y, int z, int block)
	{
		m_blockWaitingToFall.emplace_back(vec3{ (float)x, (float)y, (float)z }, ivec3{ x, y, z }, block, FALLING_BLOCK_WAIT);
	}

	void World::AddToFlowingBlocks(int x, int y, int z, int block, float flowSpeed)
	{
		m_flowingBlocks.emplace_front(vec3{ (float)x, (float)y, (float)z }, ivec3{ x, y, z }, block, flowSpeed);
	}

	void World::UpdateFlowingBlockList(float dT)
	{
		std::list<DynamicBlock>::iterator it = m_flowingBlocks.begin();
		while (it != m_flowingBlocks.end())
		{
			DynamicBlock& current = *it;
			current.currValue -= dT;
			if (current.currValue < 0)
			{
				current.currBlock = GetBlock(current.startPos.x, current.startPos.y, current.startPos.z);
				if (BlockLookup::IsWater(current.currBlock))
				{
					UpdateWaterBlock(current);
				}
				else if (BlockLookup::IsLava(current.currBlock))
				{
					UpdateLavaBlock(current);
				}
				// else block was change and no longer need update

				it = m_flowingBlocks.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	void World::UpdateWaterBlock(DynamicBlock const& current)
	{
		int over = GetBlock(current.startPos.x, current.startPos.y + 1, current.startPos.z);
		int front = GetBlock(current.startPos.x, current.startPos.y, current.startPos.z + 1);
		int right = GetBlock(current.startPos.x + 1, current.startPos.y, current.startPos.z);
		int back = GetBlock(current.startPos.x, current.startPos.y, current.startPos.z - 1);
		int left = GetBlock(current.startPos.x - 1, current.startPos.y, current.startPos.z);

		int self = current.currBlock;
		if (self != BlockId::WATER_SOURCE)
		{
			self = GetSelfWaterLevel(over, front, right, back, left);
			if (self != current.currBlock)
			{
				SetBlock(current.startPos.x, current.startPos.y, current.startPos.z, self);
				return;
			}
		}

		int under = GetBlock(current.startPos.x, current.startPos.y - 1, current.startPos.z);
		if (!BlockLookup::IsSolid(under))// under is empty
		{
			Overflow(current.startPos.x, current.startPos.y, current.startPos.z, current.currBlock, under, Face::B_010);
			return;
		}

		if (current.currBlock == BlockId::WATER_1)
			return;

		if (!BlockLookup::IsSolid(front))
			Overflow(current.startPos.x, current.startPos.y, current.startPos.z, current.currBlock, front, Face::F_001);
		if (!BlockLookup::IsSolid(right))
			Overflow(current.startPos.x, current.startPos.y, current.startPos.z, current.currBlock, right, Face::F_100);
		if (!BlockLookup::IsSolid(back))
			Overflow(current.startPos.x, current.startPos.y, current.startPos.z, current.currBlock, back, Face::B_001);
		if (!BlockLookup::IsSolid(left))
			Overflow(current.startPos.x, current.startPos.y, current.startPos.z, current.currBlock, left, Face::B_100);
	}

	int World::GetSelfWaterLevel(int blockOver, int blockBeside1, int blockBeside2, int blockBeside3, int blockBeside4) const
	{
		if (BlockLookup::IsWater(blockOver))
			return BlockId::WATERFALL;

		int self = BlockId::NO_BLOCK;

		if (BlockLookup::IsWater(blockBeside1))
			self = self > blockBeside1 ? self : blockBeside1;
		if (BlockLookup::IsWater(blockBeside2))
		{
			if (self == BlockId::WATER_SOURCE && blockBeside2 == BlockId::WATER_SOURCE)
				self++;
			else
				self = self > blockBeside2 ? self : blockBeside2;
		}
		if (BlockLookup::IsWater(blockBeside3))
		{
			if (self == BlockId::WATER_SOURCE && blockBeside3 == BlockId::WATER_SOURCE)
				self++;
			else
				self = self > blockBeside3 ? self : blockBeside3;
		}
		if (BlockLookup::IsWater(blockBeside4))
		{
			if (self == BlockId::WATER_SOURCE && blockBeside4 == BlockId::WATER_SOURCE)
				self++;
			else
				self = self > blockBeside4 ? self : blockBeside4;
		}

		switch (self)
		{
		case BlockId::NO_BLOCK:
		case BlockId::WATER_1:
			return BlockId::AIR;
		case BlockId::WATER_SOURCE:
			return BlockId::WATER_7;
		default:
			return self - 1;
		}
	}

	void World::Overflow(int sourceX, int sourceY, int sourceZ, int sourceBlock, int targetBlock, Face direction)
	{
		if (BlockLookup::IsSource(targetBlock)
			&& (BlockLookup::IsWater(sourceBlock) && BlockLookup::IsWater(targetBlock)
				|| BlockLookup::IsLava(sourceBlock) && BlockLookup::IsLava(targetBlock)))
			return;

		if (direction == Face::B_010)
		{
			SetBlock(sourceX, sourceY - 1, sourceZ, BlockLookup::IsWater(sourceBlock) ? BlockId::WATERFALL : BlockId::LAVA_WATERFALL);
			return;
		}

		sourceBlock -= BlockLookup::IsSource(sourceBlock) ? 2 : 1;
		if (targetBlock >= sourceBlock
			&& (BlockLookup::IsWater(sourceBlock) && BlockLookup::IsWater(targetBlock)
				|| BlockLookup::IsLava(sourceBlock) && BlockLookup::IsLava(targetBlock)))
			return;
		else if (targetBlock == BlockId::LAVA_SOURCE && BlockLookup::IsWater(sourceBlock))
			sourceBlock = BlockId::OBSIDIAN;
		else if (BlockLookup::IsWater(sourceBlock) && BlockLookup::IsLava(targetBlock)
			|| BlockLookup::IsLava(sourceBlock) && BlockLookup::IsWater(targetBlock))
			sourceBlock = BlockId::COBBLESTONE;

		switch (direction)
		{
		case Face::B_100:
			SetBlock(sourceX - 1, sourceY, sourceZ, sourceBlock);
			break;
		case Face::B_001:
			SetBlock(sourceX, sourceY, sourceZ - 1, sourceBlock);
			break;
		case Face::F_100:
			SetBlock(sourceX + 1, sourceY, sourceZ, sourceBlock);
			break;
		case Face::F_001:
			SetBlock(sourceX, sourceY, sourceZ + 1, sourceBlock);
			break;
		default:
			assert(false);
		}
	}

	void World::UpdateLavaBlock(DynamicBlock const& /*current*/)
	{
		/*
		int front = GetBlock(current.startPos.x, current.startPos.y, current.startPos.z + 1);
		int right = GetBlock(current.startPos.x + 1, current.startPos.y, current.startPos.z);
		int back = GetBlock(current.startPos.x, current.startPos.y, current.startPos.z - 1);
		int left = GetBlock(current.startPos.x - 1, current.startPos.y, current.startPos.z);
		*/
	}

	int World::GetHighestSpawnable(int x, int z)
	{
		int chunkX = x >= 0 ? (x / CHUNK_X_SIZE) : ((x + 1) / CHUNK_X_SIZE - 1);
		int chunkZ = z >= 0 ? (z / CHUNK_Z_SIZE) : ((z + 1) / CHUNK_Z_SIZE - 1);

		x = x - chunkX * CHUNK_X_SIZE;
		z = z - chunkZ * CHUNK_Z_SIZE;

		if (x == 16)
			x = 0;
		if (z == 16)
			z = 0;

		Chunk* chunk(this->GetChunk(chunkX, chunkZ));
		if (chunk != nullptr)
			return chunk->GetHighestBlock(x, z, 10);

		else return CHUNK_Z_SIZE;
	}

	void World::GenerateMob()
	{
		MobType type = MobType::NONE;
		vec3 spawnPoint = m_player.GetPosition();

		float randOffsetX = (rand() % 100 / (SPAWN_RADIUS - 10)) + 2;
		float randOffsetZ = (rand() % 100 / (SPAWN_RADIUS - 10)) + 2;

		int randType = rand() % 3;

		int negX = rand() % 2;
		int negZ = rand() % 2;
		spawnPoint.x += negX == 0 ? randOffsetX : -randOffsetX;
		spawnPoint.z += negZ == 0 ? randOffsetZ : -randOffsetZ;
		
		switch (randType)
		{
		case 0:
			type = MobType::COW;
			break;
		case 1:
			type = MobType::CREEPER;
			break;
		case 2:
			type = MobType::ZOMBIE;
			break;
		default:
			break;
		}
		
		int newY = GetHighestSpawnable((int)spawnPoint.x, (int)spawnPoint.z);
		spawnPoint.y = (float)newY;

		Enemy newEnemy(*this, spawnPoint, type);
		m_enemies.push_back(newEnemy);
	}
}