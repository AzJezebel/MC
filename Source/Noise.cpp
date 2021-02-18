#include "Noise.h"

#pragma warning( push )
#pragma warning( disable : 4701 ) // potentially uninitialized local variable 'Result' used	(glm) func_common.inl L-99
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#pragma warning( pop )
#include <time.h>

int Noise::maxHeight = 95;
int Noise::minHeight = 35;
float Noise::frequency = 1.4f;
float Noise::redistribution = 1.3f;
float Noise::cavesXZFrequency = 6.f;
float Noise::cavesYFrequency = 9.05f;
float Noise::cavesThreshold = 0.75f;
int Noise::seed = 0;
bool Noise::offsetsGenerated = false;
int Noise::xOffset = 0;
int Noise::zOffset = 0;

int Noise::GetHeight(int x, int z, int chunkX, int chunkZ)
{
	// Simple hacky way of implementing a seed without totally randomizing
	// Use the seed to generate a random offset on first iteration

	if (!offsetsGenerated)
	{
		srand(seed);
		xOffset = rand() % MAX_NOISE_OFFSET;
		zOffset = rand() % MAX_NOISE_OFFSET;
		offsetsGenerated = true;
	}

	float xSample = (float)(chunkX * CHUNK_X_SIZE + x + xOffset) / NOISE_ZOOM_LEVEL;
	float zSample = (float)(chunkZ * CHUNK_Z_SIZE + z + zOffset) / NOISE_ZOOM_LEVEL;

	float noiseOctave1 = 1.f * GetNoise(frequency * xSample, frequency * zSample);
	float noiseOctave2 = 0.5f * GetNoise(2 * frequency * xSample, 2 * frequency * zSample) * noiseOctave1;
	float noiseOctave3 = 0.25f * GetNoise(4 * frequency * xSample, 4 * frequency * zSample) * (noiseOctave1 + noiseOctave2);

	float noiseSample = noiseOctave1 + noiseOctave2 + noiseOctave3;

	noiseSample = powf(noiseSample, redistribution);
	int heightChangeSample = (int)(noiseSample * (maxHeight - minHeight)) + minHeight; // Map 0 - 1 to min - max
	return glm::clamp(heightChangeSample, 0, 255);
}

bool Noise::GetCave(int x, int y, int z, int chunkX, int chunkZ)
{
	float xSample = (float)(chunkX * CHUNK_X_SIZE + x) / NOISE_ZOOM_LEVEL;
	float ySample = (float)y / NOISE_ZOOM_LEVEL;
	float zSample = (float)(chunkZ * CHUNK_Z_SIZE + z) / NOISE_ZOOM_LEVEL;

	float noiseSample = 1.f * GetNoise(cavesXZFrequency * xSample, cavesYFrequency * ySample, cavesXZFrequency * zSample);

	return noiseSample >= cavesThreshold;
}

bool Noise::GetTree(int x, int z, int chunkX, int chunkZ)
{
	// I know, magic numbers etc, no time gotta go fast xd
	float xSample = (float)(chunkX * CHUNK_X_SIZE + x) / NOISE_ZOOM_LEVEL;
	float zSample = (float)(chunkZ * CHUNK_Z_SIZE + z) / NOISE_ZOOM_LEVEL;

	float noiseSample = GetNoise(50.f * xSample, 50.f * zSample);

	int heightChangeSample = (int)(noiseSample * 255.f);

	return heightChangeSample >= 215;
}

BiomeType Noise::GetBiome(int height)
{
	if (height < CHUNK_WATER_LEVEL)
		return BiomeType::OCEAN;
	else if (height < CHUNK_WATER_LEVEL + 7)
		return BiomeType::DESERT;
	else if (height < CHUNK_WATER_LEVEL + 25)
		return BiomeType::PLAINS;
	else if (height < CHUNK_WATER_LEVEL + 35)
		return BiomeType::FOREST;
	else
		return BiomeType::MOUNTAIN;
}

float Noise::GetNoise(float xSample, float zSample)
{
	return (1 + glm::perlin(glm::vec2(xSample, zSample))) / 2.f; // Map to 0 - 1 instead of -1 - 1
}

float Noise::GetNoise(float xSample, float ySample, float zSample)
{
	return (1 + glm::perlin(glm::vec3(xSample, ySample, zSample))) / 2.f; // Map to 0 - 1 instead of -1 - 1
}

void Noise::ChangeSeed(int newSeed)
{
	if (newSeed == 0)
		newSeed = (int)time(NULL);

	seed = newSeed;
	offsetsGenerated = false;
}