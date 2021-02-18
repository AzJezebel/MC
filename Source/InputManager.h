#pragma once
#include "GLFW/glfw3.h"
#include <unordered_map>
#include <queue>

namespace Minecraft
{
	typedef void(*inputCallback)();

	enum class InputAction
	{
		PAUSE,
		INVENTORY,
		DEBUG_IN_GAME,
		HIDE_HUD,
		MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT,
		FLY_UP, FLY_DOWN,	/*TODO This is for debug care to rm*/
		ITEM_SLOT_1, ITEM_SLOT_2, ITEM_SLOT_3, ITEM_SLOT_4, ITEM_SLOT_5, ITEM_SLOT_6, ITEM_SLOT_7, ITEM_SLOT_8, ITEM_SLOT_9,
		JUMP,
		SPRINT,
		CROUCH,
		CREATIVE,
		CLICK, RIGHT_CLICK,
		QUIT,
		NO_ACTION
	};

	enum class InputState
	{
		RELEASED,
		PRESSED,
		HOLD,
		FREE // TODO: rename FREE
	};

	class InputManager
	{
	public:
		InputManager(GLFWwindow* window, int winWidth, int winHeight);
		~InputManager() = default;

		void Update();

		InputState GetState(InputAction action) const;
		
		float GetMouseVertMove() const;
		float GetMouseHoriMove() const;
		float GetMouseScroll() const;

		const int GetKey(const InputAction action);
		void ChangeKey(const InputAction action, const int newKey);
		void UnbindAction(const InputAction action);
		bool GetBindingStatus() { return m_bindingNewKey; }

		void (*updateProjection)(float aspectRatio) = nullptr;

		static void WindowResize(GLFWwindow* window, int width, int height);

		void SetMouseLock(bool tf);

		bool IsInventoryKeyPressed() const;
		
		void SetInventoryKeyPressed(bool tf);
		

	private:
		static std::unordered_map<GLFWwindow*, InputManager*> s_managers;
		static InputManager* GetManager(GLFWwindow* window);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
		static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);


		std::unordered_map<int, InputAction> m_actions;
		std::unordered_map<InputAction, InputState> m_currentState;
		std::queue<std::pair<InputAction, InputState>> m_pendingChange;

		void SetDefaultInput();
		InputAction GetAction(int key);

		bool m_bindingNewKey = false;
		bool m_mouseLock = true;
		int m_windowHalfWidth;
		int m_windowHalfHeight;

		float m_mouseVertPending;
		float m_mouseHoriPending;
		float m_mouseScrollPending;

		float m_mouseVertCurrent;
		float m_mouseHoriCurrent;
		float m_mouseScrollCurrent;

		bool m_inventoryKeyPressed = false;
	};
}
