#include "Item.h"

Minecraft::Item::Item(ItemNameAvailable name, int count, bool isABlock, Tools tools)
{
	m_itemNameEnum = name;
	m_count = count;
	m_isABlock = isABlock;
	m_tools = tools;
	switch (m_itemNameEnum)
	{
	case ItemNameAvailable::GRASS:
		m_itemName = "Grass";
		m_myTexture = Texture("assets/UI/TexturesUI/Grass.png");
		break;
	case ItemNameAvailable::STONE:
		m_itemName = "Stone";
		m_myTexture = Texture("assets/UI/TexturesUI/Stone.png");
		break;
	case ItemNameAvailable::OAK_LOG:
		m_itemName = "Oak Log";
		m_myTexture = Texture("assets/UI/TexturesUI/Oak_Log.png");
		break;
	case ItemNameAvailable::OAK_PLANK:
		m_itemName = "Oak Plank";
		m_myTexture = Texture("assets/UI/TexturesUI/Oak_Plank.png");
		break;
	case ItemNameAvailable::CHEST:
		m_itemName = "Chest";
		m_myTexture = Texture("assets/UI/TexturesUI/Chest.png");
		break;
	case ItemNameAvailable::TORCH:
		break;
	case ItemNameAvailable::DIRT:
		m_itemName = "Dirt";
		m_myTexture = Texture("assets/UI/TexturesUI/Dirt.png");
		break;
	case ItemNameAvailable::GRAVEL:
		m_itemName = "Gravel";
		m_myTexture = Texture("assets/UI/TexturesUI/Gravel.png");
		break;
	case ItemNameAvailable::NONE:
		m_itemName = "None";
		m_myTexture = Texture("assets/UI/TexturesUI/Png.png");
		break;
	case ItemNameAvailable::KEKW:
		m_itemName = "KEKW";
		m_myTexture = Texture("assets/UI/TexturesUI/kekw.png");
		break;
	case ItemNameAvailable::DWAYNE_THE_BLOCK_JOHNSON:
		m_itemName = "Dwayne \"The Block\" Johnson";
		m_myTexture = Texture("assets/UI/TexturesUI/Dwayne_TheBlock_Johnson.png");
		break;
	case ItemNameAvailable::STICK:
		m_itemName = "Stick";
		m_myTexture = Texture("assets/UI/TexturesUI/Stick.png");
		break;
	case ItemNameAvailable::WOODEN_SWORD:
		m_itemName = "Wooden Sword";
		m_myTexture = Texture("assets/UI/TexturesUI/Wooden_Sword.png");
		break;
	case ItemNameAvailable::SAND:
		m_itemName = "Sand";
		m_myTexture = Texture("assets/UI/TexturesUI/Sand.png");
		break;
	case ItemNameAvailable::OAK_LEAVES:
		m_itemName = "Oak Leaves";
		m_myTexture = Texture("assets/UI/TexturesUI/Oak_Leaves.png");
		break;
	case ItemNameAvailable::WATER_SOURCE:
		m_itemName = "Water";
		m_myTexture = Texture("assets/UI/TexturesUI/Water.png");
		break;
	default:
		break;
	}
}

Minecraft::Item::Item(const std::string& name, int count)
{
	m_itemName = name;
	m_count = count;
	// C++ switch with strings when
	if (m_itemName == "Grass")
	{
		m_itemNameEnum = ItemNameAvailable::GRASS;
		m_myTexture = Texture("assets/UI/Grass.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}

	if (m_itemName == "Stone")
	{
		m_itemNameEnum = ItemNameAvailable::STONE;
		m_myTexture = Texture("assets/UI/Stone.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}

	if (m_itemName == "Oak Log")
	{
		m_itemNameEnum = ItemNameAvailable::OAK_LOG;
		m_myTexture = Texture("assets/UI/Oak_Log.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}
	if (m_itemName == "Oak Plank")
	{
		m_itemNameEnum = ItemNameAvailable::OAK_PLANK;
		m_myTexture = Texture("assets/UI/Grass.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}
	if (m_itemName == "Chest")
	{
		m_itemNameEnum = ItemNameAvailable::CHEST;
		m_myTexture = Texture("assets/UI/Chest.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}

	if (m_itemName == "Dirt")
	{
		m_itemNameEnum = ItemNameAvailable::DIRT;
		m_myTexture = Texture("assets/UI/Dirt.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}

	if (m_itemName == "Gravel")
	{
		m_itemNameEnum = ItemNameAvailable::GRAVEL;
		m_myTexture = Texture("assets/UI/Gravel.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}

	if (m_itemName == "None")
	{
		m_itemNameEnum = ItemNameAvailable::NONE;
		m_myTexture = Texture("assets/UI/Png.png");
		m_isABlock = false;
		m_tools = Tools::NONE;
		return;
	}

	if (m_itemName == "KEKW")
	{
		m_itemNameEnum = ItemNameAvailable::KEKW;
		m_myTexture = Texture("assets/UI/kekw.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}

	if (m_itemName == "Dwayne \"The Block\" Johnson")
	{
		m_itemNameEnum = ItemNameAvailable::DWAYNE_THE_BLOCK_JOHNSON;
		m_myTexture = Texture("assets/UI/Dwayne_TheBlock_Johnson.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}

	if (m_itemName == "Stick")
	{
		m_itemNameEnum = ItemNameAvailable::STICK;
		m_myTexture = Texture("assets/UI/Stick.png");
		m_isABlock = false;
		m_tools = Tools::NONE;
		return;
	}

	if (m_itemName == "Wooden_Sword")
	{
		m_itemNameEnum = ItemNameAvailable::WOODEN_SWORD;
		m_myTexture = Texture("assets/UI/Wooden_Sword.png");
		m_isABlock = false;
		m_tools = Tools::SWORD;
		return;
	}

	if (m_itemName == "Sand")
	{
		m_itemNameEnum = ItemNameAvailable::SAND;
		m_myTexture = Texture("assets/UI/Sand.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}

	if (m_itemName == "Oak Leaves")
	{
		m_itemNameEnum = ItemNameAvailable::OAK_LEAVES;
		m_myTexture = Texture("assets/UI/Oak_Leaves.png");
		m_isABlock = true;
		m_tools = Tools::NONE;
		return;
	}
}

Minecraft::Texture& Minecraft::Item::GetTexture()
{
	return m_myTexture;
}

std::string& Minecraft::Item::GetItemName()
{
	return m_itemName;
}

Minecraft::ItemNameAvailable& Minecraft::Item::GetItemNameEnum()
{
	return m_itemNameEnum;
}

int& Minecraft::Item::GetCountItem()
{
	return m_count;
}

void Minecraft::Item::SetCountItem(int i)
{
	m_count = i;
}

void Minecraft::Item::SwitchToNoneItem()
{
	m_itemName = "";
	m_itemNameEnum = ItemNameAvailable::NONE;
	m_count = 0;
	m_myTexture = Texture("assets/UI/Png.png");
	m_isABlock = false;
	m_tools = Tools::NONE;
}

bool Minecraft::Item::IsABlock() const
{
	return m_isABlock;
}

Minecraft::Tools& Minecraft::Item::GetToolType()
{
	return m_tools;
}