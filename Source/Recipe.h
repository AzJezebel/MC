#pragma once
#include "Item.h"
namespace Minecraft
{
	
	class Recipe
	{
	public:
		Recipe(Item item1, Item item2, Item item3, Item item4, Item item5, Item item6, Item item7, Item item8, Item item9, Item result, int widthRecipe, int heightRecipe);
		~Recipe() = default;

		Item* GetRecipeArray();

		Item GetResultOfRecipe();

		int GetWidthRecipe();
		int GetHeightRecipe();

	private:

		//Recipe of 9 items or less
		Item m_bigRecipe[9];

		Item m_resultOfRecipe;

		int m_widthRecipe;
		int m_heightRecipe;


	};
}