#pragma once

#include "BlockId.h"
#include "Face.h"

namespace Minecraft
{
	struct TargetBlock
	{
		TargetBlock() 
		{
		}
		TargetBlock(int x, int y, int z) :
			x(x),
			y(y),
			z(z)
		{
		}

		bool operator==(const TargetBlock& other)const
		{
			return (x == other.x &&
				y == other.y &&
				z == other.z);
		}

		int x = 0;
		int y = 0;
		int z = 0;
		Face face = Face::INSIDE;
		int block = BlockId::NO_BLOCK;
	};
}