#pragma once

#include "TargetBlock.h"
#include "glmType.h"

#include <vector>

namespace Minecraft
{
	enum class Why
	{
		VALID_LOCATION,
		UNACCEPTABLE_DROP,
		BLOCK_IN_THE_WAY,
		DROP_AND_BLOCK
	};

	class World;
	class Creature;
	class Physics
	{
	public:
		Physics() = delete;
		Physics(World& world);
		~Physics() = default;

		vec3 CheckNewLocation(vec3 currPos, const vec3& collisionBox, const vec3& requestPos, float accetableDrop = -1.f, Why* why = nullptr) const;
		TargetBlock GetTargetedBlock(vec3 const& origin, vec2 const& angles, float distance) const;

	private:
		void CheckNewLocationX(vec3& currentPos, const vec3& requestPos, float padding, float height, float accetableDrop, Why* why) const;
		void CheckNewLocationY(vec3& currentPos, const vec3& requestPos, float padding) const;
		void CheckNewLocationZ(vec3& currentPos, const vec3& requestPos, float padding, float height, float accetableDrop, Why* why) const;

		float GetDistance(float start, float step, float stop) const { return (stop - start) / step; }

		World& m_world;
	};
}
