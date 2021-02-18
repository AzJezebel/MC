#include "Enemy.h"

#pragma warning( push )
#pragma warning( disable : 4201 )
#include <glm/gtc/matrix_transform.hpp>
#pragma warning( pop )

#include <stdlib.h>     //srand, rand
#include <time.h>       //time
#include "World.h"
#include "Trigo.h"

namespace Minecraft
{
	Enemy::Enemy(World& world, const vec3& position, MobType type, const vec2& rotation) :
		Creature(world, position, rotation, { 0.5f, 1.625f, 0.75f }),
		StateMachine(&Enemy::Idle)
	{
		m_type = type;
		switch (type)
		{
		case MobType::NONE:
			this->~Enemy();
			break;
		case MobType::COW:
			m_mesh = world.GetMesh().cow;
			break;
		case MobType::CREEPER:
			m_mesh = world.GetMesh().creeper;
			break;
		case MobType::ZOMBIE:
			m_mesh = world.GetMesh().zombie;
			break;
		default:
			break;
		}
	}

	mat4x4 Enemy::GetModelMatrix() const
	{
		mat4x4 model = glm::identity<mat4x4>();

		model = glm::translate(model, vec3(GetPosition().x, GetPosition().y, GetPosition().z));
		model = glm::rotate(model, GetRotation().y, vec3(0.f, 1.f, 0.f));

		return model;
	}

	void Enemy::KnockbackEnemy()
	{
		m_knocked = true;
		m_kbTimer = ATTACK_TIMER;
		EnterTempState(&Enemy::GetKnockedBack);
	}

	void Enemy::GetKnockedBack(const Player& target, float dT)
	{
		if (m_knocked)
		{
			vec3 dmgInput = vec3({ GetPosition().x, 0.f, GetPosition().z }) - vec3({ target.GetPosition().x, 0.f, target.GetPosition().z });
			glm::normalize(dmgInput);
			m_knockback = this->GetPosition() + (dmgInput * KNOCKBACK_MOD);
			m_knockback.y = KNOCKBACK_MOD;
			m_knocked = false;
		}

		if (m_knockback != vec3{ 0 })
		{
			m_kbTimer -= dT;
			this->EditPosition().x += m_knockback.x * dT;
			this->EditPosition().y += m_knockback.y * dT;
			this->EditPosition().z += m_knockback.z * dT;
		}
		if (m_kbTimer <= 0.f)
		{
			m_knockback = vec3{ 0 };
			ExitTempState();
		}
	}

	//Prolly trash
	void Enemy::KnockBackPlayer(Player& target, float dT)
	{
		if (m_knocked)
		{
			vec3 dmgInput = vec3(vec3{ target.GetPosition().x, 0.f, target.GetPosition().z } -vec3{ GetPosition().x, 0.f, GetPosition().z });
			glm::normalize(dmgInput);
			m_knockback = target.GetPosition() + (dmgInput * KNOCKBACK_MOD);
			m_knockback.y = KNOCKBACK_MOD;
			m_knocked = false;
		}
		if (m_knockback != vec3{ 0 })
		{
			m_kbTimer -= dT;
			target.EditPosition().x += m_knockback.x * dT;
			target.EditPosition().y += m_knockback.y * dT;
			target.EditPosition().z += m_knockback.z * dT;
		}
		if (m_kbTimer <= 0.f)
		{
			m_knockback = vec3{ 0 };
		}
	}

	vec2 Enemy::GetRandomDirection()
	{
		int negX = rand() % 2;
		int negZ = rand() % 2;
		float randX = negX == 0 ? (rand() % 100) / 100.f : -1.f * ((rand() % 100) / 100.f);
		float randZ = negZ == 0 ? (rand() % 100) / 100.f : -1.f * ((rand() % 100) / 100.f);

		return vec2(normalize(vec2(randX, randZ) * MOB_SPEED));
	}

	void Enemy::Idle(const Player& target, float dT)
	{
		m_b4Rand -= dT;
		m_eating -= dT;

		if (IsNear(target.GetPosition()) && m_type != MobType::COW)
			ReplaceState(&Enemy::Aggro);

		if (m_attacked)
		{
			m_timer = FLEE_TIME;
			ReplaceState(&Enemy::Flee);
		}

		if (m_eating <= 0.f && m_type == MobType::COW && m_world->GetBlock((int)GetPosition().x, (int)GetPosition().y, (int)GetPosition().z) == BlockId::GRASS)
		{
			TryToEat();
		}

		if (m_b4Rand <= 0.f)
		{
			m_toward = GetRandomDirection();
			m_b4Rand = (rand() % 100 / (MAX_TIME_BEFORE_RAND - MIN_TIME_BEFORE_RAND)) + MIN_TIME_BEFORE_RAND;
		}

		Rotate(m_toward);

		//TODO Use : AcceptableDrop and Why
		if (!Walk(vec2(m_toward), dT))
			Jump();
	}

	void Enemy::Flee(const Player& target, float dT)
	{
		m_timer -= dT;

		m_toward = (normalize(vec2(target.GetPosition().x + this->GetPosition().x, target.GetPosition().z + this->GetPosition().z)) * FLEE_SPEED);

		Rotate(m_toward);

		if (!Walk(m_toward, dT))
			Jump();

		if (m_timer < 0)
		{
			m_attacked = false;
			ReplaceState(&Enemy::Idle);
		}
	}

	void Enemy::Aggro(const Player& target, float dT)
	{
		m_toward = (normalize(vec2(target.GetPosition().x - this->GetPosition().x, target.GetPosition().z - this->GetPosition().z)) * MOB_SPEED);

		Rotate(m_toward);

		//TODO Use : AcceptableDrop and Why
		if (!Walk(m_toward, dT))
			Jump();

		if (!IsNear(target.GetPosition()))
			ReplaceState(&Enemy::Idle);

		if (IsSuperNear(target.GetPosition()))
		{
			if (m_type == MobType::CREEPER)
			{
				EnterTempState(&Enemy::BlowUp);
				m_timer = TIME_NEEDED_TO_EXPLODE;
			}

			if (m_type == MobType::ZOMBIE)
			{
				EnterTempState(&Enemy::DealToPlayer);
				m_timer = ZOMBIE_ATK_SPEED;
			}
		}
	}

	void Enemy::ApplyDamage(const Player& target)
	{
		float damage = BLOWUP_DMG / distance(target.GetPosition(), this->GetPosition());
		m_world->GetPlayer().SetLife(target.GetLife() - damage);
	}

	void Enemy::BlowUp(const Player& target, float dT)
	{
		if (m_isCreeperExploding == false)
		{
			m_isCreeperExploding = true;

			m_creeperExplosionSound = m_world->GetSoundEngine()->play3D("assets/sounds/Minecraft_creeper_explosion_sound_effect.ogg",
				irrklang::vec3df(GetPosition().x, GetPosition().y, GetPosition().z), false, false, true);
		}

		m_timer -= dT;

		if (!IsSuperNear(target.GetPosition()) && m_timer > TIME_TO_FLEE)
		{
			m_creeperExplosionSound->stop(); // TODO: Check for nullptr (it crashed when multiple creepers exploded at the same time)
			m_isCreeperExploding = false;
		}

		if (m_timer < 0)
		{
			ApplyDamage(target);
			DestroyNearby();
			this->SetLife(0);
			//m_creeperExplosionSound->drop();
		}
	}

	void Enemy::DealToPlayer(const Player& target, float dT)
	{
		m_timer -= dT;

		if (!IsSuperNear(target.GetPosition()) && m_timer > TIME_TO_FLEE)
			ExitTempState();

		if (m_timer < 0)
		{
			m_world->GetPlayer().SetLife(target.GetLife() - ZOMBIE_DMG);
			KnockBackPlayer(m_world->GetPlayer(), dT);
			m_timer = ZOMBIE_ATK_SPEED;
		}
	}

	bool Enemy::IsNear(vec3 targetPos)
	{
		return (distance(targetPos, this->GetPosition()) < AGGRO_RANGE);
	}

	bool Enemy::IsSuperNear(vec3 targetPos)
	{
		return (distance(targetPos, this->GetPosition()) < BLOWUP_RANGE);
	}
	void Enemy::DestroyNearby()
	{
		vec3 centralPos = this->GetPosition();

		for (int xRange = -WIDTH_RANGE; xRange <= WIDTH_RANGE; xRange++)
		{
			for (int zRange = -WIDTH_RANGE; zRange <= WIDTH_RANGE; zRange++)
			{
				int depth = xRange >= 0 ? xRange : -xRange;
				depth += zRange >= 0 ? zRange : -zRange;

				for (int yRange = -(HEIGHT_RANGE)+depth; yRange <= HEIGHT_RANGE; yRange++)
				{
					m_world->SetBlock((int)centralPos.x + xRange, (int)centralPos.y + yRange, (int)centralPos.z + zRange, BlockId::AIR);
				}
			}
		}
	}

	void Enemy::Rotate(vec2 rot)
	{
		float newY;
		if (rot.y == 0)
			newY = Trigo::QUARTER_CIRCLE_RAD;
		else
			newY = atanf(rot.x / rot.y);

		if (rot.y < 0)
			newY += Trigo::HALF_CIRCLE_RAD;

		this->SetRotation(0, newY);
	}

	void Enemy::TryToEat()
	{
		if (m_type == MobType::COW)
		{
			m_world->GetSoundEngine()->play3D("assets/sounds/Minecraft_Cow_Sound.ogg",
				irrklang::vec3df(GetPosition().x, GetPosition().y, GetPosition().z), false, false, true);
		}
		
		m_toward = vec2{ 0 };
		m_world->SetBlock((int)GetPosition().x, (int)GetPosition().y, (int)GetPosition().z, BlockId::DIRT);
		m_eating = EATING_COOLDOWN;
	}
}