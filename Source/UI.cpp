#include "UI.h"
#include "Inventory.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui_internal.h"
#include "glad/glad.h"
#include <assert.h>

#include <sstream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

Minecraft::UI::UI(TextureManager& textureManager) :
	m_textureManager(textureManager)
{
	ImGuiIO& io = ImGui::GetIO();
	m_minecraftFontRegular = io.Fonts->AddFontFromFileTTF("assets/UI/fonts/DefaultMinecraftFont/MinecraftRegular-Bmg3.otf", 16);
	m_minecraftFontSmall = io.Fonts->AddFontFromFileTTF("assets/UI/fonts/DefaultMinecraftFont/MinecraftRegular-Bmg3.otf", 20);
	m_minecraftFontMedium = io.Fonts->AddFontFromFileTTF("assets/UI/fonts/DefaultMinecraftFont/MinecraftRegular-Bmg3.otf", 40);
	m_minecraftFontBig = io.Fonts->AddFontFromFileTTF("assets/UI/fonts/DefaultMinecraftFont/MinecraftRegular-Bmg3.otf", 50);

	m_textureMap["regularButton"] = m_textureManager.GetTexture("assets/UI/regular_button.png");
	m_textureMap["hoveredButton"] = m_textureManager.GetTexture("assets/UI/hovered_button.png");
}

void Minecraft::UI::Update(Minecraft::World& world)
{
	//If we can't access to the GLFW window size, then stop the program
	IM_ASSERT(m_glfwWindowWidth != -1 || m_glfwWindowHeight != -1 && "The UI can't get the GLFW window size to scale and display the UI. Did you call UI.UpdateGLFWWindowSize(window) in the main loop of the application ?");

	UpdateWidthAndHeightRatio();

	InitImGuiWindows();

	CreateDebugWindow();

	CreateMainMenuBackgroundWindow();
	CreateMainMenuWindow(world);

	CreateGUIInGame(world);

	CreateInventoryWindow(world);
	CreateInventoryBackgroundWindow();

	CreateSelectWorldMenuBackgroundWindow();
	CreateSelectWorldMenuWindow(world);

	CreateSettingsBackgroundWindow();
	CreateSettingsWindow(world);

	CreateDebugWindowsInGame(world);

	CreatePauseWindow(world);

	RenderImGuiWindows();

	CheckForInventoryCrafting(world);

	//Update the inventory UI when we're out of items
	for (int i = 0; i < 9; i++)
	{
		if (world.GetPlayer().GetInventory().GetItemInventoryBarArray()[i].GetItemNameEnum() != ItemNameAvailable::NONE)
		{
			if (world.GetPlayer().GetInventory().GetItemInventoryBarArray()[i].GetCountItem() == 0)
			{
				world.GetPlayer().GetInventory().GetItemInventoryBarArray()[i].SwitchToNoneItem();
				world.GetPlayer().SetCurrentItem((int)ItemNameAvailable::NONE);
			}
		}
	}

	world.GetPlayer().SetCurrentItem((int)world.GetPlayer().GetInventory().GetItemInventoryBarArray()[world.GetPlayer().GetInventoryBarItemIndex()].GetItemNameEnum());
}

void Minecraft::UI::UpdateGLFWWindowSize(GLFWwindow* window)
{
	int widthWindow;
	int heightWindow;
	glfwGetWindowSize(window, &widthWindow, &heightWindow);

	if (widthWindow != m_glfwWindowWidth && m_glfwWindowWidth != -1)
	{
		if (widthWindow >= 1280 && m_lockWindowScaleWidthBig == false)
		{
			std::cout << "BIG";
			m_lockWindowScaleHeightBig = false;
			m_lockWindowScaleHeightMedium = false;
			m_lockWindowScaleWidthBig = false;
			m_lockWindowScaleWidthMedium = false;

			m_scaleUIRatio = 3.f;
			m_GUIScaleRatioIndex = 3;
		}
		else if ((widthWindow < 1280 && widthWindow >= 850) && m_lockWindowScaleWidthMedium == false)
		{
			std::cout << "Medium";
			m_lockWindowScaleHeightBig = true;
			m_scaleUIRatio = 2.f;
			m_GUIScaleRatioIndex = 2;
		}
		else if (widthWindow < 850)
		{
			std::cout << "Small";
			m_lockWindowScaleHeightBig = true;
			m_lockWindowScaleHeightMedium = true;
			m_lockWindowScaleWidthBig = false;

			m_scaleUIRatio = 1.f;
			m_GUIScaleRatioIndex = 1;
		}
	}
	else if (heightWindow != m_glfwWindowHeight && m_glfwWindowHeight != -1)
	{
		if (heightWindow >= 960 && m_lockWindowScaleHeightBig == false)
		{
			std::cout << "BIG";
			m_lockWindowScaleHeightBig = false;
			m_lockWindowScaleHeightMedium = false;
			m_lockWindowScaleWidthBig = false;
			m_lockWindowScaleWidthMedium = false;
			m_scaleUIRatio = 3.f;
			m_GUIScaleRatioIndex = 3;
		}
		else if ((heightWindow < 960 && heightWindow >= 720) && m_lockWindowScaleHeightMedium == false)
		{
			std::cout << "Medium";
			m_lockWindowScaleWidthBig = true;
			m_scaleUIRatio = 2.f;
			m_GUIScaleRatioIndex = 2;
		}
		else if (heightWindow < 720)
		{
			std::cout << "Small";
			m_lockWindowScaleWidthBig = true;
			m_lockWindowScaleWidthMedium = true;
			m_lockWindowScaleHeightBig = false;

			m_scaleUIRatio = 1.f;
			m_GUIScaleRatioIndex = 1;
		}
	}

	SetGLFWWindowSize(widthWindow, heightWindow);
}

void Minecraft::UI::InitImGuiWindows()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
}

void Minecraft::UI::RenderImGuiWindows()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Minecraft::UI::CreateInventoryBackgroundWindow()
{
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
	ImGui::SetNextWindowSize(ImVec2((float)m_glfwWindowWidth, (float)m_glfwWindowHeight));

	if (m_showInventoryWindows == true)
	{
		ImGui::Begin("Main Inventory Background", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		Texture inventoryBackground = m_textureManager.GetTexture("assets/UI/Inventory.png");

		if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventoryBackground.GetTextureWidth() / 2 * 2.5f, (float)m_glfwWindowHeight / 2 - inventoryBackground.GetTextureHeight() / 2 * 2.5f));
			ImGui::Image((void*)(intptr_t)inventoryBackground.GetTextureID(), ImVec2((float)(inventoryBackground.GetTextureWidth() * 2.5f), (float)(inventoryBackground.GetTextureHeight() * 2.5f)));
		}
		else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventoryBackground.GetTextureWidth() / 2 * 1.7f, (float)m_glfwWindowHeight / 2 - inventoryBackground.GetTextureHeight() / 2 * 1.7f));
			ImGui::Image((void*)(intptr_t)inventoryBackground.GetTextureID(), ImVec2((float)(inventoryBackground.GetTextureWidth() * 1.7f), (float)(inventoryBackground.GetTextureHeight() * 1.7f)));
		}
		else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventoryBackground.GetTextureWidth() / 2, (float)m_glfwWindowHeight / 2 - inventoryBackground.GetTextureHeight() / 2));
			ImGui::Image((void*)(intptr_t)inventoryBackground.GetTextureID(), ImVec2((float)(inventoryBackground.GetTextureWidth()), (float)(inventoryBackground.GetTextureHeight())));
		}

		ImGui::End();
	}
}

void Minecraft::UI::CreateInventoryWindow(Minecraft::World& world)
{
	if (m_showInventoryWindows == true)
	{
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		ImGui::SetNextWindowSize(ImVec2((float)m_glfwWindowWidth, (float)m_glfwWindowHeight));

		ImGui::Begin("Main Inventory", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::SetWindowFocus();
		float paddingXImage = 0.f;
		float paddingYImage = 0.f;
		float paddingXImage2 = 0.f;
		float paddingYImage2 = 0.f;

		if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
		{
			paddingXImage = -134.4f;
			paddingYImage = 0.f;
		}
		else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
		{
			paddingXImage = 20.5f;
			paddingYImage = 0.f;
		}
		else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
		{
			paddingXImage = 157.5f;
			paddingYImage = 0.f;
		}

		for (int n = 0; n < 27; n++)
		{
			ImGui::PushID(n);

			if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
			{
				paddingXImage += 89.9f;
				if (n % 9 == 0 && n != 0)
				{
					paddingXImage = -44.5f;
					paddingYImage += 89.4f;
				}

				CreateItemsAndCountAndDDLogic(n, world, world.GetPlayer().GetInventory().GetItemInventoryArray(), Slots::INVENTORY, paddingXImage, paddingYImage);
			}
			else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
			{
				paddingXImage += 61.1f;
				if (n % 9 == 0 && n != 0)
				{
					paddingXImage = 81.f;
					paddingYImage += 60.9f;
				}

				CreateItemsAndCountAndDDLogic(n, world, world.GetPlayer().GetInventory().GetItemInventoryArray(), Slots::INVENTORY, paddingXImage, paddingYImage);
			}
			else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
			{
				paddingXImage += 36.f;
				if (n % 9 == 0 && n != 0)
				{
					paddingXImage = 193.5f;
					paddingYImage += 35.9f;
				}
				CreateItemsAndCountAndDDLogic(n, world, world.GetPlayer().GetInventory().GetItemInventoryArray(), Slots::INVENTORY, paddingXImage, paddingYImage);
			}

			ImGui::PopID();
		}

		if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
		{
			paddingXImage = -134.4f;
			paddingYImage += 111.f;
		}
		else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
		{
			paddingXImage = 20.5f;
			paddingYImage += 72.f;
		}
		else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
		{
			paddingXImage = 157.5f;
			paddingYImage += 40.f;
		}

		for (int n4 = 0; n4 < 9; n4++)
		{
			ImGui::PushID(n4);

			if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
			{
				paddingXImage += 89.9f;

				CreateItemsAndCountAndDDLogic(n4, world, world.GetPlayer().GetInventory().GetItemInventoryBarArray(), Slots::INVENTORY_BAR, paddingXImage, paddingYImage);
			}
			else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
			{
				paddingXImage += 61.1f;

				CreateItemsAndCountAndDDLogic(n4, world, world.GetPlayer().GetInventory().GetItemInventoryBarArray(), Slots::INVENTORY_BAR, paddingXImage, paddingYImage);
			}
			else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
			{
				paddingXImage += 36.f;

				CreateItemsAndCountAndDDLogic(n4, world, world.GetPlayer().GetInventory().GetItemInventoryBarArray(), Slots::INVENTORY_BAR, paddingXImage, paddingYImage);
			}

			ImGui::PopID();
		}

		for (int n2 = 0; n2 < 4; n2++)
		{
			ImGui::PushID(n2);

			if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
			{
				paddingXImage2 += 89.9f;
				if (n2 % 2 == 0 && n2 != 0)
				{
					paddingXImage2 = 89.9f;
					paddingYImage2 += 89.4f;
				}

				CreateItemsAndCountAndDDLogic(n2, world, world.GetPlayer().GetInventory().GetItemCraftArray(), Slots::CRAFTING, paddingXImage2, paddingYImage2);
			}
			else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
			{
				paddingXImage2 += 61.1f;
				if (n2 % 2 == 0 && n2 != 0)
				{
					paddingXImage2 = 62.9f;
					paddingYImage2 += 59.4f;
				}

				CreateItemsAndCountAndDDLogic(n2, world, world.GetPlayer().GetInventory().GetItemCraftArray(), Slots::CRAFTING, paddingXImage2, paddingYImage2);
			}
			else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
			{
				paddingXImage2 += 36.f;
				if (n2 % 2 == 0 && n2 != 0)
				{
					paddingXImage2 = 35.9f;
					paddingYImage2 += 35.4f;
				}

				CreateItemsAndCountAndDDLogic(n2, world, world.GetPlayer().GetInventory().GetItemCraftArray(), Slots::CRAFTING, paddingXImage2, paddingYImage2);
			}

			ImGui::PopID();
		}

		for (int n3 = 0; n3 < 1; n3++)
		{
			ImGui::PushID(n3);

			if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
			{
				CreateItemsAndCountAndDDLogic(n3, world, world.GetPlayer().GetInventory().GetItemCraftResultArray(), Slots::CRAFTING_RESULT);
			}
			else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
			{
				CreateItemsAndCountAndDDLogic(n3, world, world.GetPlayer().GetInventory().GetItemCraftResultArray(), Slots::CRAFTING_RESULT);
			}
			else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
			{
				CreateItemsAndCountAndDDLogic(n3, world, world.GetPlayer().GetInventory().GetItemCraftResultArray(), Slots::CRAFTING_RESULT);
			}

			ImGui::PopID();
		}
		ImGui::End();
	}
}

void Minecraft::UI::CreateDebugWindow()
{
	//ImGui::ShowDemoWindow();
	//ImGui::ShowStyleEditor();
}

void Minecraft::UI::CreateMainMenuBackgroundWindow()
{
	if (m_showMainMenuWindows == true)
	{
		//Needed the adjustement and put -10.f on width and height because otherwise it won't display properly at the top left of the screen
		ImGui::SetNextWindowPos(ImVec2(-10.f, -10.f));
		ImGui::SetNextWindowSize(ImVec2((float)m_glfwWindowWidth + 15.f, (float)m_glfwWindowHeight + 15.f));

		Texture mainMenuBackground = m_textureManager.GetTexture("assets/UI/Background_Main_Menu.jpg");
		ImGui::Begin("Main Menu Background", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::Image((void*)(intptr_t)mainMenuBackground.GetTextureID(), ImVec2((float)(m_glfwWindowWidth)+15.f, (float)(m_glfwWindowHeight)+15.f));

		ImGui::End();
	}
}

void Minecraft::UI::CreateMainMenuWindow(World& world)
{
	if (m_showMainMenuWindows == true)
	{
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		ImGui::SetNextWindowSize(ImVec2((float)m_glfwWindowWidth, (float)m_glfwWindowHeight));

		ImGui::Begin("MainMenu", NULL, ImGuiNextWindowDataFlags_HasFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));

		if (m_GUIScaleRatioIndex == 3)
		{
			CreateMenuItemsBig(world);
		}
		else if (m_GUIScaleRatioIndex == 2)
		{
			CreateMenuItemsMedium(world);
		}
		else if (m_GUIScaleRatioIndex == 1)
		{
			CreateMenuItemsSmall(world);
		}

		ImGui::PopStyleColor(2);

		ImGui::End();
	}
}

void Minecraft::UI::CreateSelectWorldMenuBackgroundWindow()
{
	if (m_showSelectWorldMenuWindows == true)
	{
		//Needed the adjustement and put -10.f on width and height because otherwise it won't display properly at the top left of the screen
		ImGui::SetNextWindowPos(ImVec2(-10.f, -10.f));
		ImGui::SetNextWindowSize(ImVec2((float)m_glfwWindowWidth + 15.f, (float)m_glfwWindowHeight + 15.f));

		ImGui::Begin("Select World Background", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		Texture settingsBackground = m_textureManager.GetTexture("assets/UI/Dirt_Background_Options.png");

		ImGui::Image((void*)(intptr_t)settingsBackground.GetTextureID(), ImVec2((float)(m_glfwWindowWidth)+15.f, (float)(m_glfwWindowHeight)+15.f));
		ImGui::End();
	}
}

void Minecraft::UI::CreateSelectWorldMenuWindow(World& world)
{
	if (m_showSelectWorldMenuWindows == true)
	{
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		ImGui::SetNextWindowSize(ImVec2((float)m_glfwWindowWidth, (float)m_glfwWindowHeight));

		ImGui::Begin("Select World Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));

		if (m_GUIScaleRatioIndex == 3)
		{
			ImGui::PushFont(m_minecraftFontBig);

			CreateMinecraftButton(PositionXButtonImGui::CENTER_LEFT, PositionYButtonImGui::SEVENTH_LINE, ButtonSize::SMALL, GUISize::BIG, "Create New World", ButtonLogic::CREATE_NEW_WORLD, world);

			CreateMinecraftButton(PositionXButtonImGui::CENTER_RIGHT, PositionYButtonImGui::SEVENTH_LINE, ButtonSize::SMALL, GUISize::BIG, "Load World", ButtonLogic::LOAD_WORLD, world);
			ImGui::PopFont();
		}
		else if (m_GUIScaleRatioIndex == 2)
		{
			ImGui::PushFont(m_minecraftFontMedium);

			CreateMinecraftButton(PositionXButtonImGui::CENTER_LEFT, PositionYButtonImGui::SEVENTH_LINE, ButtonSize::SMALL, GUISize::MEDIUM, "Create New World", ButtonLogic::CREATE_NEW_WORLD, world);

			CreateMinecraftButton(PositionXButtonImGui::CENTER_RIGHT, PositionYButtonImGui::SEVENTH_LINE, ButtonSize::SMALL, GUISize::MEDIUM, "Load World", ButtonLogic::LOAD_WORLD, world); \
				ImGui::PopFont();
		}
		else if (m_GUIScaleRatioIndex == 1)
		{
			ImGui::PushFont(m_minecraftFontSmall);

			CreateMinecraftButton(PositionXButtonImGui::CENTER_LEFT, PositionYButtonImGui::SEVENTH_LINE, ButtonSize::SMALL, GUISize::SMALL, "Create New World", ButtonLogic::CREATE_NEW_WORLD, world);

			CreateMinecraftButton(PositionXButtonImGui::CENTER_RIGHT, PositionYButtonImGui::SEVENTH_LINE, ButtonSize::SMALL, GUISize::SMALL, "Load World", ButtonLogic::LOAD_WORLD, world);
			ImGui::PopFont();
		}

		ImGui::PopStyleColor(2);

		ImGui::End();
	}
}

void Minecraft::UI::CreatePauseWindow(World& world)
{
	if (m_showPauseWindow == true)
	{
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		ImGui::SetNextWindowSize(ImVec2((float)m_glfwWindowWidth, (float)m_glfwWindowHeight));

		ImGui::Begin("Pause", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));

		if (m_GUIScaleRatioIndex == 3)
		{
			ImGui::PushFont(m_minecraftFontBig);

			CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::FIFTH_LINE, ButtonSize::BIG, GUISize::BIG, "Options", ButtonLogic::OPTIONS, world);

			CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::SIXTH_LINE, ButtonSize::BIG, GUISize::BIG, "Return To Menu", ButtonLogic::RETURN_TO_MENU, world);
			ImGui::PopFont();
		}
		else if (m_GUIScaleRatioIndex == 2)
		{
			ImGui::PushFont(m_minecraftFontMedium);

			CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::FIFTH_LINE, ButtonSize::BIG, GUISize::MEDIUM, "Options", ButtonLogic::OPTIONS, world);

			CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::SIXTH_LINE, ButtonSize::BIG, GUISize::MEDIUM, "Return To Menu", ButtonLogic::RETURN_TO_MENU, world);
			ImGui::PopFont();
		}
		else if (m_GUIScaleRatioIndex == 1)
		{
			ImGui::PushFont(m_minecraftFontSmall);

			CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::FIFTH_LINE, ButtonSize::BIG, GUISize::SMALL, "Options", ButtonLogic::OPTIONS, world);

			CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::SIXTH_LINE, ButtonSize::BIG, GUISize::SMALL, "Return To Menu", ButtonLogic::RETURN_TO_MENU, world);
			ImGui::PopFont();
		}
		ImGui::PopStyleColor(2);

		ImGui::End();
	}
}

void Minecraft::UI::CreateSettingsBackgroundWindow()
{
	if (m_showSettingsWindows == true)
	{
		//Needed the adjustement and put -10.f on width and height because otherwise it won't display properly at the top left of the screen
		ImGui::SetNextWindowPos(ImVec2(-10.f, -10.f));
		ImGui::SetNextWindowSize(ImVec2((float)m_glfwWindowWidth + 15.f, (float)m_glfwWindowHeight + 15.f));

		ImGui::Begin("Settings Background", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		Texture settingsBackground = m_textureManager.GetTexture("assets/UI/Dirt_Background_Options.png");

		ImGui::Image((void*)(intptr_t)settingsBackground.GetTextureID(), ImVec2((float)(m_glfwWindowWidth)+15.f, (float)(m_glfwWindowHeight)+15.f));
		ImGui::End();
	}
}

void Minecraft::UI::CreateSettingsWindow(World& world)
{
	// @TODO TO REDO

	if (m_showSettingsWindows == true)
	{
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		ImGui::SetNextWindowSize(ImVec2((float)m_glfwWindowWidth, (float)m_glfwWindowHeight));

		ImGui::Begin("Settings Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));

		if (m_GUIScaleRatioIndex == 3)
		{
			ImGui::PushFont(m_minecraftFontBig);
			CreateMinecraftButton(PositionXButtonImGui::CENTER_LEFT, PositionYButtonImGui::SECOND_LINE, ButtonSize::BIG, GUISize::BIG, "GUI Scale : 3", ButtonLogic::GUI_SCALE_3, world);

			CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::HEIGHTTH_LINE, ButtonSize::BIG, GUISize::BIG, "Done", ButtonLogic::DONE_SETTINGS, world);
			ImGui::PopFont();
		}
		else if (m_GUIScaleRatioIndex == 2)
		{
			ImGui::PushFont(m_minecraftFontMedium);
			CreateMinecraftButton(PositionXButtonImGui::CENTER_LEFT, PositionYButtonImGui::SECOND_LINE, ButtonSize::BIG, GUISize::MEDIUM, "GUI Scale : 2", ButtonLogic::GUI_SCALE_2, world);

			CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::HEIGHTTH_LINE, ButtonSize::BIG, GUISize::MEDIUM, "Done", ButtonLogic::DONE_SETTINGS, world);
			ImGui::PopFont();
		}
		else if (m_GUIScaleRatioIndex == 1)
		{
			ImGui::PushFont(m_minecraftFontSmall);
			CreateMinecraftButton(PositionXButtonImGui::CENTER_LEFT, PositionYButtonImGui::SECOND_LINE, ButtonSize::BIG, GUISize::SMALL, "GUI Scale : 1", ButtonLogic::GUI_SCALE_1, world);

			CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::HEIGHTTH_LINE, ButtonSize::BIG, GUISize::SMALL, "Done", ButtonLogic::DONE_SETTINGS, world);
			ImGui::PopFont();
		}

		ImGui::PopStyleColor(2);

		ImGui::End();
	}
}

void Minecraft::UI::FPSCounter()
{
	float fps = ImGui::GetIO().Framerate;

	for (int i = 1; i < FRAME_COUNT_SIZE; i++)
		frames[i - 1] = frames[i];

	frames[FRAME_COUNT_SIZE - 1] = fps;

	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMouseInputs |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoResize;

	ImGui::Begin("FPS Graph", NULL, flags);

	ImGui::SetWindowPos(ImVec2(0, 0));

	std::ostringstream text;
	text.precision(2);
	text << "FPS: " << std::fixed << fps;

	ImGui::Text(text.str().c_str());

	GenericGraph(&frames[0], FRAME_COUNT_SIZE, ImVec2(75, 30));

	ImGui::End();
}

void Minecraft::UI::PlayerPos(Minecraft::World& /*world*/)
{
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMouseInputs |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoResize;

	ImGui::Begin("PlayerPos", NULL, flags);

	ImGui::SetWindowPos(ImVec2(0, 50));
	ImGui::SetWindowSize(ImVec2(110, 200));
	{
		std::ostringstream text;
		text << "PlayerPos: \n";
		text << "X " << m_playerPos.x << "\n";
		text << "Y " << m_playerPos.y << "\n";
		text << "Z " << m_playerPos.z << "\n";
		text << "PlayerRot: \n";
		text << "X " << m_playerRot.x << "\n";
		text << "Y " << m_playerRot.y << "\n";

		ImGui::Text(text.str().c_str());
	}

	std::ostringstream chunkText;
	chunkText << "Chunk: \n(" << m_chunkPos.x << ", " << m_chunkPos.y << ")";
	ImGui::Text(chunkText.str().c_str());

	ImGui::End();
}

void Minecraft::UI::RegenWorld(Minecraft::World& world)
{
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoResize;

	ImGui::Begin("RegenWorld", NULL, flags);

	ImGui::SetWindowPos(ImVec2(0, 210));
	ImGui::SetWindowSize(ImVec2(400, 400));

	bool regenNeeded = false;

	regenNeeded |= ImGui::InputInt("Seed", &world.m_noise.seed);
	regenNeeded |= ImGui::DragIntRange2("Height Clamping", &world.m_noise.minHeight, &world.m_noise.maxHeight, 1.f, 0, 255);
	regenNeeded |= ImGui::SliderFloat("Frequency", &world.m_noise.frequency, 0.01f, 5.0f);
	regenNeeded |= ImGui::SliderFloat("Redistribution", &world.m_noise.redistribution, 0.01f, 10.0f);
	regenNeeded |= ImGui::SliderFloat("Caves X/Z Frequency", &world.m_noise.cavesXZFrequency, 0.01f, 10.0f);
	regenNeeded |= ImGui::SliderFloat("Caves Y Frequency", &world.m_noise.cavesYFrequency, 0.01f, 10.0f);
	regenNeeded |= ImGui::SliderFloat("Caves Threshold", &world.m_noise.cavesThreshold, 0.01f, 1.0f);

	ImGui::Text("Chunk Generation Passes");
	regenNeeded |= ImGui::ListBox("", &currentGenPasses, chunkGenPasses, 5);

	if (regenNeeded)
	{
		world.m_noise.offsetsGenerated = false;
		world.passes = (ChunkGenPasses)currentGenPasses;
		world.GenerateWorld();
	}

	ImGui::End();
}

void Minecraft::UI::ChangeHPPlayer(World& world)
{
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoResize;

	ImGui::Begin("ChangeHpPlayer", NULL, flags);

	ImGui::SetWindowPos(ImVec2(0, 550));
	ImGui::SetWindowSize(ImVec2(250, 250));

	ImGui::Text("Change Hp Player");

	currentLife = (int)(world.GetPlayer().GetLife() * 2.f);

	if(ImGui::SliderInt("", &currentLife, 0, 20))
		world.GetPlayer().SetLife(currentLife / 2.f);

	ImGui::End();
}

void Minecraft::UI::CreateDebugWindowsInGame(Minecraft::World& world)
{
	if (m_showDebugWindows == true)
	{
		FPSCounter();
		PlayerPos(world);
		RegenWorld(world);
		ChangeHPPlayer(world);
	}
}

void Minecraft::UI::CreateGUIInGame(Minecraft::World& world)
{
	if (m_showGuiInGameWindows == true)
	{
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		ImGui::SetNextWindowSize(ImVec2((float)m_glfwWindowWidth, (float)m_glfwWindowHeight));

		ImGui::Begin("Main GUI in game", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		Texture crosshair = m_textureManager.GetTexture("assets/UI/crosshair.png");
		Texture inventory_bar = m_textureManager.GetTexture("assets/UI/inventory_bar.png");
		Texture heart = m_textureManager.GetTexture("assets/UI/heart.png");
		Texture half_heart = m_textureManager.GetTexture("assets/UI/half_heart.png");
		Texture placeholder_heart = m_textureManager.GetTexture("assets/UI/placeholder_heart.png");
		Texture selectedSlotInventoryBar = m_textureManager.GetTexture("assets/UI/selected_slot_inventory_bar.png");

		float paddingXImage = -68.f;
		float paddingYImage = 12.f;

		ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - crosshair.GetTextureWidth() / 2, (float)m_glfwWindowHeight / 2 - crosshair.GetTextureHeight() / 2));
		ImGui::Image((void*)(intptr_t)crosshair.GetTextureID(), ImVec2((float)crosshair.GetTextureWidth(), (float)crosshair.GetTextureHeight())/*, ImVec2(0,0), ImVec2(1, 1), ImVec4(0.1f, 0.6f, 0.2f, 1.f)*/);

		ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f));
		ImGui::Image((void*)(intptr_t)inventory_bar.GetTextureID(), ImVec2((float)inventory_bar.GetTextureWidth() * 2.f, (float)inventory_bar.GetTextureHeight() * 2.f));

		ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f + 80.f * world.GetPlayer().GetInventoryBarItemIndex() + 1, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f));
		ImGui::Image((void*)(intptr_t)selectedSlotInventoryBar.GetTextureID(), ImVec2((float)42 * 2.f, (float)42 * 2.f));

		for (int n = 0; n < 9; n++)
		{
			paddingXImage += 80.f;
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f + paddingXImage, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f + paddingYImage));
			ImVec2 cursor = ImGui::GetCursorPos();
			ImGui::Image((void*)(intptr_t)world.GetPlayer().GetInventory().GetItemInventoryBarArray()[n].GetTexture().GetTextureID(), ImVec2(60.f, 60.f));
			ImGui::PushFont(m_minecraftFontBig);
			if (world.GetPlayer().GetInventory().GetItemInventoryBarArray()[n].GetItemNameEnum() != ItemNameAvailable::NONE && world.GetPlayer().GetInventory().GetItemInventoryBarArray()[n].GetToolType() == Tools::NONE)
			{
				if (world.GetPlayer().GetInventory().GetItemInventoryBarArray()[n].GetCountItem() < 10)
				{
					ImGui::SetCursorPos(ImVec2(cursor.x + 17.f, cursor.y + 15.f));

					ImGui::Text("%d", world.GetPlayer().GetInventory().GetItemInventoryBarArray()[n].GetCountItem());
				}
				else if (world.GetPlayer().GetInventory().GetItemInventoryBarArray()[n].GetCountItem() < 100 && world.GetPlayer().GetInventory().GetItemInventoryBarArray()[n].GetCountItem() >= 10)
				{
					ImGui::SetCursorPos(ImVec2(cursor.x + 7.5f, cursor.y + 15.f));
					ImGui::Text("%d", world.GetPlayer().GetInventory().GetItemInventoryBarArray()[n].GetCountItem());
				}
				else
				{
					ImGui::SetCursorPos(ImVec2(cursor.x - 5.f, cursor.y + 15.f));
					ImGui::Text("%d", world.GetPlayer().GetInventory().GetItemInventoryBarArray()[n].GetCountItem());
				}
			}
			ImGui::PopFont();
		}

		float heartToRemove = 10 - world.GetPlayer().GetLife();
		int checkheartfull = 1;

		for (float i = 1; i <= 10; i += 0.5f)
		{
			if (i == checkheartfull)
			{
				if (i == 1)
				{
					ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f - 40.f));
				}
				else if (i == 2)
				{
					ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f + heart.GetTextureWidth() * 2, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f - 40.f));
				}
				else
				{
					ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f + heart.GetTextureWidth() * (i - 2) + heart.GetTextureWidth() * i, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f - 40.f));
				}

				ImGui::Image((void*)(intptr_t)placeholder_heart.GetTextureID(), ImVec2((float)placeholder_heart.GetTextureWidth() * 2.f, (float)placeholder_heart.GetTextureHeight() * 2.f));
			}

			if (i == floorf(i))
			{
				checkheartfull++;
			}
		}

		checkheartfull = 1;

		if (10 - heartToRemove == 0.5f)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f - 40.f));
			ImGui::Image((void*)(intptr_t)half_heart.GetTextureID(), ImVec2((float)half_heart.GetTextureWidth() * 2.f, (float)half_heart.GetTextureHeight() * 2.f));
		}

		for (float i = 1; i <= 10 - heartToRemove; i += 0.5f)
		{
			if (i == checkheartfull)
			{
				if (i == 1)
				{
					ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f - 40.f));
				}
				else if (i == 2)
				{
					ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f + heart.GetTextureWidth() * 2, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f - 40.f));
				}
				else
				{
					ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f + heart.GetTextureWidth() * (i - 2) + heart.GetTextureWidth() * i, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f - 40.f));
				}

				ImGui::Image((void*)(intptr_t)heart.GetTextureID(), ImVec2((float)heart.GetTextureWidth() * 2.f, (float)heart.GetTextureHeight() * 2.f));
			}
			else if (i == 10 - heartToRemove)
			{
				if (i == 1.5f)
				{
					ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f + heart.GetTextureWidth() * 2, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f - 40.f));
				}
				else
				{
					ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - inventory_bar.GetTextureWidth() / 2 * 2.f + heart.GetTextureWidth() * (i - 2 + 1.f) + heart.GetTextureWidth() * i + 1.f, (float)m_glfwWindowHeight - inventory_bar.GetTextureHeight() * 2.f - 40.f));
				}

				ImGui::Image((void*)(intptr_t)half_heart.GetTextureID(), ImVec2((float)half_heart.GetTextureWidth() * 2.f, (float)half_heart.GetTextureHeight() * 2.f));
			}

			if (i == floorf(i))
			{
				checkheartfull++;
			}
		}

		ImGui::End();
	}
}

void Minecraft::UI::DisplayInventoryWindows()
{
	m_showInventoryWindows = true;
}

void Minecraft::UI::CloseInventoryWindows()
{
	m_showInventoryWindows = false;
}

void Minecraft::UI::DisplayMainMenuWindows()
{
	m_showMainMenuWindows = true;
}

void Minecraft::UI::CloseMainMenuWindows()
{
	m_showMainMenuWindows = false;
}

bool& Minecraft::UI::IsMainMenuWindowsDisplaying()
{
	return m_showMainMenuWindows;
}

void Minecraft::UI::DisplaySettingsWindows()
{
	m_showSettingsWindows = true;
}

void Minecraft::UI::CloseSettingsWindows()
{
	m_showSettingsWindows = false;
}

bool& Minecraft::UI::IsSettingsWindowsDisplaying()
{
	return m_showSettingsWindows;
}

void Minecraft::UI::DisplayDebugWindows()
{
	m_showDebugWindows = true;
}

void Minecraft::UI::CloseDebugWindows()
{
	m_showDebugWindows = false;
}

bool& Minecraft::UI::IsDebugWindowsDisplaying()
{
	return m_showDebugWindows;
}

void Minecraft::UI::DisplayGUIInGameWindows()
{
	m_showGuiInGameWindows = true;
}

void Minecraft::UI::CloseGUIInGameWindows()
{
	m_showGuiInGameWindows = false;
}

bool& Minecraft::UI::IsGUIInGameWindowsDisplaying()
{
	return m_showGuiInGameWindows;
}

void Minecraft::UI::DisplaySelectWorldMenuWindows()
{
	m_showSelectWorldMenuWindows = true;
}

void Minecraft::UI::CloseSelectWorldMenuWindows()
{
	m_showSelectWorldMenuWindows = false;
}

bool& Minecraft::UI::IsSelectWorldMenuWindowsDisplaying()
{
	return m_showSelectWorldMenuWindows;
}

void Minecraft::UI::DisplayPauseMenuWindows()
{
	m_showPauseWindow = true;
}

void Minecraft::UI::ClosePauseMenuWindows()
{
	m_showPauseWindow = false;
}

bool& Minecraft::UI::IsPauseWindowsDisplaying()
{
	return m_showPauseWindow;
}

void Minecraft::UI::SetSwitchStateMenu(bool tf)
{
	m_switchToStateMenu = tf;
}

bool& Minecraft::UI::IsSwitchingStateToMenu()
{
	return m_switchToStateMenu;
}

void Minecraft::UI::SetSwitchStatePlay(bool tf)
{
	m_switchToStatePlay = tf;
}

bool& Minecraft::UI::IsSwitchingStateToPlay()
{
	return m_switchToStatePlay;
}

void Minecraft::UI::SetSwitchStateSettings(bool tf)
{
	m_switchToStateSettings = tf;
}

bool& Minecraft::UI::IsSwitchingStateToSettings()
{
	return m_switchToStateSettings;
}

void Minecraft::UI::SetSwitchStateQuit(bool tf)
{
	m_switchToStateQuit = tf;
}

bool& Minecraft::UI::IsSwitchingStateToQuit()
{
	return m_switchToStateQuit;
}

bool& Minecraft::UI::IsInventoryWindowsDisplaying()
{
	return m_showInventoryWindows;
}

int Minecraft::UI::GetGLFWWindowWidth()
{
	return m_glfwWindowWidth;
}

int Minecraft::UI::GetGLFWWindowHeight()
{
	return m_glfwWindowHeight;
}

void Minecraft::UI::SetGLFWWindowSize(int width, int height)
{
	m_glfwWindowWidth = width;
	m_glfwWindowHeight = height;
}

void Minecraft::UI::UpdateWidthAndHeightRatio()
{
	float WidthBasis = 1920.f;
	float HeightBasis = 1080.f;

	m_widthRatio = (float)m_glfwWindowWidth / WidthBasis;
	m_heightRatio = (float)m_glfwWindowHeight / HeightBasis;
}

void Minecraft::UI::SetButtonPos(PositionXButtonImGui position, PositionYButtonImGui positionY, int widthWindow, int heightWindow, int widthTexture, int heightTexture)
{
	switch (position)
	{
	case Minecraft::PositionXButtonImGui::CENTER_LEFT:
		ImGui::SetCursorPosX(widthWindow / 2 - (float)widthTexture - 8.f);
		break;
	case Minecraft::PositionXButtonImGui::CENTER:
		ImGui::SetCursorPosX(widthWindow / 2 - (float)widthTexture / 2);
		break;
	case Minecraft::PositionXButtonImGui::CENTER_RIGHT:
		ImGui::SetCursorPosX(widthWindow / 2 + 8.f);
		break;
	default:
		break;
	}

	float half = (float)(heightWindow / 2);
	float quarterPlusPAdding = (float)(heightWindow / 8);
	float recenterHeight = (float)heightTexture / 2;

	switch (positionY)
	{
	case Minecraft::PositionYButtonImGui::FIRST_LINE:
		ImGui::SetCursorPosY(half - (quarterPlusPAdding * 4) - recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::SECOND_LINE:
		ImGui::SetCursorPosY(half - (quarterPlusPAdding * 3) - recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::THIRD_LINE:
		ImGui::SetCursorPosY(half - (quarterPlusPAdding * 2) - recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::FOURTH_LINE:
		ImGui::SetCursorPosY(half - (quarterPlusPAdding * 1) - recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::FIFTH_LINE:
		ImGui::SetCursorPosY(half - recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::SIXTH_LINE:
		ImGui::SetCursorPosY(half + (quarterPlusPAdding * 1) + recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::SEVENTH_LINE:
		ImGui::SetCursorPosY(half + (quarterPlusPAdding * 2) + recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::HEIGHTTH_LINE:
		ImGui::SetCursorPosY(half + (quarterPlusPAdding * 3) + recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::NINETH_LINE:
		ImGui::SetCursorPosY(half + (quarterPlusPAdding * 4) + recenterHeight);
		break;
	default:
		break;
	}
}

void Minecraft::UI::CreateMinecraftButton(PositionXButtonImGui positionX, PositionYButtonImGui positionY, ButtonSize buttonsize, GUISize GUISize, const char* label, ButtonLogic buttonLogic, World& world)
{
	float sizeMultiplicator = 0.f;

	switch (GUISize)
	{
	case Minecraft::GUISize::BIG:
		sizeMultiplicator = 2.f;
		break;
	case Minecraft::GUISize::MEDIUM:
		sizeMultiplicator = 1.5f;
		break;
	case Minecraft::GUISize::SMALL:
		sizeMultiplicator = 1.f;
		break;
	default:
		break;
	}

	switch (positionX)
	{
	case Minecraft::PositionXButtonImGui::CENTER_LEFT:
		if (buttonsize == ButtonSize::BIG)
		{
			ImGui::SetCursorPosX((float)m_glfwWindowWidth / 2 - (float)m_textureMap.find("regularButton")->second.GetTextureWidth() * sizeMultiplicator + 8.f);
		}
		else
		{
			ImGui::SetCursorPosX((float)m_glfwWindowWidth / 2 - (float)m_textureMap.find("regularButton")->second.GetTextureWidth() / 2 * sizeMultiplicator - 8.f);
		}
		break;
	case Minecraft::PositionXButtonImGui::CENTER:
		ImGui::SetCursorPosX((float)m_glfwWindowWidth / 2 - (float)m_textureMap.find("regularButton")->second.GetTextureWidth() / 2 * sizeMultiplicator);
		break;
	case Minecraft::PositionXButtonImGui::CENTER_RIGHT:
		ImGui::SetCursorPosX((float)m_glfwWindowWidth / 2 + 8.f);
		break;
	default:
		break;
	}

	float half = (float)(m_glfwWindowHeight / 2);
	float quarterPlusPAdding = (float)(m_glfwWindowHeight / 8);
	float recenterHeight = (float)m_textureMap.find("regularButton")->second.GetTextureHeight() / 2;

	switch (positionY)
	{
	case Minecraft::PositionYButtonImGui::FIRST_LINE:
		ImGui::SetCursorPosY(half - (quarterPlusPAdding * 4) - recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::SECOND_LINE:
		ImGui::SetCursorPosY(half - (quarterPlusPAdding * 3) - recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::THIRD_LINE:
		ImGui::SetCursorPosY(half - (quarterPlusPAdding * 2) - recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::FOURTH_LINE:
		ImGui::SetCursorPosY(half - (quarterPlusPAdding * 1) - recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::FIFTH_LINE:
		ImGui::SetCursorPosY(half - recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::SIXTH_LINE:
		ImGui::SetCursorPosY(half + (quarterPlusPAdding * 1) + recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::SEVENTH_LINE:
		ImGui::SetCursorPosY(half + (quarterPlusPAdding * 2) + recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::HEIGHTTH_LINE:
		ImGui::SetCursorPosY(half + (quarterPlusPAdding * 3) + recenterHeight);
		break;
	case Minecraft::PositionYButtonImGui::NINETH_LINE:
		ImGui::SetCursorPosY(half + (quarterPlusPAdding * 4) + recenterHeight);
		break;
	default:
		break;
	}

	ImVec2 cursorPosition = ImGui::GetCursorPos();

	switch (buttonsize)
	{
	case Minecraft::ButtonSize::BIG:
		if (ImGui::Button(label, ImVec2((float)m_textureMap.find("regularButton")->second.GetTextureWidth() * sizeMultiplicator, (float)m_textureMap.find("regularButton")->second.GetTextureHeight() * sizeMultiplicator)))
		{
			world.GetSoundEngine()->play2D("assets/sounds/Minecraft_Button_Sound_Effects.ogg", false);
			ApplyButtonLogic(buttonLogic);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetCursorPos(cursorPosition);
			ImGui::Image((void*)(intptr_t)m_textureMap.find("hoveredButton")->second.GetTextureID(), ImVec2((float)m_textureMap.find("hoveredButton")->second.GetTextureWidth() * sizeMultiplicator, (float)m_textureMap.find("hoveredButton")->second.GetTextureHeight() * sizeMultiplicator));

			ImGui::SetCursorPos(ImVec2(cursorPosition.x + (float)m_textureMap.find("hoveredButton")->second.GetTextureWidth() / 2 * sizeMultiplicator - ImGui::CalcTextSize(label).x / 2, cursorPosition.y + (float)m_textureMap.find("hoveredButton")->second.GetTextureHeight() / 2 * sizeMultiplicator - ImGui::CalcTextSize(label).y / 2 - 2.f));
			ImGui::TextColored(ImVec4(1, 1, 0.627f, 1), label);
		}
		else
		{
			ImGui::SetCursorPos(cursorPosition);
			ImGui::Image((void*)(intptr_t)m_textureMap.find("regularButton")->second.GetTextureID(), ImVec2((float)m_textureMap.find("regularButton")->second.GetTextureWidth() * sizeMultiplicator, (float)m_textureMap.find("regularButton")->second.GetTextureHeight() * sizeMultiplicator));
			ImGui::SetCursorPos(ImVec2(cursorPosition.x + (float)m_textureMap.find("hoveredButton")->second.GetTextureWidth() / 2 * sizeMultiplicator - ImGui::CalcTextSize(label).x / 2, cursorPosition.y + (float)m_textureMap.find("hoveredButton")->second.GetTextureHeight() / 2 * sizeMultiplicator - ImGui::CalcTextSize(label).y / 2 - 2.f));
			ImGui::Text(label);
		}
		break;
	case Minecraft::ButtonSize::SMALL:
		if (ImGui::Button(label, ImVec2((float)m_textureMap.find("regularButton")->second.GetTextureWidth() / 2 * sizeMultiplicator, (float)m_textureMap.find("regularButton")->second.GetTextureHeight() * sizeMultiplicator)))
		{
			world.GetSoundEngine()->play2D("assets/sounds/Minecraft_Button_Sound_Effects.ogg", false);
			ApplyButtonLogic(buttonLogic);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetCursorPos(cursorPosition);
			ImGui::Image((void*)(intptr_t)m_textureMap.find("hoveredButton")->second.GetTextureID(), ImVec2((float)m_textureMap.find("hoveredButton")->second.GetTextureWidth() / 2 * sizeMultiplicator, (float)m_textureMap.find("hoveredButton")->second.GetTextureHeight() * sizeMultiplicator));
			ImGui::SetCursorPos(ImVec2(cursorPosition.x + (float)m_textureMap.find("hoveredButton")->second.GetTextureWidth() / 2 * sizeMultiplicator / 2 - ImGui::CalcTextSize(label).x / 2, cursorPosition.y + (float)m_textureMap.find("hoveredButton")->second.GetTextureHeight() / 2 * sizeMultiplicator - ImGui::CalcTextSize(label).y / 2 - 2.f));
			ImGui::TextColored(ImVec4(1, 1, 0.627f, 1), label);
		}
		else
		{
			ImGui::SetCursorPos(cursorPosition);
			ImGui::Image((void*)(intptr_t)m_textureMap.find("regularButton")->second.GetTextureID(), ImVec2((float)m_textureMap.find("regularButton")->second.GetTextureWidth() / 2 * sizeMultiplicator, (float)m_textureMap.find("regularButton")->second.GetTextureHeight() * sizeMultiplicator));
			ImGui::SetCursorPos(ImVec2(cursorPosition.x + (float)m_textureMap.find("hoveredButton")->second.GetTextureWidth() / 2 * sizeMultiplicator / 2 - ImGui::CalcTextSize(label).x / 2, cursorPosition.y + (float)m_textureMap.find("hoveredButton")->second.GetTextureHeight() / 2 * sizeMultiplicator - ImGui::CalcTextSize(label).y / 2 - 2.f));
			ImGui::Text(label);
		}
		break;
	default:
		break;
	}
}

void Minecraft::UI::ApplyButtonLogic(ButtonLogic buttonLogic)
{
	switch (buttonLogic)
	{
	case Minecraft::ButtonLogic::SINGLE_PLAYER:
		m_showMainMenuWindows = false;
		m_showSelectWorldMenuWindows = true;
		break;
	case Minecraft::ButtonLogic::OPTIONS:
		m_switchToStateSettings = true;
		break;
	case Minecraft::ButtonLogic::QUIT_GAME:
		m_switchToStateQuit = true;
		break;
	case Minecraft::ButtonLogic::DONE_SETTINGS:
		if (m_previousState == PreviousState::MENU)
		{
			m_switchToStateMenu = true;
		}
		else if (m_previousState == PreviousState::PAUSE)
		{
			m_switchToStatePlay = true;
		}

		break;
	case Minecraft::ButtonLogic::GUI_SCALE_1:
		m_GUIScaleRatioIndex++;
		m_scaleUIRatio = 2.f;
		break;
	case Minecraft::ButtonLogic::GUI_SCALE_2:
		m_GUIScaleRatioIndex++;
		m_scaleUIRatio = 3.f;
		break;
	case Minecraft::ButtonLogic::GUI_SCALE_3:
		m_GUIScaleRatioIndex = 1;
		m_scaleUIRatio = 1.f;
		break;
	case Minecraft::ButtonLogic::CREATE_NEW_WORLD:
		m_switchToStatePlay = true;
		break;
	case Minecraft::ButtonLogic::RETURN_TO_MENU:
		CloseDebugWindows();
		CloseGUIInGameWindows();
		m_switchToStateMenu = true;
		break;
	case Minecraft::ButtonLogic::LOAD_WORLD:
		//Load functions here or turn a bool to true to detect in minecraft.cpp and then use the load code in minecraft.cpp
		m_switchToStatePlay = true;
		m_loadWorld = true;
		break;
	default:
		break;
	}
}

void Minecraft::UI::ApplyDragAndDropLogic(Item* source, Item* target, int indexSource, int indexTarget, Minecraft::World& world, bool isResultCrafting)
{
	// Number of items of what we want to drag & drop
	int payloadCountItem = source[indexSource].GetCountItem();
	//Number of items receiving the items
	int targetCountItem = target[indexTarget].GetCountItem();

	ImGuiIO& io = ImGui::GetIO();

	//Moving a stack with left click released and if LEFT_SHIFT isn't pressed
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && io.KeysDown[GLFW_KEY_LEFT_SHIFT] == false)
	{
		//Check if we move an item into another same item
		if (target[indexTarget].GetItemNameEnum() == source[indexSource].GetItemNameEnum())
		{
			//Check if stack isn't full
			if (targetCountItem != world.GetPlayer().GetInventory().GetMaxItemPerStack())
			{
				//Check if the addition will be under 100, if so just add the source items counts and delete the payload item
				if (targetCountItem + payloadCountItem <= world.GetPlayer().GetInventory().GetMaxItemPerStack())
				{
					if (isResultCrafting == true)
					{
						m_isResultCrafting = false;
						for (int i = 0; i < 4; i++)
						{
							if (world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetItemNameEnum() != ItemNameAvailable::NONE)
							{
								world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetCountItem() -= 1;
							}
							if (world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetCountItem() == 0)
							{
								world.GetPlayer().GetInventory().GetItemCraftArray()[i].SwitchToNoneItem();
							}
						}
					}
					target[indexTarget].SetCountItem(targetCountItem + payloadCountItem);
					source[indexSource].SwitchToNoneItem();
				}
				//Else just complete the stack
				else
				{
					int countItemToCompleteStack = world.GetPlayer().GetInventory().GetMaxItemPerStack() - targetCountItem;
					target[indexTarget].SetCountItem(targetCountItem + countItemToCompleteStack);
					source[indexSource].SetCountItem(payloadCountItem - countItemToCompleteStack);
				}
			}
		}
		//Check if we move an item into a different one, if so we just swap the items
		else if (target[indexTarget].GetItemNameEnum() != source[indexSource].GetItemNameEnum() && source[indexSource].GetItemNameEnum() != ItemNameAvailable::NONE && isResultCrafting == false)
		{
			Item tmp = target[indexTarget];
			target[indexTarget] = source[indexSource];
			source[indexSource] = tmp;
		}
		/*Check if we move the crafting result item into an empty slot (Because we can't swap items formn inventory to the crafting result slot), if so we just remove one to each items count
		in the crafting array, then just move the crafting result into the targeted array and switch the crafting result to "None" item.
		*/
		else if (target[indexTarget].GetItemNameEnum() == ItemNameAvailable::NONE && source[indexSource].GetItemNameEnum() != ItemNameAvailable::NONE && isResultCrafting == true)
		{
			if (isResultCrafting == true)
			{
				m_isResultCrafting = false;
				for (int i = 0; i < 4; i++)
				{
					if (world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetItemNameEnum() != ItemNameAvailable::NONE)
					{
						world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetCountItem() -= 1;
					}
					if (world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetCountItem() == 0)
					{
						world.GetPlayer().GetInventory().GetItemCraftArray()[i].SwitchToNoneItem();
					}
				}
			}
			target[indexTarget] = source[indexSource];
			source[indexSource].SwitchToNoneItem();
		}
	}
	//Moving a half of a stack with right click released and if the source isn't the crafting result
	else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && isResultCrafting == false)
	{
		//Check if we move an item into another same item && if he has more than 1 item Count
		if (target[indexTarget].GetItemNameEnum() == source[indexSource].GetItemNameEnum() && payloadCountItem >= 2)
		{
			//Check if stack isn't full
			if (targetCountItem != world.GetPlayer().GetInventory().GetMaxItemPerStack())
			{
				//Check if the addition will be under 100, if so just add the payload item divided by 2 to the source items counts and divide the payload item by 2
				if (targetCountItem + (payloadCountItem / 2) <= world.GetPlayer().GetInventory().GetMaxItemPerStack())
				{
					target[indexTarget].SetCountItem(targetCountItem + (payloadCountItem / 2));
					source[indexSource].SetCountItem(payloadCountItem - payloadCountItem / 2);
				}
				//Else just complete the stack
				else
				{
					int countItemToCompleteStack = world.GetPlayer().GetInventory().GetMaxItemPerStack() - targetCountItem;
					target[indexTarget].SetCountItem(targetCountItem + countItemToCompleteStack);
					source[indexSource].SetCountItem(payloadCountItem - countItemToCompleteStack);
				}
			}
		}
		//Check if we move an item into a none one, if so we just create the source item into the targeted one and add half of the source item count. then remove the same amount in the source item
		else if (target[indexTarget].GetItemNameEnum() == ItemNameAvailable::NONE && payloadCountItem >= 2)
		{
			int toAdd = payloadCountItem / 2;
			target[indexTarget] = Item(source[indexSource].GetItemNameEnum());
			target[indexTarget].SetCountItem(toAdd);
			source[indexSource].SetCountItem(payloadCountItem - (payloadCountItem / 2));
		}
	}
	//Check if we moving the crafting result by pressing LEFT_SHIFT and mouse left click button released
	else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && io.KeysDown[GLFW_KEY_LEFT_SHIFT] == true && isResultCrafting == true)
	{
		if (source[indexSource].GetToolType() == Tools::NONE)
		{
			//Check if we are moving the crafting result into an empty slot
			if (target[indexTarget].GetItemNameEnum() == ItemNameAvailable::NONE)
			{
				//If so we get the number of craft possible according to the items count in the crafting array
				int min = 100;
				int maxCraft = 100 / source[indexSource].GetCountItem();

				for (int i = 0; i < 4; i++)
				{
					if (world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetCountItem() < min && world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetCountItem() != 0)
					{
						min = world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetCountItem();
					}
				}

				if (source[indexSource].GetCountItem() * min < 100)
				{
					//Then we remove the minimum amount to every items counts in the crafting array
					m_isResultCrafting = false;
					for (int j = 0; j < 4; j++)
					{
						if (world.GetPlayer().GetInventory().GetItemCraftArray()[j].GetItemNameEnum() != ItemNameAvailable::NONE)
						{
							world.GetPlayer().GetInventory().GetItemCraftArray()[j].GetCountItem() -= min;
						}
						if (world.GetPlayer().GetInventory().GetItemCraftArray()[j].GetCountItem() <= 0)
						{
							world.GetPlayer().GetInventory().GetItemCraftArray()[j].SwitchToNoneItem();
						}
					}
					//Get the number of items count of the item we crafted and times it by the minimum, then just move the item into the targeted array
					source[indexSource].SetCountItem(source[indexSource].GetCountItem() * min);
					target[indexTarget] = source[indexSource];
					source[indexSource].SwitchToNoneItem();
				}
				else
				{
					m_isResultCrafting = false;
					for (int j = 0; j < 4; j++)
					{
						if (world.GetPlayer().GetInventory().GetItemCraftArray()[j].GetItemNameEnum() != ItemNameAvailable::NONE)
						{
							world.GetPlayer().GetInventory().GetItemCraftArray()[j].GetCountItem() -= maxCraft;
						}
						if (world.GetPlayer().GetInventory().GetItemCraftArray()[j].GetCountItem() <= 0)
						{
							world.GetPlayer().GetInventory().GetItemCraftArray()[j].SwitchToNoneItem();
						}
					}
					//Get the number of items count of the item we crafted and times it by the minimum, then just move the item into the targeted array
					source[indexSource].SetCountItem(source[indexSource].GetCountItem() * maxCraft);
					target[indexTarget] = source[indexSource];
					source[indexSource].SwitchToNoneItem();
				}
			}
			////Check if we are moving the crafting result into the same item
			else if (target[indexTarget].GetItemNameEnum() == source[indexSource].GetItemNameEnum())

				//Check if stack isn't full
				if (targetCountItem != world.GetPlayer().GetInventory().GetMaxItemPerStack())
				{
					//Check if the addition will be under 100
					if (targetCountItem + payloadCountItem <= world.GetPlayer().GetInventory().GetMaxItemPerStack())
					{
						//We get the number of craft possible according to the items count in the crafting array
						int min = 100;

						for (int i = 0; i < 4; i++)
						{
							if (world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetCountItem() < min && world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetCountItem() != 0)
							{
								min = world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetCountItem();
							}
						}
						m_isResultCrafting = false;
						//Then we remove the minimum amount to every items counts in the crafting array

						for (int j = 0; j < 4; j++)
						{
							if (world.GetPlayer().GetInventory().GetItemCraftArray()[j].GetItemNameEnum() != ItemNameAvailable::NONE)
							{
								world.GetPlayer().GetInventory().GetItemCraftArray()[j].GetCountItem() -= min;
							}
							if (world.GetPlayer().GetInventory().GetItemCraftArray()[j].GetCountItem() == 0)
							{
								world.GetPlayer().GetInventory().GetItemCraftArray()[j].SwitchToNoneItem();
							}
						}
						//Get the number of items count of the item we crafted and times it by the minimum, then just add this count into the targeted array

						target[indexTarget].SetCountItem(targetCountItem + payloadCountItem * min);
						source[indexSource].SwitchToNoneItem();
					}
					//Pop Up Abort cause stack would be over the max
					else
					{
					}
				}
		}
	}
}

void Minecraft::UI::CreateMenuItemsBig(World& world)
{
	ImGui::PushFont(m_minecraftFontBig);
	Texture gameTitle = m_textureManager.GetTexture("assets/UI/Minecraft_title.png");

	SetButtonPos(PositionXButtonImGui::CENTER, PositionYButtonImGui::THIRD_LINE, m_glfwWindowWidth, m_glfwWindowHeight, gameTitle.GetTextureWidth() * 2, gameTitle.GetTextureHeight() * 2);
	ImGui::Image((void*)(intptr_t)gameTitle.GetTextureID(), ImVec2((float)gameTitle.GetTextureWidth() * 2, (float)gameTitle.GetTextureHeight() * 2));

	CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::FIFTH_LINE, ButtonSize::BIG, GUISize::BIG, "Single Player", ButtonLogic::SINGLE_PLAYER, world);

	CreateMinecraftButton(PositionXButtonImGui::CENTER_LEFT, PositionYButtonImGui::SEVENTH_LINE, ButtonSize::SMALL, GUISize::BIG, "Options", ButtonLogic::OPTIONS, world);

	CreateMinecraftButton(PositionXButtonImGui::CENTER_RIGHT, PositionYButtonImGui::SEVENTH_LINE, ButtonSize::SMALL, GUISize::BIG, "Quit Game", ButtonLogic::QUIT_GAME, world);
	ImGui::PopFont();
}

void Minecraft::UI::CreateMenuItemsMedium(World& world)
{
	ImGui::PushFont(m_minecraftFontMedium);

	Texture gameTitle = m_textureManager.GetTexture("assets/UI/Minecraft_title.png");

	SetButtonPos(PositionXButtonImGui::CENTER, PositionYButtonImGui::SECOND_LINE, m_glfwWindowWidth, m_glfwWindowHeight, (int)(gameTitle.GetTextureWidth() * 1.5f), (int)(gameTitle.GetTextureHeight() * 1.5f));

	ImGui::Image((void*)(intptr_t)gameTitle.GetTextureID(), ImVec2((float)gameTitle.GetTextureWidth() * 1.5f, (float)gameTitle.GetTextureHeight() * 1.5f));

	CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::FOURTH_LINE, ButtonSize::BIG, GUISize::MEDIUM, "Single Player", ButtonLogic::SINGLE_PLAYER, world);

	CreateMinecraftButton(PositionXButtonImGui::CENTER_LEFT, PositionYButtonImGui::SIXTH_LINE, ButtonSize::SMALL, GUISize::MEDIUM, "Options", ButtonLogic::OPTIONS, world);

	CreateMinecraftButton(PositionXButtonImGui::CENTER_RIGHT, PositionYButtonImGui::SIXTH_LINE, ButtonSize::SMALL, GUISize::MEDIUM, "Quit Game", ButtonLogic::QUIT_GAME, world);

	ImGui::PopFont();
}

void Minecraft::UI::CreateMenuItemsSmall(World& world)
{
	ImGui::PushFont(m_minecraftFontSmall);

	Texture gameTitle = m_textureManager.GetTexture("assets/UI/Minecraft_title.png");

	SetButtonPos(PositionXButtonImGui::CENTER, PositionYButtonImGui::SECOND_LINE, m_glfwWindowWidth, m_glfwWindowHeight, gameTitle.GetTextureWidth(), gameTitle.GetTextureHeight());
	ImGui::Image((void*)(intptr_t)gameTitle.GetTextureID(), ImVec2((float)gameTitle.GetTextureWidth(), (float)gameTitle.GetTextureHeight()));

	CreateMinecraftButton(PositionXButtonImGui::CENTER, PositionYButtonImGui::FOURTH_LINE, ButtonSize::BIG, GUISize::SMALL, "Single Player", ButtonLogic::SINGLE_PLAYER, world);

	CreateMinecraftButton(PositionXButtonImGui::CENTER_LEFT, PositionYButtonImGui::FIFTH_LINE, ButtonSize::SMALL, GUISize::SMALL, "Options", ButtonLogic::OPTIONS, world);

	CreateMinecraftButton(PositionXButtonImGui::CENTER_RIGHT, PositionYButtonImGui::FIFTH_LINE, ButtonSize::SMALL, GUISize::SMALL, "Quit Game", ButtonLogic::QUIT_GAME, world);

	ImGui::PopFont();
}

void Minecraft::UI::CreateDragAndDropInventoryLogic(int n, Minecraft::World& world)
{
	if (world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetItemNameEnum() != ItemNameAvailable::NONE)
	{
		// Our items are both drag sources and drag targets here!
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			m_currentDragFrom = CurrentDragFrom::INVENTORY;

			ImGui::SetDragDropPayload("Drag And Drop Inventory", &n, sizeof(Item));    // Set payload to carry the index of our item (could be anything)
			ImGui::EndDragDropSource();
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Drag And Drop Inventory"))
		{
			IM_ASSERT(payload->DataSize == sizeof(Item));
			int payload_n = *(const int*)payload->Data;

			switch (m_currentDragFrom)
			{
			case Minecraft::CurrentDragFrom::INVENTORY:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemInventoryArray(), world.GetPlayer().GetInventory().GetItemInventoryArray(), payload_n, n, world, false);
				break;
			case Minecraft::CurrentDragFrom::CRAFTING:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemCraftArray(), world.GetPlayer().GetInventory().GetItemInventoryArray(), payload_n, n, world, false);
				break;
			case Minecraft::CurrentDragFrom::CRAFTING_RESULT:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemCraftResultArray(), world.GetPlayer().GetInventory().GetItemInventoryArray(), payload_n, n, world, true);
				break;
			case Minecraft::CurrentDragFrom::INVENTORY_BAR:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemInventoryBarArray(), world.GetPlayer().GetInventory().GetItemInventoryArray(), payload_n, n, world, true);
				break;
			case Minecraft::CurrentDragFrom::CHEST:
				break;
			default:
				break;
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void Minecraft::UI::CreateDragAndDropInventoryBarLogic(int n, World& world)
{
	if (world.GetPlayer().GetInventory().GetItemInventoryBarArray()[n].GetItemNameEnum() != ItemNameAvailable::NONE)
	{
		// Our items are both drag sources and drag targets here!
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			m_currentDragFrom = CurrentDragFrom::INVENTORY_BAR;

			ImGui::SetDragDropPayload("Drag And Drop Inventory", &n, sizeof(Item));    // Set payload to carry the index of our item (could be anything)
			ImGui::EndDragDropSource();
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Drag And Drop Inventory"))
		{
			IM_ASSERT(payload->DataSize == sizeof(Item));
			int payload_n = *(const int*)payload->Data;

			switch (m_currentDragFrom)
			{
			case Minecraft::CurrentDragFrom::INVENTORY:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemInventoryArray(), world.GetPlayer().GetInventory().GetItemInventoryBarArray(), payload_n, n, world, false);
				break;
			case Minecraft::CurrentDragFrom::CRAFTING:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemCraftArray(), world.GetPlayer().GetInventory().GetItemInventoryBarArray(), payload_n, n, world, false);
				break;
			case Minecraft::CurrentDragFrom::CRAFTING_RESULT:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemCraftResultArray(), world.GetPlayer().GetInventory().GetItemInventoryBarArray(), payload_n, n, world, true);
				break;
			case Minecraft::CurrentDragFrom::INVENTORY_BAR:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemInventoryBarArray(), world.GetPlayer().GetInventory().GetItemInventoryBarArray(), payload_n, n, world, false);
				break;
			case Minecraft::CurrentDragFrom::CHEST:
				break;
			default:
				break;
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void Minecraft::UI::CreateDragAndDropCraftingLogic(int n, World& world)
{
	if (world.GetPlayer().GetInventory().GetItemCraftArray()[n].GetItemNameEnum() != ItemNameAvailable::NONE)
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::SetDragDropPayload("Drag And Drop Inventory", &n, sizeof(Item));    // Set payload to carry the index of our item (could be anything)
			m_currentDragFrom = CurrentDragFrom::CRAFTING;
			ImGui::EndDragDropSource();
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Drag And Drop Inventory"))
		{
			IM_ASSERT(payload->DataSize == sizeof(Item));
			int payload_n = *(const int*)payload->Data;

			switch (m_currentDragFrom)
			{
			case Minecraft::CurrentDragFrom::INVENTORY:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemInventoryArray(), world.GetPlayer().GetInventory().GetItemCraftArray(), payload_n, n, world, false);
				break;
			case Minecraft::CurrentDragFrom::CRAFTING:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemCraftArray(), world.GetPlayer().GetInventory().GetItemCraftArray(), payload_n, n, world, false);
				break;
			case Minecraft::CurrentDragFrom::CRAFTING_RESULT:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemCraftResultArray(), world.GetPlayer().GetInventory().GetItemCraftArray(), payload_n, n, world, true);
				break;
			case Minecraft::CurrentDragFrom::INVENTORY_BAR:
				ApplyDragAndDropLogic(world.GetPlayer().GetInventory().GetItemInventoryBarArray(), world.GetPlayer().GetInventory().GetItemCraftArray(), payload_n, n, world, true);
				break;
			case Minecraft::CurrentDragFrom::CHEST:
				break;
			default:
				break;
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void Minecraft::UI::CreateDragAndDropCraftingResultLogic(int n, World& world)
{
	if (world.GetPlayer().GetInventory().GetItemCraftResultArray()[0].GetItemNameEnum() != ItemNameAvailable::NONE)
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::SetDragDropPayload("Drag And Drop Inventory", &n, sizeof(Item));    // Set payload to carry the index of our item (could be anything)
			m_currentDragFrom = CurrentDragFrom::CRAFTING_RESULT;
			ImGui::EndDragDropSource();
		}
	}
}

void Minecraft::UI::CreateCountItemsBig(Item* slots, ImVec2 cursor, int n, World& /*world*/)
{
	if (slots[n].GetItemNameEnum() != ItemNameAvailable::NONE && slots[n].GetToolType() == Tools::NONE)
	{
		if (slots[n].GetCountItem() < 10)
		{
			ImGui::SetCursorPos(ImVec2(cursor.x + 25.f, cursor.y + 30.f));

			ImGui::Text("%d", slots[n].GetCountItem());
		}
		else if (slots[n].GetCountItem() < 100 && slots[n].GetCountItem() >= 10)
		{
			ImGui::SetCursorPos(ImVec2(cursor.x + 12.5f, cursor.y + 30.f));
			ImGui::Text("%d", slots[n].GetCountItem());
		}
		else
		{
			ImGui::SetCursorPos(ImVec2(cursor.x, cursor.y + 30.f));
			ImGui::Text("%d", slots[n].GetCountItem());
		}
	}
}

void Minecraft::UI::CreateCountItemsMedium(Item* slots, ImVec2 cursor, int n, World& /*world*/)
{
	if (slots[n].GetItemNameEnum() != ItemNameAvailable::NONE && slots[n].GetToolType() == Tools::NONE)
	{
		if (slots[n].GetCountItem() < 10)
		{
			ImGui::SetCursorPos(ImVec2(cursor.x + 15.f, cursor.y + 15.f));

			ImGui::Text("%d", slots[n].GetCountItem());
		}
		else if (slots[n].GetCountItem() < 100 && slots[n].GetCountItem() >= 10)
		{
			ImGui::SetCursorPos(ImVec2(cursor.x + 7.5f, cursor.y + 15.f));
			ImGui::Text("%d", slots[n].GetCountItem());
		}
		else
		{
			ImGui::SetCursorPos(ImVec2(cursor.x, cursor.y + 15.f));
			ImGui::Text("%d", slots[n].GetCountItem());
		}
	}
}

void Minecraft::UI::CreateCountItemsSmall(Item* slots, ImVec2 cursor, int n, World& /*world*/)
{
	if (slots[n].GetItemNameEnum() != ItemNameAvailable::NONE && slots[n].GetToolType() == Tools::NONE)
	{
		if (slots[n].GetCountItem() < 10)
		{
			ImGui::SetCursorPos(ImVec2(cursor.x + 7.f, cursor.y + 10.f));

			ImGui::Text("%d", slots[n].GetCountItem());
		}
		else if (slots[n].GetCountItem() < 100 && slots[n].GetCountItem() >= 10)
		{
			ImGui::SetCursorPos(ImVec2(cursor.x + 3.5f, cursor.y + 10.f));
			ImGui::Text("%d", slots[n].GetCountItem());
		}
		else
		{
			ImGui::SetCursorPos(ImVec2(cursor.x, cursor.y + 10.f));
			ImGui::Text("%d", slots[n].GetCountItem());
		}
	}
}

void Minecraft::UI::CreateItemsAndCountAndDDLogic(int n, World& world, Item* slots, Slots slotsLogic, float paddingX, float paddingY)
{
	switch (slotsLogic)
	{
	case Minecraft::Slots::INVENTORY:
		if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX, (float)m_glfwWindowHeight / 2 + 8.f + paddingY));
		}
		else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX, (float)m_glfwWindowHeight / 2 + 6.f + paddingY));
		}
		else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX, (float)m_glfwWindowHeight / 2 + 4.f + paddingY));
		}
		break;
	case Minecraft::Slots::INVENTORY_BAR:
		ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX, (float)m_glfwWindowHeight / 2 + 8.f + paddingY));
		break;
	case Minecraft::Slots::CRAFTING:
		if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
		{
			ImGui::SetCursorPos(ImVec2(m_glfwWindowWidth / 2 + paddingX - 37.f, 158.5f + paddingY));
		}
		else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
		{
			ImGui::SetCursorPos(ImVec2(m_glfwWindowWidth / 2 + paddingX - 27.f, 262.5f + paddingY));
		}
		else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
		{
			ImGui::SetCursorPos(ImVec2(m_glfwWindowWidth / 2 + paddingX - 14.f, 352.5f + paddingY));
		}
		break;
	case Minecraft::Slots::CRAFTING_RESULT:
		if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
		{
			ImGui::SetCursorPos(ImVec2(m_glfwWindowWidth / 2 + 333.f, 208.f));
		}
		else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
		{
			ImGui::SetCursorPos(ImVec2(m_glfwWindowWidth / 2 + 226.f, 295.f));
		}
		else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
		{
			ImGui::SetCursorPos(ImVec2(m_glfwWindowWidth / 2 + 133.5f, 373.f));
		}
		break;
	default:
		break;
	}
	ImVec2 cursor = ImGui::GetCursorPos();

	if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
	{
		ImGui::Image((void*)(intptr_t)slots[n].GetTexture().GetTextureID(), ImVec2(73.f, 73.f));
	}
	else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
	{
		ImGui::Image((void*)(intptr_t)slots[n].GetTexture().GetTextureID(), ImVec2(50.f, 50.f));
	}
	else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
	{
		ImGui::Image((void*)(intptr_t)slots[n].GetTexture().GetTextureID(), ImVec2(28.f, 28.f));
	}

	if (ImGui::IsItemHovered())
	{
		if (slots[n].GetItemNameEnum() != ItemNameAvailable::NONE)
		{
			ImGui::SetTooltip(slots[n].GetItemName().c_str());
		}
	}
	switch (slotsLogic)
	{
	case Minecraft::Slots::INVENTORY:
		CreateDragAndDropInventoryLogic(n, world);
		break;
	case Minecraft::Slots::INVENTORY_BAR:
		CreateDragAndDropInventoryBarLogic(n, world);
		break;
	case Minecraft::Slots::CRAFTING:
		CreateDragAndDropCraftingLogic(n, world);
		break;
	case Minecraft::Slots::CRAFTING_RESULT:
		CreateDragAndDropCraftingResultLogic(n, world);
		break;
	default:
		break;
	}

	if (m_GUIScaleRatioIndex == 3 && m_scaleUIRatio == 3.f)
	{
		ImGui::PushFont(m_minecraftFontBig);
		CreateCountItemsBig(slots, cursor, n, world);
		ImGui::PopFont();
	}
	else if (m_GUIScaleRatioIndex == 2 && m_scaleUIRatio == 2.f)
	{
		ImGui::PushFont(m_minecraftFontMedium);
		CreateCountItemsMedium(slots, cursor, n, world);
		ImGui::PopFont();
	}
	else if (m_GUIScaleRatioIndex == 1 && m_scaleUIRatio == 1.f)
	{
		ImGui::PushFont(m_minecraftFontSmall);
		CreateCountItemsSmall(slots, cursor, n, world);
		ImGui::PopFont();
	}
}

void Minecraft::UI::CreateInventoryTexturesMedium(float paddingX, float paddingY, int n, Minecraft::World& world)
{
	ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX, (float)m_glfwWindowHeight / 2 + 6.f + paddingY));
	ImGui::Image((void*)(intptr_t)world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetTexture().GetTextureID(), ImVec2(50.f, 50.f));
}

void Minecraft::UI::CreateInventoryCountItemsMedium(float paddingX, float paddingY, int n, Minecraft::World& world)
{
	if (world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetItemNameEnum() != ItemNameAvailable::NONE)
	{
		if (world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem() < 10)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX + 16.f, (float)m_glfwWindowHeight / 2 + 25.f + paddingY));

			ImGui::Text("%d", world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem());
		}
		else if (world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem() < 100 && world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem() >= 10)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX + 7.f, (float)m_glfwWindowHeight / 2 + 25.f + paddingY));
			ImGui::Text("%d", world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem());
		}
		else
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX, (float)m_glfwWindowHeight / 2 + 25.f + paddingY));
			ImGui::Text("%d", world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem());
		}
	}
}

void Minecraft::UI::CreateInventoryTexturesSmall(float paddingX, float paddingY, int n, Minecraft::World& world)
{
	ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX, (float)m_glfwWindowHeight / 2 + 4.f + paddingY));
	ImGui::Image((void*)(intptr_t)world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetTexture().GetTextureID(), ImVec2(28.f, 28.f));
}

void Minecraft::UI::CreateInventoryCountItemsSmall(float paddingX, float paddingY, int n, Minecraft::World& world)
{
	if (world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetItemNameEnum() != ItemNameAvailable::NONE)
	{
		if (world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem() < 10)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX + 9.f, (float)m_glfwWindowHeight / 2 + 15.f + paddingY));

			ImGui::Text("%d", world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem());
		}
		else if (world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem() < 100 && world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem() >= 10)
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX + 4.f, (float)m_glfwWindowHeight / 2 + 15.f + paddingY));
			ImGui::Text("%d", world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem());
		}
		else
		{
			ImGui::SetCursorPos(ImVec2((float)m_glfwWindowWidth / 2 - m_textureManager.GetTexture("assets/UI/Inventory.png").GetTextureWidth() + paddingX, (float)m_glfwWindowHeight / 2 + 15.f + paddingY));
			ImGui::Text("%d", world.GetPlayer().GetInventory().GetItemInventoryArray()[n].GetCountItem());
		}
	}
}

void Minecraft::UI::SetPlayerPos(Minecraft::vec3 position, Minecraft::vec2 rotation)
{
	m_playerPos = position;
	m_playerRot = rotation;
}

void Minecraft::UI::SetChunkPos(glm::ivec2 position)
{
	m_chunkPos = position;
}

bool Minecraft::UI::IsResultCrafting()
{
	return m_isResultCrafting;
}

void Minecraft::UI::CheckForInventoryCrafting(Minecraft::World& world)
{
	//Logic works but need a redo to be more generic

	for (std::vector<Recipe>::iterator it = world.GetPlayer().GetInventory().GetRecipeList().begin(); it != world.GetPlayer().GetInventory().GetRecipeList().end(); it++)
	{
		if (it->GetWidthRecipe() == 1 && it->GetHeightRecipe() == 1)
		{
			for (int i = 0; i < 4; i++)
			{
				if (it->GetRecipeArray()[0].GetItemNameEnum() == world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetItemNameEnum() && world.GetPlayer().GetInventory().CountNoneItemInCraftingInventory() == 3)
				{
					m_isResultCrafting = true;
					world.GetPlayer().GetInventory().GetItemCraftResultArray()[0] = it->GetResultOfRecipe();
					break;
				}
				else
				{
					m_isResultCrafting = false;
					world.GetPlayer().GetInventory().GetItemCraftResultArray()[0].SwitchToNoneItem();
					continue;
				}
			}
		}
		else if (it->GetWidthRecipe() == 1 && it->GetHeightRecipe() == 2)
		{
			for (int i = 0; i < 4; i++)
			{
				if (i == 0 || i == 1)
				{
					if (it->GetRecipeArray()[0].GetItemNameEnum() == world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetItemNameEnum())
					{
						i += 2;
						if (it->GetRecipeArray()[1].GetItemNameEnum() == world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetItemNameEnum() && world.GetPlayer().GetInventory().CountNoneItemInCraftingInventory() == 2)
						{
							m_isResultCrafting = true;
							world.GetPlayer().GetInventory().GetItemCraftResultArray()[0] = it->GetResultOfRecipe();
						}
					}
				}
			}
		}
		else if (it->GetWidthRecipe() == 2 && it->GetHeightRecipe() == 1)
		{
			for (int i = 0; i < 4; i++)
			{
				if (i == 0 || i == 2)
				{
					if (it->GetRecipeArray()[0].GetItemNameEnum() == world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetItemNameEnum())
					{
						i++;
						if (it->GetRecipeArray()[1].GetItemNameEnum() == world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetItemNameEnum() && world.GetPlayer().GetInventory().CountNoneItemInCraftingInventory() == 2)
						{
							m_isResultCrafting = true;
							world.GetPlayer().GetInventory().GetItemCraftResultArray()[0] = it->GetResultOfRecipe();
						}
					}
				}
			}
		}
		else if (it->GetWidthRecipe() == 2 && it->GetHeightRecipe() == 2)
		{
			for (int i = 0; i < 4; i++)
			{
				if (it->GetRecipeArray()[i].GetItemNameEnum() == world.GetPlayer().GetInventory().GetItemCraftArray()[i].GetItemNameEnum())
				{
					if (i == 3)
					{
						m_isResultCrafting = true;
						world.GetPlayer().GetInventory().GetItemCraftResultArray()[0] = it->GetResultOfRecipe();
						break;
					}
					continue;
				}
				else
				{
					m_isResultCrafting = false;
					world.GetPlayer().GetInventory().GetItemCraftResultArray()[0].SwitchToNoneItem();
					break;
				}
			}
		}

		if (m_isResultCrafting == true)
		{
			break;
		}
	}
}

void Minecraft::UI::SetPreviousState(PreviousState state)
{
	m_previousState = state;
}

Minecraft::PreviousState Minecraft::UI::GetPreviousState()
{
	return m_previousState;
}

void Minecraft::UI::GenericGraph(float* data, int dataSize, ImVec2 graphSize)
{
	float maxVal = data[0];
	float minVal = data[0];

	for (int i = 0; i < dataSize; i++)
	{
		if (maxVal < data[i])
			maxVal = data[i];
		if (minVal > data[i])
			minVal = data[i];
	}

	ImGui::PlotLines("", data, dataSize, 0, NULL, minVal, maxVal, graphSize);
}

bool& Minecraft::UI::IsLoadingWorld()
{
	return m_loadWorld;
}