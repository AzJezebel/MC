#pragma once
#include "Texture.h"
namespace Minecraft
{
	enum class Tools
	{
		NONE,

		SWORD,
		SHOVEL,
		PICKAXE,
		AXE
	};
	//Keep this numbers equals to those in BlockId.h
	enum class ItemNameAvailable
	{

		//All these items below are linked to BlockId.h and are block that we can place on the world
		NONE,
		SAND,
		GRAVEL,
		DIRT,
		GRASS,
		PATH,
		FARMLAND_DRY,
		FARMLAND_MOIST,
		MELON = 9,
		PUMPKIN,
		DWAYNE_THE_BLOCK_JOHNSON,
		KEKW,
		TORCH,
		OAK_LEAVES,
		OAK_LOG,
		OAK_PLANK,
		CRAFTING_TABLE,
		CHEST,
		BIRCH_PLANK,
		BIRCH_LOG,
		BIRCH_LEAVES,
		STONE,
		IRON,
		COAL,
		DIAMOND,
		COBBLESTONE,
		FURNACE_OFF,

		STONE_BRICK = 29,
		OBSIDIAN = 31,

		WATER_SOURCE = 136,
		LAVA_SOURCE = 141,

		//All these items below are just items in game (Used for craft or used as a tool, can't be place on the ground)
		STICK,
		WOODEN_SWORD,
	};


	class Item
	{
	public:
		Item() = default;
		Item(ItemNameAvailable name, int count=1, bool isABlock = true, Tools tools = Tools::NONE);
		Item(const std::string& name, int count);
		~Item() = default;
		Texture const& GetTexture() const { return m_myTexture; }
		Texture& GetTexture();
		std::string& GetItemName();
		ItemNameAvailable& GetItemNameEnum();
		int& GetCountItem();
		void SetCountItem(int i);

		void SwitchToNoneItem();

		bool IsABlock() const;

		Tools& GetToolType();
		Tools const& GetToolType() const { return m_tools; }
	private:
		ItemNameAvailable m_itemNameEnum = ItemNameAvailable::NONE;
		std::string m_itemName = "None";
		Texture m_myTexture = Texture("assets/UI/Png.png");
		int m_count = 0;
		bool m_isABlock = false;
		Tools m_tools;

	};
}