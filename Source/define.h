# pragma once

#include "Trigo.h"

// World
#define CHUNK_LOAD_RADIUS 8
#define CHUNK_ARRAY_SIZE (CHUNK_LOAD_RADIUS * 2) + 1
#define MAX_ENEMIES 10

// Chunk
#define CHUNK_X_SIZE 16
#define CHUNK_Y_SIZE 256
#define CHUNK_Z_SIZE 16
#define CHUNK_WATER_LEVEL 62
//#define GENERATE_FLAT_CHUNKS

// Noise
#define NOISE_ZOOM_LEVEL 100.0f
#define NOISE_LACUNARITY 2
#define NOISE_PERSISTENCE 2
#define MAX_NOISE_OFFSET 100000

// Creature
#define MAX_LIFE 10
#define BASE_DAMAGE 1
#define JUMP_VEL 8
#define BREATH_TIME 5 

// Player
#define MOUSE_SENSI 0.002f
#define BASE_SPEED 5.f
#define CROUCH_SPEED (BASE_SPEED * .5f)
#define SPRINT_SPEED (BASE_SPEED * 3.f)
#define SWIM_SPEED (BASE_SPEED * 2.5f)
#define FLY_SPEED (BASE_SPEED * 10.f)
#define PLAYER_HEIGHT 1.5f
#define CROUCH_HEIGHT (PLAYER_HEIGHT * .75f)
#define ATTACK_RANGE 2.5f
#define ATTACK_TIMER 0.5f
#define HALF_ATTACK_TIMER (ATTACK_TIMER * 0.5f)
#define SWING_ANGLE Trigo::EIGHTH_CIRCLE_RAD
#define MAX_DISTANCE 4.f

//Item
#define SWORD_MODIFIER 3.f //care to change based on type of sword

// Mob
#define MOB_SPEED 2.5f
#define FLEE_SPEED 3.5f
#define AGGRO_RANGE 8
#define BLOWUP_RANGE 2
#define BLOWUP_DMG 15
#define TIME_NEEDED_TO_EXPLODE 6
#define TIME_TO_FLEE 1.5
#define MIN_TIME_BEFORE_RAND 5.f
#define MAX_TIME_BEFORE_RAND 15.f
#define WIDTH_RANGE 3
#define HEIGHT_RANGE 4
#define MIN_BEFORE_SPAWN 20.f
#define MAX_BEFORE_SPAWN 50.f
#define SPAWN_RADIUS 25.f
#define KNOCKBACK_MOD 1.2f
#define ZOMBIE_ATK_SPEED 2.5f
#define ZOMBIE_DMG 3.f
#define FLEE_TIME 3.f
#define EATING_COOLDOWN 40.f

// Block
#define FALLING_BLOCK_WAIT 2.f

// Physics
#define GRAVITY -18.f //-9.81f

//UI
#define FRAME_COUNT_SIZE 250

//Inventory
#define MAX_ITEM_PER_STACK 100
#define ITEM_NUMBER_IN_CRAFT_INVENTORY 4