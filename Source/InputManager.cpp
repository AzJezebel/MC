#include "InputManager.h"

#include <iostream> // TODO: use log
#include <UI.h>

namespace Minecraft
{
	std::unordered_map<GLFWwindow*, InputManager*> InputManager::s_managers;

	InputManager::InputManager(GLFWwindow* window, int winWidth, int winHeight) :
		m_windowHalfWidth(winWidth / 2), m_windowHalfHeight(winHeight / 2)
	{
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

		glfwSetCursorPosCallback(window, MouseMoveCallback);

		glfwSetScrollCallback(window, MouseScrollCallback);
		glfwSetMouseButtonCallback(window, MouseClickCallback);
		glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

		// TODO: if(canLoadPref) loadPref(); else
		SetDefaultInput();

		s_managers[window] = this;
	}

	InputManager* InputManager::GetManager(GLFWwindow* window)
	{
		std::unordered_map<GLFWwindow*, InputManager*>::iterator findResult = s_managers.find(window);

		if (findResult != s_managers.end())
		{
			return findResult->second;
		}
		else
		{
			// TODO: use log
			std::cout << "Current window has no InputManager attached.";
			return nullptr;
		}
	}

	void InputManager::KeyCallback(GLFWwindow* window, int glfwKey, int /*scancode*/, int glfwAction, int /*mods*/)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (glfwAction == GLFW_PRESS)
		{
			io.KeysDown[glfwKey] = true;
			if (!io.WantCaptureKeyboard)
			{
				printf("Keypress for app: %d\n", glfwKey);
			}
		}
		else if (glfwAction == GLFW_RELEASE)
		{
			io.KeysDown[glfwKey] = false;
		}

		if (glfwAction == GLFW_REPEAT)
		{
			return;
		}

		InputManager* manager = GetManager(window);
		InputAction action = manager->GetAction(glfwKey);

		if (glfwKey == GLFW_KEY_TAB && glfwAction == GLFW_PRESS)
		{
			manager->m_mouseLock = !manager->m_mouseLock;

			glfwSetInputMode(window, GLFW_CURSOR, manager->m_mouseLock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

			return;
		}

		if (glfwAction == GLFW_PRESS)
		{
			if (!manager->m_bindingNewKey)
				manager->m_pendingChange.emplace(action, InputState::PRESSED);
			else
				if (manager->GetAction(glfwKey) == InputAction::NO_ACTION)
				{
					manager->ChangeKey(manager->GetAction(GLFW_KEY_UNKNOWN), glfwKey);
					manager->m_bindingNewKey = false;
				}
		}
		else //if (glfwAction == GLFW_RELEASE)
		{
			manager->m_pendingChange.emplace(action, InputState::RELEASED);
		}
	}

	void InputManager::MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
	{
		InputManager* manager = GetManager(window);

		if (!manager->m_mouseLock)
			return;

		manager->m_mouseVertPending += manager->m_windowHalfHeight - (float)ypos;
		manager->m_mouseHoriPending += manager->m_windowHalfWidth - (float)xpos;

		glfwSetCursorPos(window, manager->m_windowHalfWidth, manager->m_windowHalfHeight);
	}

	void InputManager::MouseClickCallback(GLFWwindow* window, int glfwButton, int glfwAction, int /*mods*/)
	{
		InputManager* manager = GetManager(window);

		InputAction action = InputAction::NO_ACTION;
		if (glfwButton == GLFW_MOUSE_BUTTON_LEFT) // TODO: GLFW_MOUSE_BUTTON_1 ->GLFW_MOUSE_BUTTON_8
		{
			action = InputAction::CLICK;
		}
		else if (glfwButton == GLFW_MOUSE_BUTTON_RIGHT)
		{
			action = InputAction::RIGHT_CLICK;
		}

		if (glfwAction == GLFW_PRESS)
		{
			manager->m_pendingChange.emplace(action, InputState::PRESSED);
		}
		else //if (glfwAction == GLFW_RELEASE)
		{
			manager->m_pendingChange.emplace(action, InputState::RELEASED);
		}
	}

	void InputManager::MouseScrollCallback(GLFWwindow* window, double /*xoffset*/, double yoffset)
	{
		InputManager* manager = GetManager(window);

		if (!manager->m_mouseLock)
			return;

		manager->m_mouseScrollPending += (float)yoffset;
	}

	void InputManager::WindowResize(GLFWwindow* window, int width, int height)
	{
		InputManager* manager = GetManager(window);

		manager->m_windowHalfWidth = width / 2;
		manager->m_windowHalfHeight = height / 2;

		if(manager->updateProjection != nullptr)
		{
			manager->updateProjection((float)width / (float)height);
		}
	}

	void InputManager::SetMouseLock(bool tf)
	{
		m_mouseLock = tf;
	}

	bool InputManager::IsInventoryKeyPressed() const
	{
		return m_inventoryKeyPressed;
	}

	void InputManager::SetInventoryKeyPressed(bool tf)
	{
		m_inventoryKeyPressed = tf;
	}

	void InputManager::Update()
	{
		std::unordered_map<InputAction, InputState>::iterator iterator = m_currentState.begin();
		while (iterator != m_currentState.end())
		{
			if (iterator->second == InputState::PRESSED)
			{
				iterator->second = InputState::HOLD;
			}
			else if (iterator->second == InputState::RELEASED)
			{
				iterator->second = InputState::FREE;
			}
			iterator++;
		}

		while (!m_pendingChange.empty())
		{
			std::pair<InputAction, InputState> pair = m_pendingChange.front();
			m_currentState[pair.first] = pair.second;
			m_pendingChange.pop();
		}

		m_mouseVertCurrent = m_mouseVertPending;
		m_mouseVertPending = 0;

		m_mouseHoriCurrent = m_mouseHoriPending;
		m_mouseHoriPending = 0;

		m_mouseScrollCurrent = m_mouseScrollPending;
		m_mouseScrollPending = 0;
	}

	InputState InputManager::GetState(InputAction action) const
	{
		if (action == InputAction::NO_ACTION)
		{
			return InputState::FREE;
		}

		std::unordered_map<InputAction, InputState>::const_iterator findResult = m_currentState.find(action);

		if (findResult == m_currentState.end())
		{
			return InputState::FREE;
		}
		else
		{
			return findResult->second;
		}
	}

	float InputManager::GetMouseVertMove() const
	{
		return m_mouseVertCurrent;
	}

	float InputManager::GetMouseHoriMove() const
	{
		return m_mouseHoriCurrent;
	}

	float InputManager::GetMouseScroll() const
	{
		return m_mouseScrollCurrent;
	}

	// Searches the map for the key corresponding to the action.
	// -1 is returned if the action is unbound
	const int InputManager::GetKey(const InputAction action)
	{
		int key = GLFW_KEY_UNKNOWN;

		for (auto& pair : m_actions)
			if (pair.second == action)
			{
				key = pair.first;
				break;
			}

		return key;
	}

	void InputManager::ChangeKey(const InputAction action, const int newKey)
	{
		int oldKey = GetKey(action);

		auto handle = m_actions.extract(oldKey);
		handle.key() = newKey;
		m_actions.insert(move(handle));
	}

	void InputManager::UnbindAction(const InputAction action)
	{
		m_bindingNewKey = true;
		ChangeKey(action, GLFW_KEY_UNKNOWN);
	}

	InputAction InputManager::GetAction(int key)
	{
		std::unordered_map<int, InputAction>::iterator findResult = m_actions.find(key);

		if (findResult != m_actions.end())
		{
			return findResult->second;
		}
		else
		{
			return InputAction::NO_ACTION;
		}
	}

	void InputManager::SetDefaultInput()
	{
		m_actions[GLFW_KEY_P] = InputAction::PAUSE;
		m_currentState[InputAction::PAUSE] = InputState::FREE;

		m_actions[GLFW_KEY_W] = InputAction::MOVE_UP;
		m_currentState[InputAction::MOVE_UP] = InputState::FREE;

		m_actions[GLFW_KEY_S] = InputAction::MOVE_DOWN;
		m_currentState[InputAction::MOVE_DOWN] = InputState::FREE;

		m_actions[GLFW_KEY_D] = InputAction::MOVE_RIGHT;
		m_currentState[InputAction::MOVE_RIGHT] = InputState::FREE;

		m_actions[GLFW_KEY_A] = InputAction::MOVE_LEFT;
		m_currentState[InputAction::MOVE_LEFT] = InputState::FREE;

		m_actions[GLFW_KEY_SPACE] = InputAction::JUMP;
		m_currentState[InputAction::JUMP] = InputState::FREE;

		m_actions[GLFW_KEY_LEFT_SHIFT] = InputAction::SPRINT;
		m_currentState[InputAction::SPRINT] = InputState::FREE;

		m_actions[GLFW_KEY_LEFT_CONTROL] = InputAction::CROUCH;
		m_currentState[InputAction::CROUCH] = InputState::FREE;

		m_actions[GLFW_KEY_ESCAPE] = InputAction::QUIT;
		m_currentState[InputAction::QUIT] = InputState::FREE;

		m_actions[GLFW_KEY_E] = InputAction::FLY_UP;
		m_currentState[InputAction::FLY_UP] = InputState::FREE;

		m_actions[GLFW_KEY_Q] = InputAction::FLY_DOWN;
		m_currentState[InputAction::FLY_DOWN] = InputState::FREE;

		m_actions[GLFW_KEY_I] = InputAction::INVENTORY;
		m_currentState[InputAction::INVENTORY] = InputState::FREE;

		m_actions[GLFW_KEY_F3] = InputAction::DEBUG_IN_GAME;
		m_currentState[InputAction::DEBUG_IN_GAME] = InputState::FREE;

		m_actions[GLFW_KEY_M] = InputAction::CREATIVE;
		m_currentState[InputAction::CREATIVE] = InputState::FREE;

		m_actions[GLFW_KEY_1] = InputAction::ITEM_SLOT_1;
		m_currentState[InputAction::ITEM_SLOT_1] = InputState::FREE;

		m_actions[GLFW_KEY_2] = InputAction::ITEM_SLOT_2;
		m_currentState[InputAction::ITEM_SLOT_1] = InputState::FREE;

		m_actions[GLFW_KEY_3] = InputAction::ITEM_SLOT_3;
		m_currentState[InputAction::ITEM_SLOT_1] = InputState::FREE;

		m_actions[GLFW_KEY_4] = InputAction::ITEM_SLOT_4;
		m_currentState[InputAction::ITEM_SLOT_1] = InputState::FREE;

		m_actions[GLFW_KEY_5] = InputAction::ITEM_SLOT_5;
		m_currentState[InputAction::ITEM_SLOT_1] = InputState::FREE;

		m_actions[GLFW_KEY_6] = InputAction::ITEM_SLOT_6;
		m_currentState[InputAction::ITEM_SLOT_1] = InputState::FREE;

		m_actions[GLFW_KEY_7] = InputAction::ITEM_SLOT_7;
		m_currentState[InputAction::ITEM_SLOT_1] = InputState::FREE;

		m_actions[GLFW_KEY_8] = InputAction::ITEM_SLOT_8;
		m_currentState[InputAction::ITEM_SLOT_1] = InputState::FREE;

		m_actions[GLFW_KEY_9] = InputAction::ITEM_SLOT_9;
		m_currentState[InputAction::ITEM_SLOT_1] = InputState::FREE;

		m_actions[GLFW_KEY_F1] = InputAction::HIDE_HUD;
		m_currentState[InputAction::HIDE_HUD] = InputState::FREE;

	}
}