#pragma once

#include "Physics.h"
#include "Inventory.h"
#include "define.h"

namespace Minecraft
{
	class Creature
	{
	public:
		Creature(World& world, const vec3& position, const vec2& rotation, const vec3& collisionBox);
		~Creature() = default;

		bool Walk(vec2 const& speed, float dT, float accetableDrop = -1, Why* why = nullptr);
		bool Fly(vec3 const& speed, float dT);
		bool Swim(vec3 const& speed, float dT);
		void Jump();
		void Drown(float dT);

		virtual void Attack(); // TODO: think : Does creature need attack ? virtual ?
		int GetDamage() const { return m_damage; }
		void TakeFallDamage();

		vec3 const& GetCollisionBox() const { return m_collisionBox; }
		vec3 const& GetPosition() const { return m_position; }
		vec3& EditPosition() { return m_position; }
		vec2 const& GetRotation() const { return m_rotation; }
		float GetLife() const { return m_life; }

		void SetLife(float newLife) { m_life = newLife; }

		void SetPosition(const vec3& newPos) { m_position = newPos; }
		void SetRotation(float x, float y) { m_rotation.x = x; m_rotation.y = y; }
		void SetRotation(float x) { m_rotation.x = x; }

		bool& EditInWater() { return m_inWater; }

		bool& EditUnderWater() { return m_underWater; }

		float& EditBreath() { return m_breath; }

		bool IsFalling() { return m_falling; }

			Inventory& GetInventory();
		Inventory const& GetInventory() const { return m_inventory; }
	protected:
		World* m_world;			//Using pointer because ref cant be copied
		Inventory m_inventory;

	private:
		vec3 m_position;
		vec2 m_rotation;
		vec3 m_collisionBox;

		float m_life = MAX_LIFE;
		int m_damage = BASE_DAMAGE;

		float m_vertVelocity = 0;
		float m_fallStart = -1;
		float m_breath = 0.f;
		float m_tick = 0.f;

		bool m_inWater;
		bool m_underWater;
		bool m_falling = false;
	};
}