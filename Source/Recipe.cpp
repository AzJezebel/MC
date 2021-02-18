#include "Recipe.h"

Minecraft::Recipe::Recipe(Item item1, Item item2, Item item3, Item item4, Item item5, Item item6, Item item7, Item item8, Item item9, Item result, int widthRecipe, int heightRecipe):
	m_bigRecipe{item1, item2, item3, item4, item5, item6, item7, item8},
	m_resultOfRecipe(result),
	m_widthRecipe(widthRecipe),
	m_heightRecipe(heightRecipe)
{
}

Minecraft::Item* Minecraft::Recipe::GetRecipeArray()
{
	return m_bigRecipe;
}

Minecraft::Item Minecraft::Recipe::GetResultOfRecipe()
{
	return m_resultOfRecipe;
}

int Minecraft::Recipe::GetWidthRecipe()
{
	return m_widthRecipe;
}

int Minecraft::Recipe::GetHeightRecipe()
{
	return m_heightRecipe;
}
