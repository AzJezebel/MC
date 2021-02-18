#include "BlockLookup.h"

namespace Minecraft
{
	bool BlockLookup::IsTargetable(int block)
	{
		switch (block)
		{
		case BlockId::AIR:
		case BlockId::WATERFALL:
		case BlockId::WATER_7:
		case BlockId::WATER_6:
		case BlockId::WATER_5:
		case BlockId::WATER_4:
		case BlockId::WATER_3:
		case BlockId::WATER_2:
		case BlockId::WATER_1:
		case BlockId::LAVA_WATERFALL:
		case BlockId::LAVA_3:
		case BlockId::LAVA_2:
		case BlockId::LAVA_1:
			return false;
		default:
			return true;
		}
	}

	bool BlockLookup::IsSolid(int block)
	{
		switch (block)
		{
		case BlockId::AIR:
		case BlockId::WATER_SOURCE:
		case BlockId::WATERFALL:
		case BlockId::WATER_7:
		case BlockId::WATER_6:
		case BlockId::WATER_5:
		case BlockId::WATER_4:
		case BlockId::WATER_3:
		case BlockId::WATER_2:
		case BlockId::WATER_1:
		case BlockId::TORCH:
		case BlockId::LAVA_SOURCE:
		case BlockId::LAVA_WATERFALL:
		case BlockId::LAVA_3:
		case BlockId::LAVA_2:
		case BlockId::LAVA_1:
			return false;
		default:
			return true;
		}
	}
	bool BlockLookup::IsFalling(int block)
	{
		switch (block)
		{
		case BlockId::SAND:
		case BlockId::GRAVEL:
			return true;
		default:
			return false;
		}
	}
	bool BlockLookup::IsWater(int block)
	{
		switch (block)
		{
		case BlockId::WATER_SOURCE:
		case BlockId::WATERFALL:
		case BlockId::WATER_7:
		case BlockId::WATER_6:
		case BlockId::WATER_5:
		case BlockId::WATER_4:
		case BlockId::WATER_3:
		case BlockId::WATER_2:
		case BlockId::WATER_1:
			return true;
		default:
			return false;
		}
	}
	bool BlockLookup::IsLava(int block)
	{
		switch (block)
		{
		case BlockId::LAVA_SOURCE:
		case BlockId::LAVA_WATERFALL:
		case BlockId::LAVA_3:
		case BlockId::LAVA_2:
		case BlockId::LAVA_1:
			return true;
		default:
			return false;
		}
	}
	bool BlockLookup::IsWaterfall(int block)
	{
		switch (block)
		{
		case BlockId::WATERFALL:
		case BlockId::LAVA_WATERFALL:
			return true;
		default:
			return false;
		}
	}
	bool BlockLookup::IsSource(int block)
	{
		switch (block)
		{
		case BlockId::WATER_SOURCE:
		case BlockId::LAVA_SOURCE:
			return true;
		default:
			return false;
		}
	}
	float BlockLookup::FlowingSpeedOf(int block)
	{
		switch (block)
		{
		case BlockId::WATER_SOURCE:
		case BlockId::WATERFALL:
		case BlockId::WATER_7:
		case BlockId::WATER_6:
		case BlockId::WATER_5:
		case BlockId::WATER_4:
		case BlockId::WATER_3:
		case BlockId::WATER_2:
		case BlockId::WATER_1:
			return .5f;
		case BlockId::LAVA_SOURCE:
		case BlockId::LAVA_WATERFALL:
		case BlockId::LAVA_3:
		case BlockId::LAVA_2:
		case BlockId::LAVA_1:
			return 1.f;
		default:
			return 0.f;
		}
	}
	float BlockLookup::BlockHardenessOf(int block)
	{
		switch (block)
		{
		case BlockId::SAND:
		case BlockId::DIRT:
		case BlockId::GRASS:
			return .8f;
		case BlockId::OAK_LOG:
			return .5f;
		case BlockId::STONE:
		case BlockId::DIAMOND:
			return 2.f;
		default:
			return 0.f;
		}
	}
}
