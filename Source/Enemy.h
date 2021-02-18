#pragma once

#include <irrKlang.h>

#include "Player.h"
#include "StateMachine.h"

#include "Mesh.h"

namespace Minecraft
{
	class Enemy;
	typedef void (Enemy::* EnemyState)(const Player&, float);

	enum class MobType
	{
		NONE,
		COW,
		CREEPER, 
		ZOMBIE
	};

	class Enemy : public Creature, public StateMachine<Enemy, EnemyState>
	{
	public:
		Enemy(World& world, const vec3& position, MobType type, const vec2& rotation = vec3({0.f, 0.f, 0.f}));
		~Enemy() = default;

		mat4x4 GetModelMatrix() const;

		void KnockbackEnemy();

		bool& EditAttacked() { return m_attacked; }

		Mesh::Entity m_mesh;

	private:
		vec2 GetRandomDirection();

		void Idle(const Player& target, float dT);
		void Flee(const Player& target, float dT);
		void Aggro(const Player& target, float dT);

		void GetKnockedBack(const Player& target, float dT);
		void KnockBackPlayer(Player& target, float dT);

		void ApplyDamage(const Player& target);
		void BlowUp(const Player& target, float dT);
		void DealToPlayer(const Player& target, float dT);

		bool IsNear(vec3 targetPos);
		bool IsSuperNear(vec3 targetPos);

		void DestroyNearby();

		void Rotate(vec2 rot);

		void TryToEat();

		vec3 m_knockback = vec3{ 0 };
		vec2 m_toward = vec2(0.f, 0.f);
		float m_kbTimer = 0.f;
		float m_timer = 0.f;
		float m_b4Rand = 0.f;
		float m_eating = 0.f;

		bool m_knocked = false;
		bool m_isCreeperExploding = false;
		irrklang::ISound* m_creeperExplosionSound;
		MobType m_type;
		bool m_attacked = false;

		//Sound
	};
}
