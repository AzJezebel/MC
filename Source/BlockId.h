#pragma once

namespace Minecraft
{
	namespace BlockId // using int because GLSL dont have char
	{
		const int NO_BLOCK = -1;

		// Updating this file ? You should update shaders const as well.
		const int AIR = 0;
		const int SAND = 1;
		const int GRAVEL = 2;
		const int DIRT = 3;
		const int GRASS = 4;
		const int PATH = 5;
		const int FARMLAND_DRY = 6;
		const int FARMLAND_MOIST = 7;
		//const int WATER = 8; // use WATER_SOURCE instead
		const int MELON = 9;

		// Updating this file ? You should update shaders const as well.
		const int PUMPKIN = 10;
		const int DWAYNE_THE_BLOCK_JOHNSON = 11;
		const int KEKW = 12;
		const int TORCH = 13;
		const int OAK_LEAVES = 14;
		const int OAK_LOG = 15;
		const int OAK_PLANK = 16;
		const int CRAFTING_TABLE = 17;
		const int CHEST = 18;
		const int BIRCH_PLANK = 19;

		// Updating this file ? You should update shaders const as well.
		const int BIRCH_LOG = 20;
		const int BIRCH_LEAVES = 21;
		const int STONE = 22;
		const int IRON = 23;
		const int COAL = 24;
		const int DIAMOND = 25;
		const int COBBLESTONE = 26;
		const int FURNACE_OFF = 27;
		const int FURNACE_ON = 28;
		const int STONE_BRICK = 29;

		// Updating this file ? You should update shaders const as well.
		//const int LAVA = 30; // use LAVA_SOURCE instead
		const int OBSIDIAN = 31;
		const int BEDROCK = 32;

		// Updating this file ? You should update shaders const as well.
		const int WATER_1 = 128;
		const int WATER_2 = 129;
		const int WATER_3 = 130;
		const int WATER_4 = 131;
		const int WATER_5 = 132;
		const int WATER_6 = 133;
		const int WATER_7 = 134;
		const int WATERFALL = 135;
		const int WATER_SOURCE = 136;
		const int LAVA_1 = 137;
		const int LAVA_2 = 138;
		const int LAVA_3 = 139;
		const int LAVA_WATERFALL = 140;
		const int LAVA_SOURCE = 141;

		// Updating this file ? You should update shaders const as well.

		const int SWORD = 150;
		const int PICKAXE = 151;
		const int AXE = 152;
		const int SHOVEL = 153;
		const int thatHOEoverhere = 154;
		const int SHEAR = 155;
		const int FLINT_N_STEEL = 156;
	}
}