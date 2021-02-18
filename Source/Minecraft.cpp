#include "Minecraft.h"
#include "Player.h"

#include <iostream>
#include <stdlib.h>   //srand
#include <time.h>       //time

#pragma warning( push )
#pragma warning( disable : 4201 )
#pragma warning( disable : 4701 ) // potentially uninitialized local variable 'Result' used	(glm) func_common.inl L-99
#include <glm/gtc/matrix_transform.hpp>
#pragma warning( pop )

namespace Minecraft
{
	Minecraft::Minecraft(InputManager& inputManager, World& world, UI& ui) :
		StateMachine(&Minecraft::Init),
		m_inputManager(inputManager),
		m_world(world),
		m_graphics(world),
		m_ui(ui)
	{
	}

	bool Minecraft::ShouldClose()
	{
		return m_shouldClose;
	}

	World& Minecraft::GetWorld() const
	{
		return m_world;
	}

	void Minecraft::Init(float /*deltaTime*/)
	{
		// TODO: prepare all entities
		ReplaceState(&Minecraft::Menu);

		srand((unsigned int)time(NULL));
	}

	bool f3 = true;
	bool hidehudbool = false;
	void Minecraft::Play(float deltaTime)
	{
		if (m_ui.IsInventoryWindowsDisplaying() == false && hidehudbool == false)
		{
			m_ui.DisplayGUIInGameWindows();
		}
		m_world.GetPlayer().Update(m_inputManager, deltaTime);

		if (m_world.GetPlayer().GetSanity() == false)
		{
			ReplaceState(&Minecraft::GameOver);
		}

		m_world.UpdateWorld(deltaTime);
		m_graphics.Update(deltaTime);

		m_world.GetCamera().Update();
		m_world.GetMesh().UpdateMovingBlock(m_world.GetFallingBlocks());
		m_graphics.DisplayWorld(m_world.GetCamera());

		m_ui.SetPlayerPos(m_world.GetPlayer().GetPosition(), m_world.GetPlayer().GetRotation());
		m_ui.SetChunkPos(m_world.GetPlayerChunkPos());

		m_world.CheckPlayerCenter();

		for (Enemy& enemy : m_world.GetEnemies())
		{
			enemy.Update(m_world.GetPlayer(), deltaTime);
		}

		InputState quit = m_inputManager.GetState(InputAction::QUIT);
		if (quit == InputState::PRESSED)
		{
			ReplaceState(&Minecraft::Quit);
		}

		InputState pause = m_inputManager.GetState(InputAction::PAUSE);
		if (pause == InputState::PRESSED || m_world.m_forcePause)
		{
			m_world.m_forcePause = false;
			EnterTempState(&Minecraft::Pause);
		}

		GLFWwindow* window = glfwGetCurrentContext();

		InputState inventory = m_inputManager.GetState(InputAction::INVENTORY);
		if (inventory == InputState::PRESSED)
		{
			if (m_ui.IsInventoryWindowsDisplaying() == false)
			{
				m_ui.DisplayInventoryWindows();

				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				m_inputManager.SetMouseLock(false);
				m_inputManager.SetInventoryKeyPressed(true);
			}
			else
			{
				m_ui.CloseInventoryWindows();
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				m_inputManager.SetMouseLock(true);
				m_inputManager.SetInventoryKeyPressed(false);
			}
		}

		InputState hideHud = m_inputManager.GetState(InputAction::HIDE_HUD);
		if (hideHud == InputState::PRESSED)
		{
			if (m_ui.IsGUIInGameWindowsDisplaying() == false)
			{
				m_ui.DisplayGUIInGameWindows();
				f3 = true;
				if (f3 == true)
				{
					m_ui.DisplayDebugWindows();
				}
				hidehudbool = false;
			}
			else
			{
				m_ui.CloseGUIInGameWindows();
				m_ui.CloseDebugWindows();
				hidehudbool = true;
				f3 = false;
			}
		}

		InputState debugWindow = m_inputManager.GetState(InputAction::DEBUG_IN_GAME);
		if (debugWindow == InputState::PRESSED)
		{
			if (m_ui.IsDebugWindowsDisplaying() == false)
			{
				m_ui.DisplayDebugWindows();
				f3 = true;
			}
			else
			{
				m_ui.CloseDebugWindows();
				f3 = false;
			}
		}

		//Mouse wheel up
		if (m_inputManager.GetMouseScroll() == -1)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(m_world.GetPlayer().GetInventoryBarItemIndex() + 1);
			if (m_world.GetPlayer().GetInventoryBarItemIndex() > 8)
			{
				m_world.GetPlayer().SetInventoryBarItemIndex(0);
			}
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[m_world.GetPlayer().GetInventoryBarItemIndex()].GetItemNameEnum());
		}
		//Mouse wheel down
		else if (m_inputManager.GetMouseScroll() == 1)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(m_world.GetPlayer().GetInventoryBarItemIndex() - 1);
			if (m_world.GetPlayer().GetInventoryBarItemIndex() < 0)
			{
				m_world.GetPlayer().SetInventoryBarItemIndex(8);
			}
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[m_world.GetPlayer().GetInventoryBarItemIndex()].GetItemNameEnum());
		}
		InputState changeItenSlotTo1 = m_inputManager.GetState(InputAction::ITEM_SLOT_1);
		if (changeItenSlotTo1 == InputState::PRESSED)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(0);
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[0].GetItemNameEnum());
		}
		InputState changeItenSlotTo2 = m_inputManager.GetState(InputAction::ITEM_SLOT_2);
		if (changeItenSlotTo2 == InputState::PRESSED)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(1);
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[1].GetItemNameEnum());
		}
		InputState changeItenSlotTo3 = m_inputManager.GetState(InputAction::ITEM_SLOT_3);
		if (changeItenSlotTo3 == InputState::PRESSED)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(2);
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[2].GetItemNameEnum());
		}
		InputState changeItenSlotTo4 = m_inputManager.GetState(InputAction::ITEM_SLOT_4);
		if (changeItenSlotTo4 == InputState::PRESSED)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(3);
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[3].GetItemNameEnum());
		}
		InputState changeItenSlotTo5 = m_inputManager.GetState(InputAction::ITEM_SLOT_5);
		if (changeItenSlotTo5 == InputState::PRESSED)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(4);
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[4].GetItemNameEnum());
		}
		InputState changeItenSlotTo6 = m_inputManager.GetState(InputAction::ITEM_SLOT_6);
		if (changeItenSlotTo6 == InputState::PRESSED)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(5);
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[5].GetItemNameEnum());
		}
		InputState changeItenSlotTo7 = m_inputManager.GetState(InputAction::ITEM_SLOT_7);
		if (changeItenSlotTo7 == InputState::PRESSED)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(6);
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[6].GetItemNameEnum());
		}
		InputState changeItenSlotTo8 = m_inputManager.GetState(InputAction::ITEM_SLOT_8);
		if (changeItenSlotTo8 == InputState::PRESSED)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(7);
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[7].GetItemNameEnum());
		}
		InputState changeItenSlotTo9 = m_inputManager.GetState(InputAction::ITEM_SLOT_9);
		if (changeItenSlotTo9 == InputState::PRESSED)
		{
			m_world.GetPlayer().SetInventoryBarItemIndex(8);
			m_world.GetPlayer().SetCurrentItem((int)m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[8].GetItemNameEnum());
		}

		if (f3)
			m_graphics.DisplayPlayerTarget(m_world.GetPlayer().GetTargetedBlock());
	}

	bool onPause = false;
	void Minecraft::Pause(float /*deltaTime*/)
	{
		GLFWwindow* window = glfwGetCurrentContext();

		if (!onPause)
		{
			m_ui.DisplayPauseMenuWindows();
			std::cout << "Pause" << std::endl;
			onPause = true;

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_inputManager.SetMouseLock(false);
		}

		InputState pause = m_inputManager.GetState(InputAction::PAUSE);
		if (pause == InputState::PRESSED)
		{
			ExitTempState();
			m_ui.ClosePauseMenuWindows();
			onPause = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_inputManager.SetMouseLock(true);
		}

		if (m_ui.IsSwitchingStateToSettings() == true)
		{
			m_ui.ClosePauseMenuWindows();
			m_ui.CloseGUIInGameWindows();
			m_ui.CloseDebugWindows();
			m_ui.SetPreviousState(PreviousState::PAUSE);
			m_ui.SetSwitchStateSettings(false);
			ReplaceState(&Minecraft::Settings);
		}

		if (m_ui.IsSwitchingStateToMenu() == true)
		{
			m_ui.ClosePauseMenuWindows();
			m_ui.SetSwitchStateMenu(false);
			onPause = false;
			//Save here
			ReplaceState(&Minecraft::Menu);
		}
	}

	void Minecraft::GameOver(float /*deltaTime*/)
	{
		// TODO: Free all entities
		//m_world.Save("testSave.txt");		//In case we have LoadGame
		ReplaceState(&Minecraft::Menu);
	}

	void Minecraft::Menu(float /*deltaTime*/)
	{
		GLFWwindow* window = glfwGetCurrentContext();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_inputManager.SetMouseLock(false);

		if (m_ui.IsMainMenuWindowsDisplaying() == false && m_ui.IsSelectWorldMenuWindowsDisplaying() == false)
		{
			m_ui.DisplayMainMenuWindows();
		}

		if (m_ui.IsSwitchingStateToPlay() == true)
		{
			m_ui.CloseSelectWorldMenuWindows();
			m_ui.SetSwitchStatePlay(false);

			if (m_world.GetPlayer().GetSanity() == false)
			{
				m_world.GetPlayer().GetSanity() = true;
				m_world.GetPlayer().SetLife(8);
				//Player player(m_world, { 0, 3, 0 }, { Trigo::DEGREE_TO_RADIAN * 225.f, 0 });
				//m_world.SetPlayer(player);
			}
#ifdef _DEBUG
			m_ui.DisplayDebugWindows();
#endif
			m_inputManager.SetMouseLock(true);

			if (m_ui.IsLoadingWorld())
			{
				m_ui.IsLoadingWorld() = false;
				m_world.Load("world.txt");
			}
			else
				m_world.GenerateWorld();
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			ReplaceState(&Minecraft::Play);
		}

		if (m_ui.IsSwitchingStateToSettings() == true)
		{
			m_ui.CloseMainMenuWindows();
			m_ui.SetSwitchStateSettings(false);
			m_ui.SetPreviousState(PreviousState::MENU);
			ReplaceState(&Minecraft::Settings);
		}

		if (m_ui.IsSwitchingStateToQuit() == true)
		{
			m_ui.CloseMainMenuWindows();
			m_ui.SetSwitchStateQuit(false);
			ReplaceState(&Minecraft::Quit);
		}
	}

	void Minecraft::Settings(float /*deltaTime*/)
	{
		GLFWwindow* window = glfwGetCurrentContext();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_inputManager.SetMouseLock(false);

		m_ui.DisplaySettingsWindows();
		if (m_ui.IsSwitchingStateToMenu() == true)
		{
			m_ui.CloseSettingsWindows();
			m_ui.SetSwitchStateMenu(false);
			ReplaceState(&Minecraft::Menu);
		}

		if (m_ui.IsSwitchingStateToPlay() == true)
		{
			m_ui.CloseSettingsWindows();
			m_ui.DisplayGUIInGameWindows();
			m_ui.DisplayDebugWindows();
			onPause = false;
			m_ui.SetSwitchStatePlay(false);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_inputManager.SetMouseLock(true);
			//m_world.Load("testSave.txt");
			ReplaceState(&Minecraft::Play);
		}
	}

	void Minecraft::Editor(float /*deltaTime*/)
	{
	}

	void Minecraft::Quit(float /*deltaTime*/)
	{
		m_shouldClose = true;
		m_world.Save("world.txt");
	}
}