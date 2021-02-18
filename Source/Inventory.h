#pragma once

#include "define.h"
#include "Item.h"
#include "Recipe.h"
#include <vector>

namespace Minecraft
{
	class Inventory
	{
	public:
		Inventory();
		~Inventory() = default;

		Item* GetItemInventoryArray();
		Item* GetItemInventoryBarArray();
		Item const* GetItemInventoryBarArray() const { return m_itemInventoryBarArray; }
		Item* GetItemCraftArray();
		Item* GetItemCraftResultArray();
		int& GetMaxItemPerStack();

		std::vector<Recipe>& GetRecipeList();

		void CreateRecipes();

		int CountNoneItemInCraftingInventory();

		int CheckForEmptySlot(Item* slots, int size);
		int CheckForSameItemSlot(Item* slots, int blockID, int size);

		void AddMinedItemInInventory(int blockID);


	private:

		std::vector<Recipe> m_recipeList;

		Item m_itemInventoryArray[27];

		Item m_itemInventoryBarArray[9];

		Item m_itemCraftArray[ITEM_NUMBER_IN_CRAFT_INVENTORY];

		Item m_resultCrafting[1];

		int m_maxItemPerStack = MAX_ITEM_PER_STACK;



	};
}