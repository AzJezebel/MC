#pragma once

#include "define.h"
#include "Biome.h"

class Noise
{
public:
	static int GetHeight(int x, int z, int chunkX, int chunkZ);
	bool GetCave(int x, int y, int z, int chunkX, int chunkZ);
	static BiomeType GetBiome(int height);
	static float GetNoise(float xSample, float zSample);
	float GetNoise(float xSample, float ySample, float zSample);
	void ChangeSeed(int newSeed);
	static bool GetTree(int x, int z, int chunkX, int chunkZ);

	static int minHeight;
	static int maxHeight;
	static float frequency;
	static float redistribution;
	static float cavesXZFrequency;
	static float cavesYFrequency;
	static float cavesThreshold;

	static bool offsetsGenerated;
	static int seed;
	static int xOffset;
	static int zOffset;

};