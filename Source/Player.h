#pragma once

#include "BlockId.h"
#include "Creature.h"
#include "InputManager.h"

#include <glm/gtx/vector_angle.inl> // angle

namespace Minecraft
{
	class Player : public Creature
	{
	public:
		Player() = delete;
		Player(World& world, const vec3& position, const vec2& rotation);
		~Player() = default;		// TODO: In Minecraft.cpp CALL GAMEOVER STATE WHEN HE DIES

		float const& GetHeight() const { return m_height; }

		void Update(const InputManager& input, float dT);
		void Attack();
		void Mine(const InputManager& input, float dT);
		void Craft();
		void PlaceBlock(const InputManager& input);
		void DestroyItem();

		void UpdateTargetBlock();
		TargetBlock GetTargetedBlock()const { return m_currentBlock; }

		Item const& GetCurrentItemSelected() const { return m_currentItemSelected; }
		int GetInventoryBarItemIndex() const;
		void SetInventoryBarItemIndex(int i);

		void SetCurrentItem(int i);

		bool& GetSanity() 
		{
			return m_sane;
		}
		float GetAtkTimer() const { return m_atkTimer; }

	private:
		void Fly(const InputManager& input, float dT);
		void Walk(const InputManager& input, float dT);
		void Swim(const InputManager& input, float dT);
		void Rotate(const InputManager& input);

		float m_height = PLAYER_HEIGHT;
		float m_blockTimer = 0.f;
		float m_atkTimer = 0.f;

		TargetBlock m_currentBlock;

		int m_currentItem;

		int m_currentInventoryBarItemIndex = 0;

		Item m_currentItemSelected;

		bool m_sane;

		//int m_currentItem = BlockId::WATER_SOURCE;

	};
}
