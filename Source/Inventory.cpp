#include "Inventory.h"
#include <iostream>

Minecraft::Inventory::Inventory() :
	m_itemInventoryArray{ Item(ItemNameAvailable::DIRT, 10, true),
							Item(ItemNameAvailable::GRAVEL, 5, true),
							Item(ItemNameAvailable::OAK_LOG, 3, true),
							Item(ItemNameAvailable::DIRT, 2, true),
							Item(ItemNameAvailable::OAK_LOG, 57, true),
							Item(ItemNameAvailable::DIRT, 5, true),
							Item(ItemNameAvailable::DIRT, true),
							Item(ItemNameAvailable::DIRT, 100, true),
							Item(ItemNameAvailable::DIRT, 98, true),
							Item(ItemNameAvailable::GRASS, 100, true),
							Item(ItemNameAvailable::KEKW, 9),
							Item(ItemNameAvailable::SAND, 77, true) },
	m_itemInventoryBarArray {
	Item(ItemNameAvailable::WOODEN_SWORD, 1, false, Tools::SWORD)
	}
{
	CreateRecipes();
}

Minecraft::Item* Minecraft::Inventory::GetItemInventoryArray()
{
	return m_itemInventoryArray;
}

Minecraft::Item* Minecraft::Inventory::GetItemInventoryBarArray()
{
	return m_itemInventoryBarArray;
}

Minecraft::Item* Minecraft::Inventory::GetItemCraftArray()
{
	return m_itemCraftArray;
}

Minecraft::Item* Minecraft::Inventory::GetItemCraftResultArray()
{
	return m_resultCrafting;
}

int& Minecraft::Inventory::GetMaxItemPerStack()
{
	return m_maxItemPerStack;
}

std::vector<Minecraft::Recipe>& Minecraft::Inventory::GetRecipeList()
{
	return m_recipeList;
}

/*Rules to create recipes :
 - For an item that requires only 1 block, so having width and height = 1 :
	Put the block needed for the craft into the first slot of the recipe, then fill the rest with none items
 - For an item that requires 2 block in line (width = 2 and height = 1):
	 In the first slot, put the item which will be at the left of the craft, then the second item, then fill with none items
 - For an item that requires 2 block in column (width = 1 and height = 2):
	 In the first slot, put the item which will be at the top of the craft, then the second item, then fill with none items
 - For an item that requires 4 block in column (width = 2 and height = 2):
	 In the first slot, put the item which will be at the top left of the craft, then the second item which will be at the top right, then the thrid item which will be at the bottom left
	and then the last item which will be at the bottom right of the craft, then fill with none items
	PS : To create a craft with 3 items needed, we just use the same thing that we used to create the 4items craft, but replace one of them with a None item
	Example : Here we craft a chest with 3 planks and an empty slot, we want the empty slot to be at the bottom left, so we create the recipe like this for the 4 first slots :
		Recipe(ITEM::OAK_PLANK, ITEM::OAK_PLANK, ITEM::NONE, ITEM::OAK_PLANK)
*/
void Minecraft::Inventory::CreateRecipes()
{
	//Oak Planks recipe
	m_recipeList.push_back(Recipe(Item(ItemNameAvailable::OAK_LOG), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::OAK_PLANK, 4, true), 1, 1));

	//Dwayne recipe
	m_recipeList.push_back(Recipe(Item(ItemNameAvailable::GRASS), Item(ItemNameAvailable::GRASS), Item(ItemNameAvailable::KEKW),
		Item(ItemNameAvailable::KEKW), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::DWAYNE_THE_BLOCK_JOHNSON), 2, 2));

	//Stick recipe
	m_recipeList.push_back(Recipe(Item(ItemNameAvailable::OAK_PLANK), Item(ItemNameAvailable::OAK_PLANK), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::STICK, 2), 1, 2));

	//Kekw recipe
	m_recipeList.push_back(Recipe(Item(ItemNameAvailable::DIRT), Item(ItemNameAvailable::GRAVEL), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::KEKW), 2, 1));

	//Chest recipe
	m_recipeList.push_back(Recipe(Item(ItemNameAvailable::OAK_PLANK), Item(ItemNameAvailable::OAK_PLANK), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::OAK_PLANK), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::CHEST, true), 2, 2));

	m_recipeList.push_back(Recipe(Item(ItemNameAvailable::OAK_PLANK), Item(ItemNameAvailable::STICK), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE), Item(ItemNameAvailable::NONE),
		Item(ItemNameAvailable::WOODEN_SWORD, 1, false, Tools::SWORD), 1, 2));
}

int Minecraft::Inventory::CountNoneItemInCraftingInventory()
{
	int i = 0;

	for (int j = 0; j < ITEM_NUMBER_IN_CRAFT_INVENTORY; j++)
	{
		if (m_itemCraftArray[j].GetItemNameEnum() == ItemNameAvailable::NONE)
		{
			i++;
		}
	}

	return i;
}

int Minecraft::Inventory::CheckForEmptySlot(Item* slots, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (slots[i].GetItemNameEnum() == ItemNameAvailable::NONE)
		{
			return i;
		}
	}
	return -1;
}

int Minecraft::Inventory::CheckForSameItemSlot(Item* slots, int blockID, int size)
{	
	for (int i = 0; i < size; i++)
	{
		if (slots[i].GetItemNameEnum() == (ItemNameAvailable)blockID && slots[i].GetCountItem() < 100)
		{
			return i;
		}
	}
	return -1;
}

void Minecraft::Inventory::AddMinedItemInInventory(int blockID)
{

	int slotIndex = 0;
	bool m_InInventoryBar = false;
	bool m_InInventory = false;
	bool addToInventory = false;
	bool createInInventory = false;

	slotIndex = CheckForSameItemSlot(GetItemInventoryBarArray(), blockID, 9);

	if (slotIndex == -1)
	{
		slotIndex = CheckForSameItemSlot(GetItemInventoryArray(), blockID, 24);
		m_InInventoryBar = false;
		m_InInventory = true;
		addToInventory = true;
		createInInventory = false;
	}
	else
	{
		m_InInventoryBar = true;
		m_InInventory = false;
		addToInventory = true;
		createInInventory = false;
	}

	if (slotIndex == -1)
	{
		slotIndex = CheckForEmptySlot(GetItemInventoryBarArray(), 9);
		if (slotIndex == -1)
		{
			slotIndex = CheckForEmptySlot(GetItemInventoryArray(), 24);
			m_InInventoryBar = false;
			m_InInventory = true;
			addToInventory = false;
			createInInventory = true;
		}
		else
		{
			m_InInventoryBar = true;
			m_InInventory = false;
			addToInventory = false;
			createInInventory = true;
		}

	}
	

	if (m_InInventoryBar)
	{
		if (addToInventory)
		{
			GetItemInventoryBarArray()[slotIndex].SetCountItem(GetItemInventoryBarArray()[slotIndex].GetCountItem() + 1);
		}
		else if (createInInventory)
		{
			GetItemInventoryBarArray()[slotIndex] = Item((ItemNameAvailable)blockID);
		}
	}
	else
	{
		if (addToInventory)
		{
			GetItemInventoryArray()[slotIndex].SetCountItem(GetItemInventoryArray()[slotIndex].GetCountItem() + 1);
		}
		else if (createInInventory)
		{
			GetItemInventoryArray()[slotIndex] = Item((ItemNameAvailable)blockID);
		}
	}
}
