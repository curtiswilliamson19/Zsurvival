#pragma once
#include "Play.h"
#include "Player.h"
#include "PlayerMovement.h"
#include "Zombie.h"
#include "ZombieMovement.h"
#include "UpdateGamePlayState.h"
#include "Ammo.h"
#include "ZombieHealth.h"
#include "Blood.h"

enum PlayState
{
	STATE_START = 0,
	STATE_APPEAR,
	STATE_PLAY,
	STATE_DEAD,
	STATE_WAIT,
	STATE_WIN
};

struct GameState
{
	float timer = 0;
	int spriteId = 0;
	int score = 0;
	int ammo = 10;
	PlayState playState = STATE_START;
	std::vector<Zombie> vZombies;
};



enum GameObjectType
{
	TYPE_NULL = -1,
	TYPE_PLAYER,
	TYPE_PLAYER_LEGS,
	TYPE_ZOMBIE,
	TYPE_PROJECTILE,
	TYPE_AMMO,
	TYPE_BLOOD
};

extern int DISPLAY_WIDTH;
extern int DISPLAY_HEIGHT;
extern int DISPLAY_SCALE;

extern GameState gameState;
extern Player player;

void UpdateGamePlayState();
void UpdatePlayer();
void UpdatePlayerMovement(char leftKey, char rightKey, char upKey, char downKey);
void UpdateZombie();
void UpdateZombieMovement();
void UpdateAmmo();
