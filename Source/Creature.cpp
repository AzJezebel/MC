#include "Creature.h"

#include "World.h"

namespace Minecraft
{
	Creature::Creature(World& world, const vec3& position, const vec2& rotation, const vec3& collisionBox) :
		m_world(&world),
		m_position(position),
		m_rotation(rotation),
		m_collisionBox(collisionBox),
		m_inventory(Inventory())
	{
	}

	bool Creature::Walk(vec2 const& speed, float dT, float accetableDrop, Why* why)
	{
		m_falling = m_vertVelocity < 0;

		if (m_vertVelocity != 0)
		{
			accetableDrop = -1;
		}

		vec3 requestPos = m_position;

		if (!m_world->InCreativeMode())
		{
			requestPos.x += speed.x * dT;
			requestPos.z += speed.y * dT;

			m_vertVelocity += GRAVITY * dT;
			requestPos.y += m_vertVelocity * dT;

			m_position = m_world->physics.CheckNewLocation(m_position, m_collisionBox, requestPos, accetableDrop, why);
		}

		if (m_position.y != requestPos.y) //Means onGround
		{
			m_vertVelocity = 0.f;

			if (m_fallStart >= 0.f && !m_inWater)
				TakeFallDamage();
		}

		if (!m_world->InCreativeMode() && !m_inWater && m_vertVelocity < 0.f && m_fallStart < 0.f)
		{
			m_vertVelocity = 0;
			m_fallStart = m_position.y;
		}

		return (m_position.x == requestPos.x &&
			m_position.z == requestPos.z);
	}

	bool Creature::Fly(vec3 const& speed, float dT)
	{
		m_position += speed * dT;
		m_fallStart = -1;
		// TODO: m_position = m_world.m_phys.CheckNewLocation(*this, requestPos);
		return true;
	}

	bool Creature::Swim(vec3 const& speed, float dT)
	{
		m_fallStart = -1;
		vec3 requestPos = m_position;

		requestPos.x += speed.x * dT;
		requestPos.z += speed.z * dT;

		m_vertVelocity += abs(speed.y) >= 1 ? speed.y * dT : GRAVITY / 9.f * dT;
		requestPos.y += m_vertVelocity * dT;

		m_position = m_world->physics.CheckNewLocation(m_position, m_collisionBox, requestPos);

		if (m_position.y != requestPos.y)
			m_vertVelocity = 0.f;

		return (m_position.x == requestPos.x &&
			m_position.y == requestPos.y &&
			m_position.z == requestPos.z);
	}

	void Minecraft::Creature::Jump()
	{
		//TODO Change jump mechanic if in water
		if (m_vertVelocity == 0)
			m_vertVelocity += !m_inWater ? JUMP_VEL : JUMP_VEL / 4.f;
	}

	void Creature::Drown(float dT)
	{
		m_tick -= dT;
		m_breath -= dT;
		if (m_breath <= 0 && m_tick <= 0)
		{
			this->SetLife(this->GetLife() - 1);
			m_tick = 1.f;
		}
	}
	void Minecraft::Creature::Attack()
	{
		//TODO
	}

	void Creature::TakeFallDamage()
	{
		//Damage you take = Distance fallen(in blocks)-3
		int damage = 0;

		damage = ((int)floorf(m_fallStart - m_position.y)) - 3;

		if (damage > 0)
			m_life -= damage;

		m_fallStart = -1;
	}
	Inventory& Creature::GetInventory()
	{
		return m_inventory;
	}
}