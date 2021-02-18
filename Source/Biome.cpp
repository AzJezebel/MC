#include "Biome.h"

const Minecraft::Biome& Minecraft::GetStaticBiome(BiomeType type)
{
	switch (type)
	{
	default:
	case BiomeType::PLAINS:
		return Minecraft::s_PLAINS;
	case BiomeType::OCEAN:
		return Minecraft::s_OCEAN;
	case BiomeType::MOUNTAIN:
		return Minecraft::s_MOUNTAIN;
	case BiomeType::FOREST:
		return Minecraft::s_FOREST;
	case BiomeType::DESERT:
		return Minecraft::s_DESERT;
	}
}