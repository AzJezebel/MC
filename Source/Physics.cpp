#include "Physics.h"

#include "World.h"
#include "Trigo.h"
#include "BlockLookup.h"

namespace Minecraft
{
	Physics::Physics(World& world) :
		m_world(world)
	{
	}

	vec3 Physics::CheckNewLocation(vec3 currPos, const vec3& collisionBox, const vec3& requestPos, float accetableDrop, Why* why) const
	{
		// TODO: Test jump under an obstacle
		// TODO: Test in all 3 negative chunk [(+, -), (-, +) and (-, -)]
		CheckNewLocationY(currPos, requestPos, currPos.y < requestPos.y ? collisionBox.y : 0.f);

		if (currPos.x != requestPos.x)
			CheckNewLocationX(currPos, requestPos, currPos.x < requestPos.x ? (collisionBox.x / 2.f) : -(collisionBox.x / 2.f), collisionBox.y, accetableDrop, why);

		if (currPos.z != requestPos.z)
			CheckNewLocationZ(currPos, requestPos, currPos.z < requestPos.z ? (collisionBox.z / 2.f) : -(collisionBox.z / 2.f), collisionBox.y, accetableDrop, why);

		return currPos;
	}

	void Physics::CheckNewLocationX(vec3& currentPos, const vec3& requestPos, float padding, float height, float accetableDrop, Why* why) const
	{
		int current = (int)floorf(currentPos.x + 0.5f);
		int const target = (int)floorf(requestPos.x + padding + 0.5f);
		int const bottom = (int)floorf(currentPos.y + 0.5f);
		int const top = (int)floorf(currentPos.y + height + 0.5f);
		int const z = (int)floorf(currentPos.z + 0.5f);

		currentPos.x = requestPos.x;
		if (why != nullptr)
			*why = Why::VALID_LOCATION; // assume

		while (current != target)
		{
			bool goingForward = current < target;
			if (goingForward)
				current++;
			else
				current--;

			for (int body = bottom; body <= top; body++)
			{
				int currBlock = m_world.GetBlock(current, body, z);
				if (BlockLookup::IsSolid(currBlock))
				{
					currentPos.x = goingForward ? current - .5f - padding : current + .5f - padding;
					if (why != nullptr)
						*why = Why::BLOCK_IN_THE_WAY;
					return;
				}
			}

			if (accetableDrop >= 0.f)
			{
				bool valid = false;
				for (int drop = 0; drop <= accetableDrop; drop++)
				{
					int currBlock = m_world.GetBlock(current, bottom - 1 - drop, z);
					if (BlockLookup::IsSolid(currBlock))
					{
						valid = true;
						break;
					}
				}
				if (!valid && (goingForward ? currentPos.x > current - .5005f : currentPos.x < current + .5005f))
				{
					currentPos.x = goingForward ? current - .5005f : current + .5005f;
					if (why != nullptr)
						*why = Why::UNACCEPTABLE_DROP;
					break;
				}
			}
		}
	}

	void Physics::CheckNewLocationY(vec3& currentPos, const vec3& requestPos, float padding) const
	{
		float current = floorf(currentPos.y + padding + 0.5f);
		float target = floorf(requestPos.y + padding + 0.5f);

		currentPos.y = requestPos.y;

		while (current != target)
		{
			bool goingUp = current < target;
			if (goingUp)
				current++;
			else
				current--;

			int currBlock = m_world.GetBlock({ currentPos.x, current, currentPos.z });
			if (BlockLookup::IsSolid(currBlock))
			{
				currentPos.y = goingUp ? current - .5f - padding : current + .5f - padding;
				break;
			}
		}
	}

	void Physics::CheckNewLocationZ(vec3& currentPos, const vec3& requestPos, float padding, float height, float accetableDrop, Why* why) const
	{
		int current = (int)floorf(currentPos.z + 0.5f);
		int const target = (int)floorf(requestPos.z + padding + 0.5f);
		int const bottom = (int)floorf(currentPos.y + 0.5f);
		int const top = (int)floorf(currentPos.y + height + 0.5f);
		int const x = (int)floorf(currentPos.x + 0.5f);

		currentPos.z = requestPos.z;

		while (current != target)
		{
			bool goingForward = current < target;
			if (goingForward)
				current++;
			else
				current--;

			for (int body = bottom; body <= top; body++)
			{
				int currBlock = m_world.GetBlock(x, body, current);
				if (BlockLookup::IsSolid(currBlock))
				{
					currentPos.z = goingForward ? current - .5f - padding : current + .5f - padding;
					if (why != nullptr)
						*why = *why == Why::UNACCEPTABLE_DROP ? Why::DROP_AND_BLOCK : Why::BLOCK_IN_THE_WAY; // TODO: bitwise
					return;
				}
			}

			if (accetableDrop >= 0.f)
			{
				bool valid = false;
				for (int drop = 0; drop <= accetableDrop; drop++)
				{
					int currBlock = m_world.GetBlock(x, bottom - 1 - drop, current);
					if (BlockLookup::IsSolid(currBlock))
					{
						valid = true;
						break;
					}
				}
				if (!valid && (goingForward ? currentPos.z > current - .5005f : currentPos.z < current + .5005f))
				{
					currentPos.z = goingForward ? current - .5005f : current + .5005f;
					if (why != nullptr)
						*why = *why == Why::BLOCK_IN_THE_WAY ? Why::DROP_AND_BLOCK : Why::UNACCEPTABLE_DROP; // TODO: bitwise
					break;
				}
			}
		}
	}

	TargetBlock Physics::GetTargetedBlock(vec3 const& origin, vec2 const& angles, float maxDistance) const
	{
		TargetBlock target = TargetBlock{
			(int)floorf(origin.x + 0.5f),
			(int)floorf(origin.y + 0.5f),
			(int)floorf(origin.z + 0.5f)
		};

		target.block = m_world.GetBlock(target.x, target.y, target.z);
		if (BlockLookup::IsTargetable(target.block))
			return target;

		float temp = cosf(angles.y);
		vec3 ray = vec3{
			sinf(angles.x + Trigo::HALF_CIRCLE_RAD) * temp,
			sinf(angles.y),
			cosf(angles.x + Trigo::HALF_CIRCLE_RAD) * temp
		};

		ivec3 delta = ivec3{
			ray.x < 0 ? -1 : 1,
			ray.y < 0 ? -1 : 1,
			ray.z < 0 ? -1 : 1
		};

		vec3 plane = vec3{
			floorf(origin.x + .5f) + (ray.x < 0 ? -.5f : .5f),
			floorf(origin.y + .5f) + (ray.y < 0 ? -.5f : .5f),
			floorf(origin.z + .5f) + (ray.z < 0 ? -.5f : .5f)
		};

		vec3 distance = vec3{
			ray.x == 0 ? maxDistance + 1.f : GetDistance(origin.x, ray.x, plane.x),
			ray.y == 0 ? maxDistance + 1.f : GetDistance(origin.y, ray.y, plane.y),
			ray.z == 0 ? maxDistance + 1.f : GetDistance(origin.z, ray.z, plane.z),
		};

		while (distance.x < maxDistance || distance.y < maxDistance || distance.z < maxDistance)
		{
			if (distance.x < distance.y && distance.x < distance.z)
			{
				target.x += delta.x;
				target.face = delta.x < 0 ? Face::F_100 : Face::B_100;

				target.block = m_world.GetBlock(target.x, target.y, target.z);
				if (BlockLookup::IsTargetable(target.block))
					return target;

				plane.x += ray.x < 0 ? -1.f : 1.f;
				distance.x = GetDistance(origin.x, ray.x, plane.x);
			}
			else if (distance.y < distance.z)
			{
				target.y += delta.y;
				target.face = delta.y < 0 ? Face::F_010 : Face::B_010;

				target.block = m_world.GetBlock(target.x, target.y, target.z);
				if (BlockLookup::IsTargetable(target.block))
					return target;

				plane.y += ray.y < 0 ? -1.f : 1.f;
				distance.y = GetDistance(origin.y, ray.y, plane.y);
			}
			else
			{
				target.z += delta.z;
				target.face = delta.z < 0 ? Face::F_001 : Face::B_001;

				target.block = m_world.GetBlock(target.x, target.y, target.z);
				if (BlockLookup::IsTargetable(target.block))
					return target;

				plane.z += ray.z < 0 ? -1.f : 1.f;
				distance.z = GetDistance(origin.z, ray.z, plane.z);
			}
		}

		return target;
	}
}