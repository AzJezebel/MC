#pragma once

#include "BlockId.h"
#include "glmType.h"

namespace Minecraft
{
	struct DynamicBlock
	{
		DynamicBlock(vec3 pos, ivec3 posInt, int block, float value = 0.f) :
			currPos(pos), startPos(posInt), currBlock(block), currValue(value)
		{
		}

		vec3 currPos;
		ivec3 startPos;
		int currBlock;
		float currValue;
	};
}