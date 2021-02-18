#pragma once
#include "StateMachine.h"
#include "InputManager.h"
#include "World.h"
#include "Graphics.h"
#include "UI.h"

namespace Minecraft
{
	class Minecraft;
	typedef void (Minecraft::*GameState)(float);

	class Minecraft : public StateMachine<Minecraft, GameState>
	{
	public:
		Minecraft(InputManager& inputManager, World& world, UI& ui);
		~Minecraft() = default;

		bool ShouldClose();

		World& GetWorld() const;

	private:
		InputManager& m_inputManager;
		World& m_world;

		Graphics m_graphics;

		bool m_shouldClose = false;

		UI& m_ui;

		void Init(float deltaTime);
		void Play(float deltaTime);
		void Pause(float deltaTime);
		void GameOver(float deltaTime);
		void Menu(float deltaTime);
		void Settings(float deltaTime);
		void Editor(float deltaTime);
		void Quit(float deltaTime);
	};
}
