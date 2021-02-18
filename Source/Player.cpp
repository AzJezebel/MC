#include "Player.h"

#include "Trigo.h"
#include "Physics.h"
#include "World.h"
#include "BlockLookup.h"

#pragma warning( push )
#pragma warning( disable : 4201 )
#include <glm/gtc/matrix_transform.hpp>
#pragma warning( pop )

namespace Minecraft
{
	Minecraft::Player::Player(World& world, const vec3& position, const vec2& rotation) :
		Creature(world, position, rotation, { 0.5f, 1.8f, 0.5f })
	{
		m_sane = true;
	}

	void Player::Update(const InputManager& input, float dT)
	{
		EditInWater() = BlockLookup::IsWater(m_world->GetBlock(GetPosition()));
		EditUnderWater() = BlockLookup::IsWater(m_world->GetBlock({ GetPosition().x, GetPosition().y + 1, GetPosition().z }));

		m_atkTimer -= dT;

		Rotate(input);

		if (input.GetState(InputAction::CREATIVE) == InputState::PRESSED)
			m_world->ToggleCreativeMode();

		if (m_world->InCreativeMode())
		{
			Fly(input, dT);
		}
		else
		{
			if (EditInWater())
			{
				Swim(input, dT);
				if (EditUnderWater())
					Drown(dT);
			}
			else
			{
				EditBreath() = BREATH_TIME;
				Walk(input, dT);
			}

			if (input.GetState(InputAction::JUMP) == InputState::PRESSED)
			{
				Jump();
			}
		}

		UpdateTargetBlock();
		PlaceBlock(input);
		Mine(input, dT);

		if (input.GetState(InputAction::CLICK) == InputState::PRESSED && m_currentItem == (int)ItemNameAvailable::WOODEN_SWORD && m_atkTimer <= 0.f)
		{
			Attack();
		}

		if (GetLife() <= 0)
		{
			m_sane = false;
			//this->~Player();
		}
	}

	void Player::Attack()
	{
		vec3 pos = { m_currentBlock.x, m_currentBlock.y, m_currentBlock.z };

		for (size_t index = 0; index < m_world->GetEnemies().size(); index++)
		{
			Enemy& target = m_world->GetEnemies()[index];

			//Prolly add angle check to add accuracy to our attack

			if (glm::distance(pos, target.GetPosition()) <= ATTACK_RANGE)
			{
				target.EditAttacked() = true;
				target.KnockbackEnemy();
				target.SetLife(target.GetLife() - (this->IsFalling() ? (BASE_DAMAGE * SWORD_MODIFIER) * 2.f : (BASE_DAMAGE * SWORD_MODIFIER)));
			}
		}
		m_atkTimer = ATTACK_TIMER;
	}

	void Player::Mine(const InputManager& input, float dT)
	{
		if (BlockLookup::IsTargetable(m_currentBlock.block))
		{
			if (input.GetState(InputAction::CLICK) == InputState::HOLD && input.IsInventoryKeyPressed() == false)
			{
				m_blockTimer -= dT;
				//TODO Breaking block animation
				if (m_atkTimer <= 0.f)
				{
					m_atkTimer = ATTACK_TIMER;
				}

				if (m_blockTimer <= 0.f)
				{
					GetInventory().AddMinedItemInInventory(m_currentBlock.block);
					m_world->SetBlock(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z, BlockId::AIR);
				}
			}
		}
	}

	void Player::Craft()
	{
		//TODO
	}

	void Player::PlaceBlock(const InputManager& input)
	{
		//TODO FIX CONDITION
		if (input.GetState(InputAction::RIGHT_CLICK) == InputState::PRESSED && input.IsInventoryKeyPressed() == false /*&& m_currentItem != BlockId::AIR*/)
		{
			int x = m_currentBlock.x;
			int y = m_currentBlock.y;
			int z = m_currentBlock.z;

			switch (m_currentBlock.face)
			{
			case Face::B_100:
				x--;
				break;
			case Face::B_010:
				y--;
				break;
			case Face::B_001:
				z--;
				break;
			case Face::F_100:
				x++;
				break;
			case Face::F_010:
				y++;
				break;
			case Face::F_001:
				z++;
				break;
			default:
				return;
			}

			int block = m_world->GetBlock(x, y, z);
			if (!BlockLookup::IsSolid(block) && block != BlockId::TORCH &&
				BlockLookup::IsSolid(m_currentBlock.block) && GetInventory().GetItemInventoryBarArray()[m_currentInventoryBarItemIndex].IsABlock() == true)
			{
				m_world->SetBlock(x, y, z, m_currentItem);
				GetInventory().GetItemInventoryBarArray()[m_currentInventoryBarItemIndex].SetCountItem(GetInventory().GetItemInventoryBarArray()[m_currentInventoryBarItemIndex].GetCountItem() - 1);
			}
		}
	}

	void Player::DestroyItem()
	{
	}

	void Player::UpdateTargetBlock()
	{
		TargetBlock currBlock = m_world->physics.GetTargetedBlock(GetPosition() + vec3(0.f, m_height, 0.f), GetRotation(), MAX_DISTANCE);

		if (!(currBlock == m_currentBlock) || currBlock.block != m_currentBlock.block)
		{
			m_blockTimer = BlockLookup::BlockHardenessOf(m_world->GetBlock(currBlock.x, currBlock.y, currBlock.z));
			m_currentBlock = currBlock;
		}
		else
		{
			m_currentBlock.face = currBlock.face;
		}
	}

	int Player::GetInventoryBarItemIndex() const
	{
		return m_currentInventoryBarItemIndex;
	}

	void Player::SetInventoryBarItemIndex(int i)
	{
		m_currentInventoryBarItemIndex = i;
	}

	void Player::SetCurrentItem(int i)
	{
		m_currentItem = i;
	}

	void Player::Fly(const InputManager& input, float dT)
	{
		vec3 move = vec3{ 0.f, 0.f, 0.f };

		float speed = SPRINT_SPEED;

		if (input.GetState(InputAction::FLY_UP) == InputState::HOLD || input.GetState(InputAction::JUMP) == InputState::HOLD)
			move.y += SPRINT_SPEED;
		if (input.GetState(InputAction::FLY_DOWN) == InputState::HOLD || input.GetState(InputAction::CROUCH) == InputState::HOLD)
			move.y -= SPRINT_SPEED;
		if (input.GetState(InputAction::MOVE_UP) == InputState::HOLD)
		{
			if (input.GetState(InputAction::SPRINT) == InputState::HOLD)
				speed = FLY_SPEED;
			move.z += -cos(GetRotation().x) * speed;
			move.x += -sin(GetRotation().x) * speed;
		}
		if (input.GetState(InputAction::MOVE_DOWN) == InputState::HOLD)
		{
			move.z += cos(GetRotation().x) * speed;
			move.x += sin(GetRotation().x) * speed;
		}
		if (input.GetState(InputAction::MOVE_LEFT) == InputState::HOLD)
		{
			move.z += -cos(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
			move.x += -sin(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
		}
		if (input.GetState(InputAction::MOVE_RIGHT) == InputState::HOLD)
		{
			move.z += cos(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
			move.x += sin(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
		}

		Creature::Fly(move, dT);
	}

	void Player::Walk(const InputManager& input, float dT)
	{
		vec2 move = vec2{ 0.f, 0.f };

		bool crouching = input.GetState(InputAction::CROUCH) == InputState::HOLD;
		float speed = crouching ? CROUCH_SPEED : BASE_SPEED;
		m_height = crouching ? CROUCH_HEIGHT : PLAYER_HEIGHT;

		if (input.GetState(InputAction::MOVE_UP) == InputState::HOLD)
		{
			if (!crouching && input.GetState(InputAction::SPRINT) == InputState::HOLD)
				speed = SPRINT_SPEED;
			move.y += -cos(GetRotation().x) * speed;
			move.x += -sin(GetRotation().x) * speed;
		}
		if (input.GetState(InputAction::MOVE_DOWN) == InputState::HOLD)
		{
			move.y += cos(GetRotation().x) * speed;
			move.x += sin(GetRotation().x) * speed;
		}
		if (input.GetState(InputAction::MOVE_LEFT) == InputState::HOLD)
		{
			move.y += -cos(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
			move.x += -sin(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
		}
		if (input.GetState(InputAction::MOVE_RIGHT) == InputState::HOLD)
		{
			move.y += cos(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
			move.x += sin(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
		}

		Creature::Walk(move, dT, crouching ? 0.f : -1.f);
	}

	void Player::Swim(const InputManager& input, float dT)
	{
		vec3 move = vec3{ 0.f, 0.f, 0.f };

		float speed = SWIM_SPEED;

		if (input.GetState(InputAction::MOVE_UP) == InputState::HOLD)
		{
			if (input.GetState(InputAction::SPRINT) == InputState::HOLD)
				speed = SPRINT_SPEED;
			move.z += -cos(GetRotation().x) * speed;
			move.x += -sin(GetRotation().x) * speed;
			move.y += sinf(GetRotation().y) * speed;
		}
		if (input.GetState(InputAction::MOVE_DOWN) == InputState::HOLD)
		{
			move.z += cos(GetRotation().x) * speed;
			move.x += sin(GetRotation().x) * speed;
			move.y += -sinf(GetRotation().y) * speed;
		}
		if (input.GetState(InputAction::MOVE_LEFT) == InputState::HOLD)
		{
			move.z += -cos(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
			move.x += -sin(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
		}
		if (input.GetState(InputAction::MOVE_RIGHT) == InputState::HOLD)
		{
			move.z += cos(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
			move.x += sin(GetRotation().x + Trigo::QUARTER_CIRCLE_RAD) * speed;
		}
		if (input.GetState(InputAction::JUMP) == InputState::HOLD)
		{
			move.y += speed;
		}
		if (input.GetState(InputAction::CROUCH) == InputState::HOLD)
		{
			move.y -= speed;
		}

		Creature::Swim(move, dT);
	}

	void Player::Rotate(const InputManager& input)
	{
		float newX = GetRotation().x + input.GetMouseHoriMove() * MOUSE_SENSI;
		float newY = GetRotation().y + input.GetMouseVertMove() * MOUSE_SENSI;

		if (newY > Trigo::QUARTER_CIRCLE_RAD)
			newY = Trigo::QUARTER_CIRCLE_RAD;
		if (newY < -Trigo::QUARTER_CIRCLE_RAD)
			newY = -Trigo::QUARTER_CIRCLE_RAD;

		SetRotation(newX, newY);
	}
}