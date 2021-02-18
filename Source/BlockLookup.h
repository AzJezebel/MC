#pragma once

#include "BlockId.h"

namespace Minecraft
{
	class BlockLookup
	{
	public:
		static bool IsTargetable(int block);
		static bool IsSolid(int block);
		static bool IsFalling(int block);
		static bool IsWater(int block);
		static bool IsLava(int block);
		static bool IsWaterfall(int block);
		static bool IsSource(int block);
		
		static float FlowingSpeedOf(int block);
		static float BlockHardenessOf(int block);

	};
}
