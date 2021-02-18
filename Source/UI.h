#pragma once
#include "TextureManager.h"
#include "World.h"
#include <GLFW\glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <irrKlang.h>

namespace Minecraft
{
	enum class PreviousState
	{
		MENU,
		PAUSE
	};
	enum class PositionXButtonImGui
	{
		CENTER_LEFT,
		CENTER,
		CENTER_RIGHT
	};

	enum class PositionYButtonImGui
	{
		FIRST_LINE,
		SECOND_LINE,
		THIRD_LINE,
		FOURTH_LINE,
		FIFTH_LINE,
		SIXTH_LINE,
		SEVENTH_LINE,
		HEIGHTTH_LINE,
		NINETH_LINE,

		NONE
	};

	enum class ButtonSize
	{
		BIG,
		SMALL
	};

	enum class GUISize
	{
		BIG,
		MEDIUM,
		SMALL
	};

	enum class CurrentDragFrom
	{
		INVENTORY,
		CRAFTING,
		CRAFTING_RESULT,
		INVENTORY_BAR,
		CHEST
	};

	enum class ButtonLogic
	{
		SINGLE_PLAYER,
		OPTIONS,
		QUIT_GAME,
		DONE_SETTINGS,
		GUI_SCALE_1,
		GUI_SCALE_2,
		GUI_SCALE_3,
		CREATE_NEW_WORLD,
		LOAD_WORLD,
		RETURN_TO_MENU
	};

	enum class Slots
	{
		INVENTORY,
		INVENTORY_BAR,
		CRAFTING,
		CRAFTING_RESULT
	};

	class UI
	{
	public:

		//Constructor / Destructor
		UI(TextureManager& textureManager);
		~UI() = default;

		//Update the UI each frame
		void Update(World& world);
		// /!\  WARNING : Don't forget to call this function in the main loop to get the window size or else the ImGui windows won't display
		void UpdateGLFWWindowSize(GLFWwindow* window);

		//Call init functions of ImGui to create windows
		void InitImGuiWindows();
		//Call render functions of ImGui to render windows
		void RenderImGuiWindows();

		//ImGui Windows

		//Create am ImGui window containing the background image of the inventory
		void CreateInventoryBackgroundWindow();
		//Create am ImGui window containing the inventory
		void CreateInventoryWindow(World& world);

		//Create am ImGui window containing my custom debug window
		void CreateDebugWindow();

		//Create am ImGui window containing the background image of the main menu
		void CreateMainMenuBackgroundWindow();
		//Create am ImGui window containing the the main menu
		void CreateMainMenuWindow(World& world);

		void CreateSelectWorldMenuBackgroundWindow();
		void CreateSelectWorldMenuWindow(World& world);

		void CreatePauseWindow(World& world);

		//Create am ImGui window containing the background image of the settings menu
		void CreateSettingsBackgroundWindow();

		//Create am ImGui window containing the settings menu
		void CreateSettingsWindow(World& world);

		void CreateDebugWindowsInGame(World& world);

		void CreateGUIInGame(World& world);

		void FPSCounter();
		void PlayerPos(World& world);
		void RegenWorld(World& world);
		void ChangeHPPlayer(World& world);

		//Bool functions

		// Calling this function will display the inventory
		void DisplayInventoryWindows();
		// Calling this function will close the inventory
		void CloseInventoryWindows();
		// Is the inventory being display
		bool& IsInventoryWindowsDisplaying();

		// Calling this function will display the main menu
		void DisplayMainMenuWindows();
		// Calling this function will close the main menu
		void CloseMainMenuWindows();
		// Is the main menu being display
		bool& IsMainMenuWindowsDisplaying();

		// Calling this function will display the settings menu
		void DisplaySettingsWindows();
		// Calling this function will close the settings menu
		void CloseSettingsWindows();
		// Is the settings menu being display
		bool& IsSettingsWindowsDisplaying();

		// Calling this function will display the settings menu
		void DisplayDebugWindows();
		// Calling this function will close the settings menu
		void CloseDebugWindows();
		// Is the settings menu being display
		bool& IsDebugWindowsDisplaying();

		// Calling this function will display the settings menu
		void DisplayGUIInGameWindows();
		// Calling this function will close the settings menu
		void CloseGUIInGameWindows();
		// Is the settings menu being display
		bool& IsGUIInGameWindowsDisplaying();

		// Calling this function will display the settings menu
		void DisplaySelectWorldMenuWindows();
		// Calling this function will close the settings menu
		void CloseSelectWorldMenuWindows();
		// Is the settings menu being display
		bool& IsSelectWorldMenuWindowsDisplaying();

		// Calling this function will display the pause menu
		void DisplayPauseMenuWindows();
		// Calling this function will close the pause menu
		void ClosePauseMenuWindows();
		// Is the pause menu being display
		bool& IsPauseWindowsDisplaying();

		/*Set the bool value used to switch state to the Main Menu
		Passing true in parameter will let know to Minecraft.cpp that we are changing state to the Main Menu. After changing state, the value must return to false
		*/
		void SetSwitchStateMenu(bool tf);
		/*Return the bool value used to switch state to the Main Menu
		If the function return true, we are changing state to the Main Menu. Otherwise, this function should return false if we aren't chaging state.
		*/
		bool& IsSwitchingStateToMenu();

		/*Set the bool value used to switch state to the Play
		Passing true in parameter to true will let know to Minecraft.cpp that we are changing state to Play. After changing state, the value must return to false
		*/
		void SetSwitchStatePlay(bool tf);
		/*Return the bool value used to switch state to Play
		If the function return true, we are changing state to Play. Otherwise, this function should return false if we aren't chaging state.
		*/
		bool& IsSwitchingStateToPlay();

		/*Set the bool value used to switch state to the Settings
		Passing true in parameter to true will let know to Minecraft.cpp that we are changing state to Settings. After changing state, the value must return to false
		*/
		void SetSwitchStateSettings(bool tf);
		/*Return the bool value used to switch state to the Settings
		If the function return true, we are changing state to Settings. Otherwise, this function should return false if we aren't chaging state.
		*/
		bool& IsSwitchingStateToSettings();

		/*Set the bool value used to switch state to Quit
		Passing true in parameter to true will let know to Minecraft.cpp that we are changing state to Quit. After changing state, the value must return to false
		*/
		void SetSwitchStateQuit(bool tf);
		/*Return the bool value used to switch state to Quit
		If the function return true, we are changing state to Quit. Otherwise, this function should return false if we aren't chaging state.
		*/
		bool& IsSwitchingStateToQuit();

		bool& IsLoadingWorld();

		//Settings / Custom Functions for window

		// Get the current GLFW window width
		int GetGLFWWindowWidth();
		// Get the current GLFW window height
		int GetGLFWWindowHeight();
		//Set the window size value used to rescale the UI according to the GLFW window size
		void SetGLFWWindowSize(int width, int height);
		//Update the ratios (width and height) used to resacle the UI according to the GLFW window size
		void UpdateWidthAndHeightRatio();

		void SetButtonPos(PositionXButtonImGui position, PositionYButtonImGui positionY, int widthWindow, int heightWindow, int widthTexture, int heightTexture);

		void CreateMinecraftButton(PositionXButtonImGui positionX, PositionYButtonImGui positionY, ButtonSize buttonsize, GUISize GUISize, const char* label, ButtonLogic buttonLogic, World& world);

		void ApplyButtonLogic(ButtonLogic buttonLogic);
		void ApplyDragAndDropLogic(Item* source, Item* target, int indexSource, int indexTarget, World& world, bool isResultCrafting);

		void CreateMenuItemsBig(World& world);
		void CreateMenuItemsMedium(World& world);
		void CreateMenuItemsSmall(World& world);

		void CreateDragAndDropInventoryLogic(int n, World& world);
		void CreateDragAndDropInventoryBarLogic(int n, World& world);
		void CreateDragAndDropCraftingLogic(int n, World& world);
		void CreateDragAndDropCraftingResultLogic(int n, World& world);

		void CreateCountItemsBig(Item* slots, ImVec2 cursor, int n, World& world);
		void CreateCountItemsMedium(Item* slots, ImVec2 cursor, int n, World& world);
		void CreateCountItemsSmall(Item* slots, ImVec2 cursor, int n, World& world);

		void CreateItemsAndCountAndDDLogic(int n, World& world, Item* slots, Slots slotsLogic, float paddingX = 0.f, float paddingY = 0.f);

		void CreateInventoryTexturesMedium(float paddingX, float paddingY, int n, World& world);
		void CreateInventoryCountItemsMedium(float paddingX, float paddingY, int n, World& world);

		void CreateInventoryTexturesSmall(float paddingX, float paddingY, int n, World& world);
		void CreateInventoryCountItemsSmall(float paddingX, float paddingY, int n, World& world);

		//Other values

		void SetPlayerPos(vec3 position, Minecraft::vec2 rotation);

		void SetChunkPos(glm::ivec2 position);

		bool IsResultCrafting();

		void CheckForInventoryCrafting(World& world);

		void SetPreviousState(PreviousState state);
		PreviousState GetPreviousState();

	private:
		//System

		//Texture manager to load item's or background's PNG
		TextureManager& m_textureManager;

		//Bool windows

		// Passing this bool to true will display the inventory, false will close it
		bool m_showInventoryWindows = false;
		// Passing this bool to true will display the main menu, false will close it
		bool m_showMainMenuWindows = false;
		// Passing this bool to true will display the settings menu, false will close it
		bool m_showSettingsWindows = false;
		// Passing this bool to true will display the debug menu, false will close it
		bool m_showDebugWindows = false;
		// Passing this bool to true will display the debug menu, false will close it
		bool m_showGuiInGameWindows = false;
		// Passing this bool to true will display the debug menu, false will close it
		bool m_showSelectWorldMenuWindows = false;

		bool m_showPauseWindow = false;

		//Settings for window

		// If UpdateGLFWWindowSize() is called in the main loop, this is the current GLFW window width
		int m_glfwWindowWidth = -1;
		// If UpdateGLFWWindowSize() is called in the main loop, this is the current GLFW window height
		int m_glfwWindowHeight = -1;

		/* Width ratio according to GLFW window size to rescale the UI properly
		Updated with the function UpdateWidthAndHeightRatio()
		*/
		float m_widthRatio = 0.f;
		/* Height ratio according to GLFW window size to rescale the UI properly
		Updated with the function UpdateWidthAndHeightRatio()
		*/
		float m_heightRatio = 0.f;

		// Ratio to rescale the UI according to the option chosen in the game (Small (1) / Medium (2) / Big (3))
		float m_scaleUIRatio = 3.f;

		//Current state of GUI scale ratio
		int m_GUIScaleRatioIndex = 3;

		bool m_isGUIScaleAutoActivated = false;

		bool m_lockWindowScaleHeightBig = false;
		bool m_lockWindowScaleHeightMedium = false;

		bool m_lockWindowScaleWidthBig = false;
		bool m_lockWindowScaleWidthMedium = false;

		//Bool values used to switch state in Minecraft.cpp

		/*bool value used to switch state to the Main Menu
		Can be set with UI.SetSwitchStateMenu(boolean)
		*/
		bool m_switchToStateMenu = false;
		/*bool value used to switch state to Play
		Can be set with UI.SetSwitchStatePlay(boolean)
		*/
		bool m_switchToStatePlay = false;
		/*bool value used to switch state to the Settings
		Can be set with UI.SetSwitchStateSettings(boolean)
		*/
		bool m_switchToStateSettings = false;
		/*bool value used to switch state to Quit
		Can be set with UI.SetSwitchStateQuit(boolean)
		*/
		bool m_switchToStateQuit = false;

		float frames[FRAME_COUNT_SIZE];

		void GenericGraph(float* data, int dataSize, ImVec2 graphSize);

		Minecraft::vec3 m_playerPos;
		Minecraft::vec2 m_playerRot;
		glm::ivec2 m_chunkPos;

		const char* chunkGenPasses[5] = { "Stone / Water", "Biome / Surface", "Caves / Structures", "Flora", "All Passes" };
		int currentGenPasses = 5;

		int currentLife = 10;
		const char* Life[20] = { "0.5", "1", "1.5", "2", "2.5", "3", "3.5", "4", "4.5", "5", "5.5", "6", "6.5", "7", "7.5", "8", "8.5", "9", "9.5", "10" };

		ImFont* m_minecraftFontBig;
		ImFont* m_minecraftFontMedium;
		ImFont* m_minecraftFontSmall;
		ImFont* m_minecraftFontRegular;

		std::unordered_map<std::string, Texture> m_textureMap;

		CurrentDragFrom m_currentDragFrom;

		bool m_isResultCrafting = false;

		PreviousState m_previousState;

		bool m_loadWorld = false;
	};
}